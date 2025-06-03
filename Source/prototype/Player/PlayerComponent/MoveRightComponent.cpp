//--------------------------------------------------------------------
//制作者　：坂上　壱希
//制作日時：2024/12/23
//最終更新：2024/12/23
//概　要　：プレイヤーの動き部分
//--------------------------------------------------------------------
#include "MoveRightComponent.h"

UMoveRightComponent::UMoveRightComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UMoveRightComponent::BeginPlay()
{
	Super::BeginPlay();

	_ownerCharacter = Cast<ACharacter>(GetOwner());
}

void UMoveRightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMoveRightComponent::Move(float AxisValue)
{
	/*
	//右
	if (AxisValue > 0.0f)
	{
		FRotator NewRotation = FRotator(0.0f, 90.0f + _rotation, 0.0f);
		//↓何故かYaw(Z軸回転)だけがゲームに反映されない
		//SetActorRotation(NewRotation);

		GetMesh()->SetWorldRotation(NewRotation);	//代用案
	}
	//左
	else if (AxisValue < 0.0f)
	{
		FRotator newRotation = FRotator(0.0f, 90.0f + (-_rotation), 0.0f);
		//SetActorRotation(NewRotation);

		GetMesh()->SetWorldRotation(newRotation);
	}
	else
	{
		FRotator newRotation = FRotator(0.0f, 90.0f, 0.0f);
		//SetActorRotation(NewRotation);

		GetMesh()->SetWorldRotation(newRotation);
	}
	*/

#if false
	// 右方向のベクトルを取得し、その方向に移動
	//現在座標を取得
	FVector mypos = GetOwner()->GetActorLocation();
	//現在座標から加算分を計算
	mypos.Y += AxisValue * _moveSpeed;
	//実際の座標に加算
	GetOwner()->SetActorLocation(mypos);
#endif // false


	_ownerCharacter->AddMovementInput(_ownerCharacter->GetActorRightVector(), AxisValue);

}

