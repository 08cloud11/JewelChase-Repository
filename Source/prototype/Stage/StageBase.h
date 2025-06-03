//--------------------------------------------------------------------
// ファイル名 ：StageBase.h
// 概要       ：ステージ（障害物や敵）の生成を制御するクラス
// 作成者     ：0231本間
// 更新内容   ：11/25　作成、プロパティとメソッドを作成
//--------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StageBase.generated.h"

UCLASS()
class PROTOTYPE_API AStageBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStageBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	void SpawnObject();

	void CommonSpawn(float xPos);

	void SpawnEnemy();

	void SpawnNextStage();

	void Move();

	void DeleteStage();

	AActor* SearchStageMng();

private:
	//次のステージをスポーンさせたかの確認用
	bool _bspawnstage;

	//マップ移動速度
	float _movespeed;

	int8 _stagelane; //ステージマネージャから取得

	TSoftObjectPtr<UDataTable> StageDataTable;

	UPROPERTY()
	AActor* pStageMng;

	AActor* pPlayer;

	//BPポインタ
	UPROPERTY(EditAnywhere, Category = "Common")//ステージ共通オブジェクト　ゴール＝99
		TSubclassOf<class AActor> _commons[6];
	UPROPERTY(EditAnywhere, Category = "obstacles")//障害物　1～10
		TSubclassOf<class AActor> _obstacles[10];
	UPROPERTY(EditAnywhere, Category = "Jewel")//ジュエル　11～20
		TSubclassOf<class AActor>  _jewels[5];
	UPROPERTY(EditAnywhere, Category = "Item")//アイテム　21～30
		TSubclassOf<class AActor>  _Items[7];
	UPROPERTY(EditAnywhere, Category = "enemy")//エネミー　31～40
		TSubclassOf<class AActor> _enemys[3];
};
