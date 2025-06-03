//--------------------------------------------------------------------
// ファイル名：Obstacle.h
// 概要　　　：障害物の基底クラス
// 作成者　　：山内　将吾
// 更新内容　：2024/01/30	クラスの作成
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class PROTOTYPE_API AObstacle : public AActor
{
	GENERATED_BODY()

protected:
	// ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

	// オーバーラップに設定するコリジョンコンポーネントを格納するための配列
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPrimitiveComponent* _CollisionComponentsToOverlap;

	// オーバーラップイベント
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// コンストラクタ
	AObstacle();
	// 毎フレーム処理
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Hp;			// 体力
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Score;		// 点数

private:

};
