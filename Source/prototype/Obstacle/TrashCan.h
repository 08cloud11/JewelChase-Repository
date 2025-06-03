//--------------------------------------------------------------------
// ファイル名：TrashCan.h
// 概要　　　：ゴミ箱クラス
// 作成者　　：山内　将吾
// 更新内容　：2024/02/12 クラスの作成
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "prototype/Obstacle/Obstacle.h"
#include "TrashCan.generated.h"

// 前方宣言
class UBoxComponent;

// ドロップさせるアイテムをenum型で制御する
UENUM()
enum class DropItems : uint8
{
	Jewel_Blue,		// 青ジュエル
	Jewel_Red,		// 赤ジュエル
	Jewel_Yellow,	// 黄ジュエル
	Jewel_Purple,	// 紫ジュエル

	HpItem,			// 体力回復
	Magnet,			// マグネット
};

UCLASS()
class PROTOTYPE_API ATrashCan : public AObstacle
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
	ATrashCan();

	UPROPERTY(EditAnywhere, Category = "Drop Items")
	TSubclassOf<class AActor> _pDropItems[6];		// ドロップするアイテムの BP を指定する

	UPROPERTY()
	DropItems DropItemValue;		// ドロップアイテムを管理する Enmu 型変数

	UFUNCTION(BlueprintCallable)
	void SpawnDropItem();		// アイテムをドロップさせる関数


private:

	UPROPERTY(EditAnywhere)
	class UBoxComponent* _pBoxComponent;	 // ボックスコンポーネント
};
