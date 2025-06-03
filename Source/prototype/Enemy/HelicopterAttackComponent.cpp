//--------------------------------------------------------------------
// ファイル名：HelicopterAttackComponent.cpp
// 概要　　　：ヘリコプター（敵）の攻撃するコンポーネント
// 作成者　　：山内　将吾
// 更新内容　：2024/12/31	クラスの作成
//--------------------------------------------------------------------

#include "HelicopterAttackComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyBullet.h"
#include "LineControllerComponent.h"
#include "Components/SplineComponent.h"
#include "prototype/SEManager.h"

// コンストラクタ
UHelicopterAttackComponent::UHelicopterAttackComponent()
	: _playerActor(nullptr)
	, _seManager(nullptr)
	, _rotorSound(nullptr)
	, _gunSound(nullptr)
	, _owner(UEnemyAttackComponent::GetOwner())
	, _moveTargetActor(nullptr)
	, _pLineControllerComponent(nullptr)
	, _startPointToDrawPrediction(0.0f, 0.0f, 0.0f)
	, _endPointToDrawPrediction(0.0f, 0.0f, 0.0f)
	, _rotationToFire(0.0f,0.0f,0.0f)
	, _canAttackTimer(0.0f)
	, _bulletTimer(0.0f)
	, _hasFired(false)
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
	// 必要に応じて、パフォーマンス向上のために切ることもできる。
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// ゲームスタート時、または生成時に呼ばれる処理
void UHelicopterAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// もしヘリがステージに存在する場合は消去
	TArray<AActor*> _searchHelicopters;
	// ワールドからタグに一致するアクターを取得
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Helicopter"), _searchHelicopters);
	// 見つかったアクターに対して処理を行う
	for (AActor* Actor : _searchHelicopters)
	{
		if (Actor != this->_owner)
		{
			// デバッグ用
			//UE_LOG(LogTemp, Warning, TEXT("The helicopter is already there."));
			// 消去
			this->_owner->Destroy();

			return;
		}
	}


	// コンポーネントの取得
	_pLineControllerComponent = GetOwner()->FindComponentByClass<ULineControllerComponent>();		// 予測線を出すためのコンポーネント
	if (!_pLineControllerComponent)
	{
		// デバッグ用
		//UE_LOG(LogTemp, Warning, TEXT("LineControllerComponent not found on Owner"));
	}

	// タグを持つアクターの配列を宣言
	TArray<AActor*> _foundActors;

	// ワールドからタグに一致するアクターを取得
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), _foundActors);

	// 見つかったアクターに対して処理を行う
	for (AActor* Actor : _foundActors)
	{
		if (Actor)
		{
			// プレイヤーアクターを保持する
			_playerActor = Actor;
			//UE_LOG(LogTemp, Log, TEXT("Found Actor: %s"), *Actor->GetName());
		}
	}

	// 妥協処理
	TArray<AActor*> _searchMoveTargetActors;
	// ワールドからタグに一致するアクターを取得
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("MoveTarget"), _searchMoveTargetActors);
	// 見つかったアクターに対して処理を行う
	for (AActor* Actor : _searchMoveTargetActors)
	{
		//UE_LOG(LogTemp, Log, TEXT("MoveTargetActor is Active"));
		if (Actor->ActorHasTag("Right"))
		{
			// 登録
			_moveTargetActors[0] = Actor;
		}
		if (Actor->ActorHasTag("Center"))
		{
			// 登録
			_moveTargetActors[1] = Actor;
		}
		if (Actor->ActorHasTag("Left"))
		{
			// 登録
			_moveTargetActors[2] = Actor;
		}
	}

	// レイキャストで無視するアクターを検索・保持
	FName _itemTag = "Item";
	FName _smogTag = "Smog";
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), _itemTag, _pItems);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), _smogTag, _pSmogs);

	// トレース用のクエリパラメータを設定
	_queryParamsToTrace.AddIgnoredActor(_owner);	// 自分のキャラクターを無視
	_queryParamsToTrace.AddIgnoredActors(_pSmogs);	// 霧全てをを無視
	_queryParamsToTrace.AddIgnoredActors(_pItems);	// アイテム全てを無視

	// 移動目標アクターを設定
	// 妥協処理
	TSubclassOf<AStageManager> _findClass;
	_findClass = AStageManager::StaticClass();

	AActor* stageManager = UGameplayStatics::GetActorOfClass(GetWorld(), _findClass);
	SetMoveTargetNumber(Cast<AStageManager>(stageManager)->GetHeliStatus());
	_setMoveTargetActor();

	// サウンドマネージャーのインスタンスを取得
	_seManager = USEManager::GetInstance(this);
	// ローター音の設定
	_seManager->RegisterSE(TEXT("_rotorSound"), _rotorSound);
	// オーター音を 1 回だけ再生
	_seManager->PlaySE2D(TEXT("_rotorSound"));
}

