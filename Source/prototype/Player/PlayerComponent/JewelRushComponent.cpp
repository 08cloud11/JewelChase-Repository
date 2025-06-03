//--------------------------------------------------------------------
//制作者　：坂上　壱希
//制作日時：2024/12/23
//最終更新：2024/12/23
//概　要　：プレイヤーJewelRush
//--------------------------------------------------------------------
#include "JewelRushComponent.h"

UJewelRushComponent::UJewelRushComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UJewelRushComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UJewelRushComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

