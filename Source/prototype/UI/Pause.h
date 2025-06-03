//--------------------------------------------------------------------
// ファイル名 ：Pause.h
// 概要       ：インゲーム内のポーズ画面
// 作成者     ：0231本間
// 更新内容   ：02/24　作成、
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "prototype/SEManager.h"
#include "prototype/Player/PlayerCharacter.h"
#include "Components/Button.h"
#include "BlackOut.h"
#include "Pause.generated.h"

UCLASS()
class PROTOTYPE_API UPause : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

private:
	void init();

	UFUNCTION(BlueprintCallable)
	void BackPressed();
	UFUNCTION(BlueprintCallable)
	void RetryPressed();
	UFUNCTION(BlueprintCallable)
	void QuitPressed();

private:

	UPROPERTY()
	TObjectPtr<AActor> _player = nullptr;		//プレイヤー

	UPROPERTY()
	TObjectPtr<USEManager> _seManager = nullptr;	//SEManager

	UPROPERTY(EditAnywhere, Category = "SelectSE")
	TObjectPtr<USoundBase> _selectSE;		//セレクトのSE

	UPROPERTY()
	PlayerStatus _beginPlayerStatus;	//ポーズに入った時の状態を補管

	UPROPERTY()
	TObjectPtr<UButton> _backButton;	//再開ボタン

	UPROPERTY()
	TObjectPtr<UButton> _retryButton;	//リトライボタン

	UPROPERTY()
	TObjectPtr<UButton> _quitButton;	//ステージセレクトボタン

	//--------再開時のUI
	UPROPERTY(EditAnywhere, Category = "UI_QuitPause")
	TSubclassOf<UUserWidget> m_ui_BackPause;

	UPROPERTY()
	UUserWidget* pm_ui_BackPause = nullptr;
	//---------------------------------

	//--------暗幕UI
	UPROPERTY(EditAnywhere, Category = "UI_BlackOut")
	TSubclassOf<UBlackOut> m_ui_QuitPause;

	UPROPERTY()
	UBlackOut* pm_ui_QuitPause = nullptr;
	//---------------------------------
};