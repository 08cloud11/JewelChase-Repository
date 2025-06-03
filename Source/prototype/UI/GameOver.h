//--------------------------------------------------------------------
// ファイル名 ：GameOver.h
// 概要       ：ゲームオーバーのUI
// 作成者     ：0231本間
// 更新内容   ：02/24　作成、
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "prototype/SEManager.h"
#include "BlackOut.h"
#include "GameOver.generated.h"

UCLASS()
class PROTOTYPE_API UGameOver : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

private:
	UFUNCTION(BlueprintCallable)
	void RetryPressed();
	UFUNCTION(BlueprintCallable)
	void QuitPressed();
	
private:
	UPROPERTY()
	TObjectPtr<USEManager> _seManager = nullptr;	//SEManager

	UPROPERTY(EditAnywhere, Category = "SelectSE")
	TObjectPtr<USoundBase> _selectSE;		//セレクトのSE

	UPROPERTY()
	TObjectPtr<UButton> _retryButton;	//リトライボタン

	UPROPERTY()
	TObjectPtr<UButton> _quitButton;	//ステージセレクトボタン

	//--------暗幕UI
	UPROPERTY(EditAnywhere, Category = "UI_BlackOut")
	TSubclassOf<UBlackOut> m_ui_QuitPause;

	UPROPERTY()
	UBlackOut* pm_ui_QuitPause = nullptr;
	//---------------------------------
};
