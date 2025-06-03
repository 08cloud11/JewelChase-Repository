//--------------------------------------------------------------------
// ファイル名 ：Title.h
// 概要       ：タイトル画面でのメインのUI
// 作成者     ：0231本間
// 更新内容   ：02/24　作成、
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "prototype/SEManager.h"
#include "BlackOut.h"
#include "Title.generated.h"

UCLASS()
class PROTOTYPE_API UTitle : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void init();

	void BeginTick(float InDeltaTime);

	void TitleLogoScaleUpDown(float InDeltaTime);

	UFUNCTION(BlueprintCallable)
	void StartPressed();
	UFUNCTION(BlueprintCallable)
	void EndPressed();
	UFUNCTION(BlueprintCallable)
	void OptionPressed();

private:
	UPROPERTY()
	TObjectPtr<USEManager> _seManager = nullptr;	//SEManager

	UPROPERTY(EditAnywhere, Category = "Sound")
	TMap<FName, TObjectPtr<USoundBase>> _seMap;		//SE

	UPROPERTY(EditAnywhere, Category = "BGM")
	TObjectPtr<USoundBase> TitleBGM;		//BGM

	UPROPERTY()
	TObjectPtr<UButton> _startButton;	//スタートボタン

	UPROPERTY()
	TObjectPtr<UButton> _endButton;		//エンドボタン

	UPROPERTY()
	TObjectPtr<UButton> _optionButton;	//オプションボタン

	UPROPERTY()
	TObjectPtr<UImage> _beginblackoutImage;	//開始時の暗幕

	UPROPERTY()
	TObjectPtr<UImage> _titleLogo;	//タイトルロゴ

	//--------オプションUI
	UPROPERTY(EditAnywhere, Category = "UI_Option")
	TSubclassOf<UUserWidget> m_ui_OptionTitle;

	UPROPERTY()
	UUserWidget* pm_ui_OptionTitle = nullptr;
	//---------------------------------

	//--------暗幕UI
	UPROPERTY(EditAnywhere, Category = "UI_BlackOut")
	TSubclassOf<UBlackOut> m_ui_blackout;

	UPROPERTY()
	UBlackOut* pm_ui_blackout = nullptr;
	//---------------------------------

	//--------デモプレイムービーUI
	UPROPERTY(EditAnywhere, Category = "UI_DemoPlay")
	TSubclassOf<UUserWidget> m_ui_demoplay;

	UPROPERTY()
	UUserWidget* pm_ui_demoplay = nullptr;
	//---------------------------------

	//--------タイトル画面を開きなおすUI(ボタン)
	UPROPERTY(EditAnywhere, Category = "UI_OpenTitle")
	TSubclassOf<UUserWidget> m_ui_openTitle;

	UPROPERTY()
	UUserWidget* pm_ui_openTitle = nullptr;
	//---------------------------------

	float _beginblackoutOpacity = 0.0f;	//開始時の暗幕の透明度

	float _logoRenderScale = 0.85f; //ロゴのサイズ

	float _time = 0.0f;	//時間

	bool _beginAfter = false;	//開始から数秒の処理のための変数

	bool _logoScaleUpDown = true;	//trueで拡大、falseで縮小

	bool _createDemo = false;	//デモプレイムービーUIを生成したか
};
