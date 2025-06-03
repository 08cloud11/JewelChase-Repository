//--------------------------------------------------------------------
// ファイル名：PlayerHitByPoliceCarComponent.h
// 概要　　　：プレイヤーがパトカーに当たった際に弾く処理を行うためのコンポーネント
// 作成者　　：山内　将吾
// 更新内容　：2024/02/01	クラスの作成
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerHitByPoliceCarComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROTOTYPE_API UPlayerHitByPoliceCarComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:
	// コンストラクタ
	UPlayerHitByPoliceCarComponent();
	// 毎フレーム処理
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ClampPlayerYLocation(float playerMaxLocationY, float playerMinLocationY);	// プレイヤーのY座標の制限値を取得する関数

	void TriggerKnockBack(AActor* playerActor, AActor* policeCarActor);	// 弾きのトリガーをONにする関数

private:

	UPROPERTY()
	AActor* _playerActor;  			// プレイヤー格納用
	UPROPERTY()
	AActor* _policeCarActor;		// パトカー格納用

	UPROPERTY(EditAnywhere)
	FVector _knockBackStrength;	// 弾かれる強さ
	UPROPERTY(EditAnywhere)
	float _knockbackCheckDistancef; // 弾く方向に壁があるかどうか確かめる際の計る距離
	UPROPERTY(EditAnywhere)
	float _knockbackDuration;		// ノックバックを行う全体の時間

	FVector _playerLocation;		// プレイヤーの座標
	FVector _relativeLocation;		// プレイヤーとパトカーの相対位置
	FVector _knockbackDirection;	// ノックバックの方向
	float _crossProductZ;			// プレイヤーとパトカーの外積Z成分
	float _timer;					// 経過時間測定用
	float _playerMaxLocationY;	// プレイヤーの最大Y座標
	float _playerMinLocationY;	// プレイヤーの最大Y座標
	bool _isHitPoliceCar;			// パトカーに当たったかどうかのフラグ

	void _knockBack();	// パトカーに弾かれる関数
	void _calculateKnockbackDirection();		// ノックバックする方向を計算する関数
	bool _isWallInDirection(const FVector policeCarLocation);	// 弾く方向に壁があるか確かめる関数
};