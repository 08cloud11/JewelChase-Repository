//--------------------------------------------------------------------
//制作者　：坂上　壱希
//制作日時：2024/12/23
//最終更新：2024/12/23
//概　要　：プレイヤーの動き部分
//--------------------------------------------------------------------
#pragma once

#include "prototype/Interface/IMovement.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "MoveRightComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROTOTYPE_API UMoveRightComponent : public UActorComponent, public IMovement
{
	GENERATED_BODY()
	//
public:
	UMoveRightComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Move(float AxisValue) override;

private:
	UPROPERTY()
	TObjectPtr<ACharacter> _ownerCharacter = nullptr;

	UPROPERTY(EditAnywhere)
	float _moveSpeed = 7.0f;

};
