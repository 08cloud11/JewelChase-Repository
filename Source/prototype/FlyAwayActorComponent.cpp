//--------------------------------------------------------------------
// ファイル名   ：FlyAwayActorComponent.cpp
// 概要　　　	：吹っ飛ばす処理
// 作成者　　	：神野琉生
// 更新内容　	：1/24 作成
//                1/26 吹っ飛ばす処理とフラグを逆にする処理の作成
//                2/7  吹っ飛んだ時間を見てDestroyする処理の作成
//--------------------------------------------------------------------

// インクルード
#include "FlyAwayActorComponent.h"

UFlyAwayActorComponent::UFlyAwayActorComponent()
	:_ownerActor(nullptr)
	, _destroyTime(0.0f)
	, _flyingTime(0.0f)
	, _canFlyFlag(false)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFlyAwayActorComponent::BeginPlay()
{
	Super::BeginPlay();
	_ownerActor = GetOwner<AActor>();
	_flyingTime = 0.0f;
}

void UFlyAwayActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (_canFlyFlag == true)
	{
		if (_ownerActor == nullptr)
		{
			return;
		}

		// 各軸の速度をまとめたベクトルを作成 毎フレーム高さが追加されていく
		FVector FlySpeed = FVector(_flySpeed.X, _flySpeed.Y, _flySpeed.Z *= 1.01f);

		// 現在の座標に 速度 × DeltaTime を加算
		FVector CurrentLocation = _ownerActor->GetActorLocation();
		CurrentLocation += FlySpeed * DeltaTime;

		// 計算した新しい座標に設定
		_ownerActor->SetActorLocation(CurrentLocation);

		// 飛んでいる時間を加算
		_flyingTime += DeltaTime;

		// 飛んでいる時間が壊す時間を超えたら
		if (_flyingTime >= _destroyTime)
		{
			_ownerActor->Destroy();		// このコンポーネントがついてるActorを破壊
		}
	}
}

//----------------------------------
// 関数名：ChangeFlyFlag
// 引　数：なし
// 戻り値：なし
// 吹っ飛ばす処理を行うかのフラグを逆にする
//----------------------------------
void UFlyAwayActorComponent::ChangeFlyFlag()
{
	_canFlyFlag = !_canFlyFlag;
}

