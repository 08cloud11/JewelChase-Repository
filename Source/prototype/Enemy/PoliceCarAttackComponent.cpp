//--------------------------------------------------------------------
// ファイル名：PoliceCarAttackComponent.cpp
// 概要　　　：パトカーを制御するクラス
// 作成者　　：山内　将吾
// 更新内容　：2025/01/26	クラスの作成 
//--------------------------------------------------------------------

#include "PoliceCarAttackComponent.h"
#include "PoliceCar_Warning.h"
#include "Components/BoxComponent.h"
#include "prototype/SEManager.h"

// コンストラクタ
UPoliceCarAttackComponent::UPoliceCarAttackComponent()
	: _seManager(nullptr)
	, _owner(UEnemyAttackComponent::GetOwner())
	, _pBoxCollisionComponent(nullptr)
	, _currentSpeed()
	, _timer(0.0f)
	, _exitTimer(0.0f)
	, _appearTimer(0.0f)
	, _isAppear(0.0f)
	, _isSound(false)
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
	// 必要に応じて、パフォーマンス向上のために切ることもできる。
	PrimaryComponentTick.bCanEverTick = true;
}

// ゲームスタート時、または生成時に呼ばれる処理
void UPoliceCarAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// 妥協処理
	// UE側で設定した BoxComponent を見つけて取得する
	_pBoxCollisionComponent = _owner->FindComponentByClass<UBoxComponent>();
	if (_pBoxCollisionComponent != nullptr)
	{
		// コリジョンプリセットを NoCollision に設定（当たり判定なし）
		_pBoxCollisionComponent->SetCollisionProfileName(TEXT("NoCollision"));
	}

	// 初期位置を取得
	_initialLocation = _owner->GetActorLocation();
	// 目標位置を取得
	_appearTargetLocation = _initialLocation + _forwardTargetLocation;
	// 警告をスポーンさせる
	APoliceCar_Warning* SpawnedWarning = GetWorld()->SpawnActor<APoliceCar_Warning>(_policeCar_WarningClass, _initialLocation + _warningActorSpawnLocation, FRotator::ZeroRotator);

	//インスタンスを取得
	_seManager = USEManager::GetInstance(this);

	if (_seManager)
	{
		// 警告音 を登録
		_seManager->RegisterSE(TEXT("_warningSound"), _warningSound);
		// 警告音 を 1 回だけ再生
		_seManager->PlaySE2D(TEXT("_warningSound"));
	}
}

// 毎フレーム処理
void UPoliceCarAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 経過時間計測
	_timer += DeltaTime;

	// 登場していなかったら処理
	if (!_isAppear)
	{
		// 指定した時間経過したら登場させる
		if (_appearStartTime <= _timer)
		{
			// 登場させる
			_appearAtLocation(DeltaTime);
		}
	}
	else
	{
		// 登城してから指定した時間経過したら退場させる
		if (_exitStartTime <= _timer)
		{
			// 退場させる
			_moveBackAndExit(DeltaTime);
		}
	}
	
}

// 関数名：_enterAtLocation
// 引数：_deltaTime：１フレームにかかる秒数
// 戻り値：なし
// 内容：ターゲットの座標まで移動しながら登場する関数
void UPoliceCarAttackComponent::_appearAtLocation(float deltaTime)
{
	if (!_owner) { /*UE_LOG(LogTemp, Warning, TEXT("PoliceCar / Failed To Acquire Owner"));*/ return;}		// コンポーネントのオーナーである Actor があるかチェック

	FVector _currentLocation = _owner->GetActorLocation();   // 現在の位置を設定

	// 経過時間を更新
	_appearTimer += deltaTime;

	// 移動中の処理
	if (_appearTimer < _appearDuration)
	{
		//UE_LOG(LogTemp, Log, TEXT("Helicopter is Moving"));
		// 線形進行度を計算
		// 妥協処理
		float LinearAlpha = FMath::Clamp(_appearTimer / _appearDuration * 0.02f, 0.0f, 1.0f);

		// イージング関数を適用
		// EaseAlphaは0～1の範囲に収める
		float EaseAlpha = FMath::Clamp(FMath::InterpEaseOut(0.0f, 1.0f, LinearAlpha, 2.0f), 0.0f, 1.0f);

		// 新しい位置を計算
		FVector NewLocation = FMath::Lerp(_currentLocation, _appearTargetLocation, EaseAlpha);

		// 新しい位置にアクターを移動
		_owner->SetActorLocation(NewLocation);

		// 妥協処理
		// 一回だけ処理
		if (!_isSound)
		{
			// サイレン音 を登録
			_seManager->RegisterSE(TEXT("_sirenSound"), _sirenSound);
			// サイレン音 を 1 回だけ再生
			_seManager->PlaySE2D(TEXT("_sirenSound"));

			_isSound = true;
		}
		
		// 妥協処理
		// コリジョンプリセットを Enemy にする
		if (_pBoxCollisionComponent != nullptr)
		{
			// コリジョンプリセットを Enemy に設定（トランプ軌道保管対象にする）
			_pBoxCollisionComponent->SetCollisionProfileName(TEXT("Enemy"));
		}
	}
	else
	{
		_isAppear = true;	// 登場完了
		_timer = 0.0f;	// タイマーリセット
	}
}

// 関数名：_moveBackAndExit
// 引数：_deltaTime：１フレームにかかる秒数
// 戻り値：なし
// 内容：後退させて退場させる関数
void UPoliceCarAttackComponent::_moveBackAndExit(float deltaTime)
{
	// 経過時間を加算
	_exitTimer += deltaTime;

	// 現在の位置を取得
	FVector _currentLocation = _owner->GetActorLocation();

	// 時間が経過しすぎた場合の調整
	if (_exitTimer >= _exitDuration)
	{
		// 妥協処理
		_owner->Destroy();
	}

	// 徐々に加速させる補間係数（例：時間の二乗で加速）
	float Alpha = FMath::Clamp(_exitTimer / _exitDuration, 0.0f, 1.0f);
	float AcceleratedAlpha = FMath::Pow(Alpha, _exitMoveSpeed);  // 時間の二乗で加速

	// 線形補間で位置を計算
	FVector NewLocation = FMath::Lerp(_currentLocation, _initialLocation, AcceleratedAlpha);

	// アクターの位置を更新
	_owner->SetActorLocation(NewLocation);

	// 妥協処理
	// UE側で設定した BoxComponent を見つけて取得する
	_pBoxCollisionComponent = _owner->FindComponentByClass<UBoxComponent>();
	if (_pBoxCollisionComponent != nullptr)
	{
		// コリジョンプリセットを NoCollision に設定（当たり判定なし）
		_pBoxCollisionComponent->SetCollisionProfileName(TEXT("NoCollision"));
	}
}


// 攻撃を仕掛ける関数(弾の予測線を出して弾を発射する)
// 関数名：EnemyAttack 
// 引数：１フレームにかかる時間
// 内容：警察の攻撃を行うオーバーラップ関数　
void UPoliceCarAttackComponent::EnemyAttack(float deltaTime)
{
	;
}
