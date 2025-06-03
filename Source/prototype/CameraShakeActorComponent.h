//--------------------------------------------------------------------
// ファイル名	：CameraShakeActorComponent.h
// 概要　　　	：カメラのシェイクを行う
// 作成者　　	：神野琉生
// 更新内容　	：2/1  作成
//                2/15 完成・調整済み
//--------------------------------------------------------------------

// インクルードガード
#pragma once

// インクルード
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "CameraShakeActorComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROTOTYPE_API UCameraShakeActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCameraShakeActorComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//----------------------------------
	// 関数名：StartCameraShakeForComponent
	// 引　数：float型 シェイクする時間、flaot型 シェイクの振幅
	// 戻り値：なし
	// カメラのシェイクの時間と強さを設定してシェイクを開始する関数
	//----------------------------------
	/* ForCameraShakeActorComponent */
	UFUNCTION(BlueprintCallable, Category = "CameraShake")
	void StartCameraShakeForComponent(float Duration, float Amplitude);

private:
	// コンポーネントのついているActor
	UPROPERTY()
	AActor* _ownerActor;

	// シェイクの残り時間
	float _shakeTimeRemaining;
	// 最初のシェイクの時間
	float _firstShakeTime;
	// シェイクの振幅
	float _shakeAmplitude;

	// シェイク中かどうかのフラグ
	bool _bIsShake;
};
