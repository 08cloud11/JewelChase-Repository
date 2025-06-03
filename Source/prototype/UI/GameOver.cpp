//--------------------------------------------------------------------
// ファイル名 ：GameOver.cpp
// 概要       ：ゲームオーバーのUI
// 作成者     ：0231本間
// 更新内容   ：02/24　作成、
//--------------------------------------------------------------------

#include "GameOver.h"
#include "Kismet/GameplayStatics.h"

void UGameOver::NativeConstruct()
{
	Super::NativeConstruct();

	//ゲームを一時停止
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	//入力受付をUIからのみにする
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeUIOnly());

	//SEManagerを取得し、セレクト音を登録
	_seManager = USEManager::GetInstance(GetWorld());
	_seManager->RegisterSE("select", _selectSE);

	//リトライボタンにバインド
	_retryButton = Cast<UButton>(GetWidgetFromName("Retry"));
	if (_retryButton) {
		if (!_retryButton->OnPressed.IsBound()) {
			_retryButton->OnPressed.AddDynamic(this, &UGameOver::RetryPressed);
		}
	}

	//ステージセレクトボタンにバインド
	_quitButton = Cast<UButton>(GetWidgetFromName("Quit"));
	if (_quitButton) {
		if (!_quitButton->OnPressed.IsBound()) {
			_quitButton->OnPressed.AddDynamic(this, &UGameOver::QuitPressed);
		}
	}
}
	
// 関数名：RetryPressed
// 引　数：なし
// 戻り値：void
// 処理内容：同じステージをリトライするボタン
void UGameOver::RetryPressed()
{
	//現在のステージ名を取得し、開きなおす
	FString Lvname = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameplayStatics::OpenLevel(GetWorld(), FName(Lvname));
}
	
// 関数名：QuitPressed
// 引　数：なし
// 戻り値：void
// 処理内容：ステージセレクトに戻るボタン
void UGameOver::QuitPressed()
{
	//暗幕のUIを生成し、ステージセレクトへ遷移
	pm_ui_QuitPause = CreateWidget<UBlackOut>(GetWorld(), m_ui_QuitPause);
	if (pm_ui_QuitPause){
		pm_ui_QuitPause->AddToViewport();
		pm_ui_QuitPause->SetOpenLevelName("Master_StageSelect");
	}

	//セレクト音を鳴らす
	_seManager->PlaySE2D("select");
}