// 毎フレーム処理
void UHelicopterAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 目標地点まで移動する
	_moveToTargetLocation(DeltaTime);

	// デバッグ用
	//_isAttackAble = true;	// 攻撃可能にする
}

// 関数名：_moveToTargetLocation
// 引数：_deltaTime：１フレームにかかる秒数
// 戻り値：なし
// 内容：ターゲットのポイントまで移動する関数
void UHelicopterAttackComponent::_moveToTargetLocation(float deltaTime)
{
	if (!_owner) { /*UE_LOG(LogTemp, Warning, TEXT("Helicopter / Failed To Acquire Owner"));*/ return; }		// コンポーネントのオーナーである Actor があるかチェック
	if (!_moveTargetActor) { /*UE_LOG(LogTemp, Warning, TEXT("Helicopter / Failed To Acquire MoveTargetActor"));*/ return; }		// 目標地点の Actor があるかチェック

	FVector StartLocation = _owner->GetActorLocation();   // 現在の位置を設定
	FVector TargetLocation = _moveTargetActor->GetActorLocation();  // 目標地点の取得

	// 経過時間を更新
	_canAttackTimer += deltaTime;

	// 移動中の処理
	if (_canAttackTimer < _canAttackTime)
	{
		//UE_LOG(LogTemp, Log, TEXT("Helicopter is Moving"));
		// 線形進行度を計算
		// 妥協処理
		float _linearAlpha = FMath::Clamp(_canAttackTimer / _canAttackTime * 0.02f, 0.0f, 1.0f);

		// イージング関数を適用
		// EaseAlphaは0～1の範囲に収める
		float _easeAlpha = FMath::Clamp(FMath::InterpEaseOut(0.0f, 1.0f, _linearAlpha, 2.0f), 0.0f, 1.0f);

		// 新しい位置を計算
		FVector _newLocation = FMath::Lerp(StartLocation, TargetLocation, _easeAlpha);

		// 新しい位置にアクターを移動
		_owner->SetActorLocation(_newLocation);
	}
	else
	{
		// 経過時間が移動時間に達したら移動終了
		// 妥協処理
		//_owner->SetActorLocation(TargetLocation);  // 最終的な位置に補正
		// 妥協処理
		if (_canAttackTime + _timeToAttack < _canAttackTimer)
		{
			_isAttackAble = true;	// 攻撃可能にする
		}
	}
}

// 関数名：EnemyAttack
// 引数：１フレームにかかる時間
// 戻り値：なし
// 内容：攻撃を仕掛ける関数(弾の予測線を出して弾を発射する)
void UHelicopterAttackComponent::EnemyAttack(float deltaTime)
{
	// 【デバッグ用】
	//UE_LOG(LogTemp, Log, TEXT("Invoke PoliceAttack()"));

	// NULLチェック	
	if (!EnemyBulletClass) { /*UE_LOG(LogTemp, Log, TEXT("Nothing EnemyBullet"));*/ return; }	// 生成する弾があるかどうかチェック
	if (!_owner) { /*UE_LOG(LogTemp, Log, TEXT("Failed To Acquire Owner"));*/ return;}		// コンポーネントのオーナーである Actor があるかチェック

	// 経過時間の計測
	_bulletTimer += deltaTime;

	// クールタイム処理
	if (!_hasFired) // まだ弾を発射していない場合
	{
		// 構えている状態（予測線を出す）
		if (_bulletTimer <= _timeToFire - _predictionStopToFireInterval)
		{
			//UE_LOG(LogTemp, Log, TEXT("Style"));	// デバッグ用
			EnemyState = EnemyStatus::Style;		// アニメーション用のステートを「Style」にする
			// 予測線を出す
			DrawPredictionSplineWithCollision(deltaTime);
			// 予測線を表示にする
			if (!_pLineControllerComponent) { /*UE_LOG(LogTemp, Warning, TEXT("Not _pLineControllerComponent")); */return; }
			_pLineControllerComponent->DisplayPredictionLineActor();
		}
		// 打つ方向が決まっていて予測線が消える時間
		else if (_timeToFire - _predictionStopToFireInterval <= _bulletTimer)
		{
			// 予測線を非表示にする
			if (!_pLineControllerComponent) {/* UE_LOG(LogTemp, Warning, TEXT("Not _pLineControllerComponent"));*/ return; }
			_pLineControllerComponent->HidePredictionLineActor();
		}
		// 弾を打つ
		if (_timeToFire <= _bulletTimer)
		{
			//UE_LOG(LogTemp, Log, TEXT("Fire"));		// デバッグ用
			EnemyState = EnemyStatus::Fire;			// アニメーション用のステートを「Fire」にする
			SpawnBullet();         // 弾を発射
			_hasFired = true;      // 発射フラグを立てる
			_bulletTimer = 0.0f;   // タイマーをリセット
		}
	}
	else // 発射後のクールタイムを計測
	{
		if (_enemyAttackCooldown <= _bulletTimer)
		{
			//UE_LOG(LogTemp, Log, TEXT("Idel"));		// デバッグ用
			EnemyState = EnemyStatus::Idle;			// アニメーション用のステートを「Idel」にする
			_bulletTimer = 0.0f;   // クールダウン終了、タイマーをリセット
			_hasFired = false;     // 再び発射可能にする
		}
	}
}

