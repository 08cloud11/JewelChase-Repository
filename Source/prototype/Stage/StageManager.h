//--------------------------------------------------------------------
// ファイル名 ：StageManager.h
// 概要       ：ステージ生成の為の番号、csvパスを管理するクラス
// 作成者     ：0231本間
// 更新内容   ：11/29　作成、
//--------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StageData.h"
#include "StageManager.generated.h"

UCLASS()
class PROTOTYPE_API AStageManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStageManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void UpdateFloating();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TSoftObjectPtr<UDataTable> Get_csvpath();
	TSoftObjectPtr<UDataTable> Get_goalpath();

	UFUNCTION(BlueprintCallable)
	int32 Get_stagenum();
	UFUNCTION(BlueprintCallable)
	int32 Get_maxstagenum();

	int8 Get_stagelane();
	UFUNCTION(BlueprintCallable)
	float Get_movespeed();
	bool Get_bgoalspawn();
	
	void SetItems(TArray<AActor*> Others);

	UFUNCTION(BlueprintCallable)
	void  _bMagnetOn();

	void approachItem(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void _bStartON() { _bStart = true; }

	UFUNCTION(BlueprintCallable)
	bool get_bStart()const { return _bStart; }

	UFUNCTION(BlueprintCallable)
	void _bGoalON()	{_bGoal = true;	}

	UFUNCTION(BlueprintCallable)
	bool get_bGoal()const{return _bGoal;	}

	// 妥協処理
	void SetHeliStatus(int other) {

		_BeginHeliState = other;
		UE_LOG(LogTemp, Log, TEXT("set_BeginHeliState=[%d]"), _BeginHeliState);
	}

	int GetHeliStatus() {
		UE_LOG(LogTemp, Log, TEXT("get_BeginHeliState=[%d]"), _BeginHeliState);
		return _BeginHeliState;
	}

private:
	UPROPERTY(EditAnywhere, meta = (EditCondition = "Data"))
	int32 _maxstagenum;					    //最大ステージ番号
	UPROPERTY(EditAnywhere, meta = (EditCondition = "Data"))
	float _movespeed;					    //ステージ全体の移動速度
	UPROPERTY(EditAnywhere, meta = (EditCondition = "Data"))
	int8 _stagelane;					    //ステージ全体の横のレーン幅
	UPROPERTY(EditAnywhere, meta = (EditCondition = "Data"))
	TSoftObjectPtr<UDataTable> _csvpath[100];//次に生成するステージのcsvパス
	UPROPERTY(EditAnywhere, meta = (EditCondition = "Data"))
	TSoftObjectPtr<UDataTable> _goalpath;//ゴールステージのcsvパス
	UPROPERTY(EditAnywhere, meta = (EditCondition = "Data"))
	float zmoveLength;//アイテムが上下に浮遊する速度
	UPROPERTY(EditAnywhere, meta = (EditCondition = "Data"))
	float magnetmoveLength;//アイテムが上下に浮遊する速度

	UPROPERTY()
	TArray<AActor*> Items;//ステージ上のアイテムを取得
	float toDefaultAddZpos;//浮遊中のポジションを設定
	bool bItemsFloating;//上がるか下がるか

	bool bMagnet;

	float magnetTime;

	bool _bgoalspawn;//すでにゴールをスポーンしたか

	int32 _stagenum;//ステージ番号

	int _BeginHeliState;	// 妥協処理

	bool _bStart;

	bool _bGoal;
};
