//--------------------------------------------------------------------
// ファイル名：LineControllerComponent.h
// 概要　　　：線描画アクターを使うためのコンポーネントクラス
// 作成者　　：山内　将吾
// 更新内容　：2024/12/15	クラスの作成
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LineControllerComponent.generated.h"

// 前方宣言
class APredictionLine;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROTOTYPE_API ULineControllerComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:
	// コンストラクタ
	ULineControllerComponent();
	// ゲーム終了時、または破棄時に呼ばれる処理
	void EndPlay(const EEndPlayReason::Type EndPlayReason);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line")
	TSubclassOf<AActor> LineActorsClass;	// 予測線のアクタークラス 
	UPROPERTY()
	APredictionLine* _pPredictionLineActor;	// 生成する線を描画するアクター

	UFUNCTION(BlueprintCallable, Category = "Line")
	void SpawnAndInitializeLineActor(); 	// 線描画アクターをスポーンして初期化
	UFUNCTION(BlueprintCallable, Category = "Line")
	void UpdateLineActor(FVector& Start, FVector& End);		// 線を更新する
	UFUNCTION()
	void DestroyPredictionLineActor();	// 線のアクターを消去する関数
	UFUNCTION()
	void HidePredictionLineActor();	// 予測線を非表示にする関数
	UFUNCTION()
	void DisplayPredictionLineActor();	// 予測線を表示にする関数

private:
	UPROPERTY(EditAnywhere)
	FVector _lineOffset;	// 予測線の位置のオフセット
	UPROPERTY(EditAnywhere)
	bool _predictionLineFlag;	// 予測線を出すかどうかのフラグ
};
