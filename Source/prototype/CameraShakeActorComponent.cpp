//--------------------------------------------------------------------
// ファイル名	：CameraShakeActorComponent.cpp
// 概要　　　	：カメラのシェイクを行う
// 作成者　　	：神野琉生
// 更新内容　	：2/1  作成
//                2/15 完成・調整済み
//--------------------------------------------------------------------

// インクルード
#include "CameraShakeActorComponent.h"
#include "Camera/CameraComponent.h"

UCameraShakeActorComponent::UCameraShakeActorComponent()
	: _ownerActor(nullptr)
	, _shakeTimeRemaining(0.0f)
	, _firstShakeTime(0.0f)
	, _shakeAmplitude(0.0f)
	, _bIsShake(false)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PostUpdateWork; // 最後にTickを実行
}

void UCameraShakeActorComponent::BeginPlay()
{
	Super::BeginPlay();
	// Ownerを格納
	_ownerActor = GetOwner<AActor>();
}

void UCameraShakeActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (_bIsShake == false && _ownerActor == nullptr)
	{
		// Ownerを格納
		_ownerActor = GetOwner<AActor>();
		return;  // 早期リターン
	}

	// シェイクの残り時間を減らす
	_shakeTimeRemaining -= DeltaTime;

	if (_shakeTimeRemaining <= 0.0f)
	{
		// シェイクの終了
		_bIsShake = false;

		// 元の位置に戻す
		if (UCameraComponent* CameraComponent = _ownerActor->FindComponentByClass<UCameraComponent>())
		{
			CameraComponent->SetRelativeLocation(FVector::ZeroVector);
		}
	}
	else
	{
		// 振幅を時間経過に応じて減衰させる
		// 1.0～0.0の間でシェイクの進行具合を表す（開始直後は１になります）FMath::Pow(累乗の元となる数,指数）
		float AmplitudeFactor = FMath::Pow(_shakeTimeRemaining / _firstShakeTime, 3.0f);
		// 負の値を防ぐ   FMath::Clamp(変数,最小値,最大値);
		AmplitudeFactor = FMath::Clamp(AmplitudeFactor, 0.0f, 1.0f);

		// 現在の振幅
		float CurrentAmplitude = _shakeAmplitude * AmplitudeFactor;

		// ランダムにカメラを揺らす
		FVector ShakeOffset = FVector(
			FMath::RandRange(-CurrentAmplitude, CurrentAmplitude), // X軸の揺れ
			FMath::RandRange(-CurrentAmplitude, CurrentAmplitude), // Y軸の揺れ
			FMath::RandRange(-CurrentAmplitude, CurrentAmplitude)  // Z軸の揺れ
		);

		// 新しい位置を設定
		if (UCameraComponent* CameraComponent = _ownerActor->FindComponentByClass<UCameraComponent>())
		{
			UE_LOG(LogTemp, Warning, TEXT("AmplitudeFactor: %f"), AmplitudeFactor);
			CameraComponent->AddLocalOffset(ShakeOffset);
		}
	}
}

//----------------------------------
// 関数名：StartCameraShakeForComponent
// 引　数：float型 シェイクする時間、flaot型 シェイクの振幅
// 戻り値：なし
// カメラのシェイクの時間と強さを設定してシェイクを開始する関数
//----------------------------------
void UCameraShakeActorComponent::StartCameraShakeForComponent(float ShakeTime, float Amplitude)
{
	// シェイクを有効化
	_bIsShake = true;
	// シェイク時間を格納
	_shakeTimeRemaining = ShakeTime;
	// 最初のシェイク時間を格納
	_firstShakeTime = ShakeTime;
	// 振幅（揺れの強さ）を格納
	_shakeAmplitude = Amplitude;
}