// 関数名：DrawPredictionSplineWithCollision
// 引数：１フレームにかかる時間
// 戻り値：なし
// 内容：自分の位置からキャストして
void UHelicopterAttackComponent::DrawPredictionSplineWithCollision(float deltaTime)
{
	if (!_pLineControllerComponent) { /*UE_LOG(LogTemp, Warning, TEXT("Not _pLineControllerComponent"));*/ return; }
	if (!_playerActor) { /*UE_LOG(LogTemp, Warning, TEXT("Not Find Player"));*/ return; }

	// ヘリの位置と向いている方向を取得
	_startPointToDrawPrediction = _owner->GetActorLocation();  // 視点の位置
	FRotator _rotation = _owner->GetActorRotation();  // 視点の回転
	FVector _forwardDirection = _rotation.Vector();  // 向いている方向のベクトル

	// 線を飛ばす距離を指定
	_endPointToDrawPrediction = _playerActor->GetActorLocation();	// 終点の位置

	// 衝突判定
	FHitResult HitResult;

	// Line Trace 実行
	bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, _startPointToDrawPrediction, _endPointToDrawPrediction, FCollisionObjectQueryParams::AllObjects, _queryParamsToTrace);
	if (bHit)
	{
		// 衝突した場合、ヒットした場所に対する処理
		//UE_LOG(LogTemp, Log, TEXT("Hit: %s"), *HitResult.GetActor()->GetName());	// 衝突相手を出力
		//UE_LOG(LogTemp, Log, TEXT("Hit at: %s"), *HitResult.ImpactPoint.ToString());  // 衝突点の座標をログに出力

		// ヒットしたアクターを取得
		AActor* _pHitActor = HitResult.GetActor();
		if (_pHitActor)
		{
			// ヒットしたアクターの座標を取得
			_endPointToDrawPrediction = HitResult.Location;

			// 補正分Endの座標がStartより大きくなったら補正
			if (_startPointToDrawPrediction.X <= _endPointToDrawPrediction.X)
			{
				_endPointToDrawPrediction = _startPointToDrawPrediction;
			}
		}

		// 衝突したActorを表示
		//UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *HitActor->GetName());
	}
	else
	{
		// 衝突しなかった場合
		//UE_LOG(LogTemp, Log, TEXT("No hit"));
	}

	// デバッグレイ
	//DrawDebugLine(GetWorld(), _startPointToDrawPrediction, _endPointToDrawPrediction, FColor::Green, false, 1.0f, 0, 1.0f);
	// デバッグ用
	//UE_LOG(LogTemp, Log, TEXT("PoliceAttackComopnent Start: %s, End: %s"), *Start.ToString(), *End.ToString());

	// 予測線の更新
	_pLineControllerComponent->UpdateLineActor(_startPointToDrawPrediction, _endPointToDrawPrediction);

	// ヘリからプレイヤーへの方向ベクトルを取得（始点：ヘリの座標、終点：プレイヤーの座標）
	_rotationToFire = CalculateRotationByPoints(_owner->GetActorLocation(), _playerActor->GetActorLocation());
}

// 関数名：CalculateRotationByPoints
// 引数：始点の座標, 終点の座標
// 戻り値：ヘリからプレイヤーへの方向ベクトル
// 内容：渡された始点から終点への方向ベクトルを計算する関数
FRotator UHelicopterAttackComponent::CalculateRotationByPoints(const FVector& startPoint, const FVector& endPoint)
{
	// 方向ベクトルを計算
	FVector Direction = endPoint - startPoint;
	// 正規化して単位ベクトルに変換
	FVector NormalizedDirection = Direction.GetSafeNormal();
	// FRotatorを取得
	FRotator Rotation = NormalizedDirection.Rotation();

	return Rotation;
}

