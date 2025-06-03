//--------------------------------------------------------------------
// ファイル名 ：StageData.h
// 概要       ：ステージ（障害物や敵）の生成を制御する構造体
// 作成者     ：0231本間
// 更新内容   ：11/25　作成
//--------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StageData.generated.h"

/**
 *
 */
UCLASS()
class PROTOTYPE_API UStageData : public UDataTable
{
	GENERATED_BODY()

};

UENUM(BlueprintType) 
enum class heliStatus : uint8
{	
	None UMETA(DisplayName = "None"),
	Right UMETA(DisplayName = "Right"),
	Center UMETA(DisplayName = "Center"),
	Left UMETA(DisplayName = "Left")
};

USTRUCT()
struct PROTOTYPE_API FMapStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	//オブジェクトの種類
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameData", DisplayName = "Type", meta = (IgnoreForMemberInitializationTest))
	int32 ObjecType;

	//Xレーン
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameData", DisplayName = "Xlane", meta = (IgnoreForMemberInitializationTest))
	int32 xlane;

	//Yレーン
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameData", DisplayName = "Ylane", meta = (IgnoreForMemberInitializationTest))
	int32 ylane;

	//Zレーン
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameData", DisplayName = "Zlane", meta = (IgnoreForMemberInitializationTest))
	int32 zlane;

	//ヘリコプター管理用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameData", DisplayName = "HeliStatus", meta = (IgnoreForMemberInitializationTest))
	heliStatus HeliStatus;
};