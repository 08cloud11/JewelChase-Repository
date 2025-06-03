//--------------------------------------------------------------------
//制作者　：坂上　壱希
//制作日時：2024/12/23
//最終更新：2024/12/23
//概　要　：プレイヤーJewelRush
//--------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JewelRushComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOTYPE_API UJewelRushComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UJewelRushComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