// 関数名：SpawnBullet
// 引数：なし
// 戻り値：なし
// 内容：弾をインスタンス化する関数
void UHelicopterAttackComponent::SpawnBullet()
{
	if (!_playerActor) { /*UE_LOG(LogTemp, Warning, TEXT("Not Find Player"));*/ return; }

	// 発射位置を決定（ヘリの位置 + ヘリが向いている方向(単位ベクトル) * _enemyBulletOffset）
	FVector SpawnLocation = _owner->GetActorLocation() + _owner->GetActorForwardVector() * _enemyBulletOffset;

	// EnemyBullet を生成
	AEnemyBullet* _spawnedEnemyBullet = GetWorld()->SpawnActor<AEnemyBullet>(EnemyBulletClass, SpawnLocation, _rotationToFire);
	if (_spawnedEnemyBullet)
	{
		_spawnedEnemyBullet->SetDirection(_rotationToFire.Vector());
		_spawnedEnemyBullet->SetBulletSpeed(_bulletSpeed);
		_spawnedEnemyBullet->SetOwner(_owner);
	}	

	// 妥協処理
	// 画面外で音を鳴らさないようにする
	if (_owner->GetActorLocation().X >= 0.0f)
	{
		if (_seManager)
		{
			// 射撃音の設定
			_seManager->RegisterSE(TEXT("_gunSound"), _gunSound);
			// 射撃音を 1 回だけ再生
			_seManager->PlaySE2D(TEXT("_gunSound"));
		}
	}
}

// 関数名：_setMoveTargetActor
// 引数：なし
// 戻り値：なし
// 内容：ヘリコプターの移動目標のターゲットを設定する関数
void UHelicopterAttackComponent::_setMoveTargetActor()
{
	// 移動目標アクターを設定
	switch (_moveTargetNumber)
	{
	// 右側
	case 1:
		if (_moveTargetActors)
		{
			_moveTargetActor = _moveTargetActors[0];
			// 左向きにする
			// 回転値を指定
			FRotator NewRotation = FRotator(0.0f, 180.0f, 0.0f);
			// アクターの回転を設定
			_owner->SetActorRotation(NewRotation);
		}
		//else
		//{
		//	// デバッグ用
		//	//UE_LOG(LogTemp, Warning, TEXT("_moveTargetActors[0] do not set"));
		//}
		break;
	// 中央
	case 2:
		if (_moveTargetActors[1])
		{
			_moveTargetActor = _moveTargetActors[1];
		}
		//else
		//{
		//	// デバッグ用
		//	UE_LOG(LogTemp, Warning, TEXT("_moveTargetActors[1] do not set"));
		//}
		break;
	// 左側
	case 3:
		if (_moveTargetActors[2])
		{
			_moveTargetActor = _moveTargetActors[2];
		}
		//else
		//{
		//	// デバッグ用
		//	UE_LOG(LogTemp, Warning, TEXT("_moveTargetActors[2] do not set"));
		//}
		break;
	default:
		// デバッグ用
		//UE_LOG(LogTemp, Warning, TEXT("Do not set _moveTargetActors"));
		break;
	}
}

// 妥協処理
void UHelicopterAttackComponent::SetMoveTargetNumber(int moveTargetNumber)
{
	//UE_LOG(LogTemp, Log, TEXT("Invoke SetMoveTargetNumber"));
	// 移動目標アクターを設定
	switch (moveTargetNumber)
	{
		// 右側
	case 1:
		_moveTargetNumber = 1;
		//UE_LOG(LogTemp, Log, TEXT("MoveTargetNumber is Right"));// デバッグ用
		break;
		// 中央
	case 2:
		_moveTargetNumber = 2;
		//UE_LOG(LogTemp, Log, TEXT("MoveTargetNumber is Center"));// デバッグ用
		break;
		// 左側
	case 3:
		_moveTargetNumber = 3;
		//UE_LOG(LogTemp, Log, TEXT("MoveTargetNumber is Left"));// デバッグ用
		break;
	default:
		//UE_LOG(LogTemp, Log, TEXT("MoveTargetNumber is None"));	// デバッグ用
		break;
	}
}

// 関数名：EndPlay
// 引数：なし
// 戻り値：なし
// 内容：ヘリコプターが削除されたときに SE が流れていたら止める
void UHelicopterAttackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (_seManager)
	{
		// SE を止める
		_seManager->StopSE(TEXT("_rotorSound"));

		//UE_LOG(LogTemp, Warning, TEXT("_rotorSound is Stop"));
	}
}
