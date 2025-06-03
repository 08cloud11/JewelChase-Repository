//--------------------------------------------------------------------
// ファイル名：PoliceAttackComponent.cpp
// 概要　　　：警察（敵）の攻撃するコンポーネント
// 作成者　　：山内　将吾
// 更新内容　：2024/12/03	クラスの作成
//--------------------------------------------------------------------

#include "PoliceAttackComponent.h"
#include "EnemyBullet.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "LineControllerComponent.h"
#include "Components/SplineComponent.h"
#include "prototype/SEManager.h"

// コンストラクタ
UPoliceAttackComponent::UPoliceAttackComponent()
	: _seManager(nullptr)
	, _gunSound(nullptr)
	, _owner(UEnemyAttackComponent::GetOwner())
	, _canAttackTimer(0.0f)
	, _bulletTimer(0.0f)
	, _hasFired(false)
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
	// 必要に応じて、パフォーマンス向上のために切ることもできる。
	PrimaryComponentTick.bCanEverTick = true;
}

// ゲームスタート時、または生成時に呼ばれる処理
void UPoliceAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// コンポーネントの取得
	_pLineControllerComponent = GetOwner()->FindComponentByClass<ULineControllerComponent>();		// 予測線を出すためのコンポーネント
	if (!_pLineControllerComponent)
	{
		// デバッグ用
		//UE_LOG(LogTemp, Warning, TEXT("LineControllerComponent not found on Owner"));
	}

	// 妥協処理 
	// 指定した角度に回転
	FRotator NewRotation(0.0f, 180.0f, 0.0f);
	_owner->SetActorRotation(NewRotation);  // Actorを指定した回転に設定

	// 妥協処理
	FName ItemTag = "Item";
	FName SmogTag = "Smog";
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), ItemTag, _pItems);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), SmogTag, _pSmogs);

	// トレース用のクエリパラメータを設定
	_queryParamsToTrace.AddIgnoredActor(_owner);	// 自分のキャラクターを無視
	_queryParamsToTrace.AddIgnoredActors(_pSmogs);	// 霧全てをを無視
	_queryParamsToTrace.AddIgnoredActors(_pItems);	// アイテム全てを無視

	// サウンドマネージャーのインスタンスを取得
	_seManager = USEManager::GetInstance(this);
}

// 毎フレーム処理
void UPoliceAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// 経過時間の計測
	_canAttackTimer += DeltaTime;
	
	// 指定した時間経過したら予測線を出して打つ構えをする
	if (_timeToAttack <= _canAttackTimer)
	{
		_isAttackAble = true;
	}
}

// 関数名：EnemyAttack 
// 引数：１フレームにかかる時間
// 戻り値：なし
// 内容：警察の攻撃を行うオーバーラップ関数　
void UPoliceAttackComponent::EnemyAttack(float deltaTime)
{
	// 【デバッグ用】
	//UE_LOG(LogTemp, Log, TEXT("Invoke PoliceAttack()"));

	// NULLチェック	
	if (!_enemyBulletActor) {/* UE_LOG(LogTemp, Warning, TEXT("Nothing EnemyBullet"));*/ return; }	// 生成する弾があるかどうかチェック
	if (!_owner) { /*UE_LOG(LogTemp, Warning, TEXT("Failed To Acquire Owner"));*/  return; }		// コンポーネントのオーナーである Actor があるかチェック

	// 経過時間の計測
	_bulletTimer += deltaTime;

	// 予測線を出す
	DrawPredictionSplineWithCollision(deltaTime);

	// クールタイム処理
	if (!_hasFired) // まだ弾を発射していない場合
	{
		// 構えている状態（予測線を出す）
		if (_bulletTimer <= _timeToFire - _predictionStopToFireInterval)
		{
			//UE_LOG(LogTemp, Log, TEXT("Style"));	// デバッグ用
			EnemyState = EnemyStatus::Style;		// アニメーション用のステートを「Style」にする
			// 予測線を表示にする
			if (!_pLineControllerComponent) { /*UE_LOG(LogTemp, Warning, TEXT("Not _pLineControllerComponent"));*/ return; }
			_pLineControllerComponent->DisplayPredictionLineActor();
		}
		// 打つ方向が決まっていて予測線が消える時間
		else if (_timeToFire - _predictionStopToFireInterval <= _bulletTimer)
		{
			// 予測線を非表示にする
			if (!_pLineControllerComponent) { /*UE_LOG(LogTemp, Warning, TEXT("Not _pLineControllerComponent"));*/ return; }
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

// 関数名：SpawnBullet
// 引数：なし
// 戻り値：なし
// 内容；弾をインスタンス化する関数
void UPoliceAttackComponent::SpawnBullet()
{
	// 発射位置を決定（Policeの位置 + 警察が向いている方向(単位ベクトル) * _enemyBulletOffset）
	FVector _spawnLocation = _owner->GetActorLocation() + _owner->GetActorForwardVector() * _enemyBulletOffset;
	// Actorの向き（前方ベクトル）
	FRotator _spawnRotation = _owner->GetActorRotation();

	// EnemyBullet を生成
	AEnemyBullet* _pSpawnedEnemyBullet = GetWorld()->SpawnActor<AEnemyBullet>(_enemyBulletActor, _spawnLocation, _spawnRotation);

	if (_pSpawnedEnemyBullet)
	{
		_pSpawnedEnemyBullet->SetDirection(_owner->GetActorForwardVector());
		_pSpawnedEnemyBullet->SetBulletSpeed(_bulletSpeed);
		_pSpawnedEnemyBullet->SetOwner(_owner);
	}

	// 射撃音の設定
	_seManager->RegisterSE(TEXT("_gunSound"), _gunSound);
	// 射撃音を 1 回だけ再生
	_seManager->PlaySE2D(TEXT("_gunSound"));
 }

// 関数名：DrawPredictionSplineWithCollision
// 引数：１フレームにかかる時間
// 戻り値：なし
// 内容：自分の位置から前方に予測線を出す関数
void UPoliceAttackComponent::DrawPredictionSplineWithCollision(float deltaTime)
{
	if (!_pLineControllerComponent) { /*UE_LOG(LogTemp, Warning, TEXT("Not _pLineControllerComponent"));*/ return; }

	// 警察の位置と向いている方向を取得
	FVector _start = _owner->GetActorLocation();  // 警察の位置
	FRotator _rotation = _owner->GetActorRotation();  // 警察の回転
	FVector _forwardDirection = _rotation.Vector();  // 警察の向いている方向のベクトル

	// 線を飛ばす距離を指定
	float _traceDistance = 5000.0f;
	FVector _end = _start + _forwardDirection * _traceDistance;  // 終点の位置

	// 衝突判定
	FHitResult HitResult;

	// Line Trace 実行
	bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, _start, _end, FCollisionObjectQueryParams::AllObjects, _queryParamsToTrace);
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
			_end = HitResult.Location; // -ForwardDirection * 350.0f;  // 少し手前に調整
		}
		// 衝突したActorを表示
		//UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *HitActor->GetName());
	}
	//else
	//{
	//  // デバッグ用
	//	// 衝突しなかった場合
	//	//UE_LOG(LogTemp, Log, TEXT("No hit"));
	//}

	// デバッグレイ
	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 1.0f);

	// デバッグ用
	//UE_LOG(LogTemp, Log, TEXT("PoliceAttackComopnent Start: %s, End: %s"), *Start.ToString(), *End.ToString());

	// 予測線の更新
	_pLineControllerComponent->UpdateLineActor(_start, _end);
}