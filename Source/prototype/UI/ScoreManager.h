//--------------------------------------------------------------------
// ファイル名 ：ScoreManager.h
// 概要       ：UIでスコアを表示する際の補助を行う
// 作成者     ：0231本間
// 更新内容   ：01/10　作成、
//--------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreManager.generated.h"

UCLASS()
class PROTOTYPE_API AScoreManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScoreManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	int32 Get_score();

	UFUNCTION(BlueprintCallable)
	void AddScore(int32 addScore);

	UFUNCTION(BlueprintCallable)
	int32 Get_beforescore();

	UFUNCTION(BlueprintCallable)
	int32 Get_JewelNum();

	UFUNCTION(BlueprintCallable)
	void AddJewelNum();

private:
	int32 _score;

	int32 beforegetscore;
	int32 _jewelNum;

	FString path = "/Game/0231_honma/UI/InGame/UI_GetScore.UI_GetScore_C";

	TSubclassOf<UUserWidget> m_ui_getscore;

	UPROPERTY()
	UUserWidget* m_pui_getscore = nullptr;
};
