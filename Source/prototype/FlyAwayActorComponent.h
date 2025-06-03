//--------------------------------------------------------------------
// ファイル名   ：FlyAwayActorComponent.h
// 概要　　　	：吹っ飛ばす処理
// 作成者　　	：神野琉生
// 更新内容　	：1/24 作成
//                1/26 吹っ飛ばす処理とフラグを逆にする処理の作成
//                2/7  吹っ飛んだ時間を見てDestroyする処理の作成
//--------------------------------------------------------------------

// インクルードガード
#pragma once

// インクルード
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlyAwayActorComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROTOTYPE_API UFlyAwayActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFlyAwayActorComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//----------------------------------
	// 関数名：ChangeFlyFlag
	// 引　数：なし
	// 戻り値：なし
	// 吹っ飛ばす処理を行うかのフラグを逆にする
	//----------------------------------
	UFUNCTION(BlueprintCallable, Category = "Fly Settings")
	void ChangeFlyFlag();
private:
	// コンポーネントのついているActor
	UPROPERTY()
	AActor* _ownerActor;

	// 移動スピード
	UPROPERTY(EditAnywhere, Category = "Fly Settings")
	FVector _flySpeed;

	// 壊す時間
	UPROPERTY(EditAnywhere, Category = "Fly Settings")
	float _destroyTime;

	// 飛んでいる時間
	float _flyingTime;

	// 吹っ飛ばすかのフラグ
	bool _canFlyFlag;
};
