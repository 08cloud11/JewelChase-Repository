//--------------------------------------------------------------------
// ファイル名 ：BlackOut.h
// 概要       ：暗幕のUI
// 作成者     ：0231本間
// 更新内容   ：02/24　作成、
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "BlackOut.generated.h"

UCLASS()
class PROTOTYPE_API UBlackOut : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetOpenLevelName(FName name) { _lvName = name; }	//どのレベルに遷移するか指定

private:
	FName _lvName;	//次に遷移するレベル

	UPROPERTY()
	TObjectPtr<UImage> _blackoutImage;	//暗幕の画像

	float _time = 0.0f;	//経過時間
};
