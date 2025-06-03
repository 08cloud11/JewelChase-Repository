//--------------------------------------------------------------------
// ファイル名：OilCan.h
// 概要　　　：オイル缶クラス
// 作成者　　：山内　将吾
// 更新内容　：2024/01/30	クラスの作成
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "prototype/Obstacle/Obstacle.h"
#include "OilCan.generated.h"

UCLASS()
class PROTOTYPE_API AOilCan : public AObstacle
{
	GENERATED_BODY()

protected:
	// ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

	// オーバーラップイベント
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
public:
	// コンストラクタ
	AOilCan();
	// 毎フレーム処理
	virtual void Tick(float DeltaTime) override;

	// 爆発のコリジョンをONにする関数
	UFUNCTION(BlueprintCallable)
	void TurnOnExplosionCollision();

private:
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* _pCapsuleComponent;		// カプセルコンポーネント
	UPROPERTY()
	class USphereComponent* _pSphereComponent; // カプセルコンポーネント
};
