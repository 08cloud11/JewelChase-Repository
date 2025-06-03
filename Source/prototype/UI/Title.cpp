//--------------------------------------------------------------------
// ファイル名 ：Title.cpp
// 概要       ：タイトル画面でのメインのUI
// 作成者     ：0231本間
// 更新内容   ：02/24　作成、
//--------------------------------------------------------------------


#include "Title.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UTitle::NativeConstruct()
{
	Super::NativeConstruct();

	init();

	//タイトルコール、BGMを鳴らす
	if (_seMap["TitleCall"] != nullptr)_seManager->PlaySE2D("TitleCall");
	if (TitleBGM != nullptr)PlaySound(TitleBGM);
}

void UTitle::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	//開始時の暗幕があるなら処理を行う
	if (IsValid(_beginblackoutImage)) {
		BeginTick(InDeltaTime);
	}

	TitleLogoScaleUpDown(InDeltaTime);

	_time += InDeltaTime;

	if (_time > 47.0f && !_createDemo)
	{
		//デモプレイムービーUIを生成
		pm_ui_demoplay = CreateWidget<UUserWidget>(GetWorld(), m_ui_demoplay);
		if (pm_ui_demoplay){
			pm_ui_demoplay->AddToViewport();
		}

		//タイトル画面を開きなおすUI(ボタン)を生成
		pm_ui_openTitle = CreateWidget<UUserWidget>(GetWorld(), m_ui_openTitle);
		if (pm_ui_openTitle){
			pm_ui_openTitle->AddToViewport();
		}

		_createDemo = true;
	}
}

// 関数名：init
// 引　数：なし
// 戻り値：void
// 処理内容：初期化処理
void UTitle::init()
{
	//SEManagerを取得し、SEを登録
	_seManager = USEManager::GetInstance(GetWorld());
	if(_seMap["select"] != nullptr) _seManager->RegisterSE("select", _seMap["select"]);
	if (_seMap["start"] != nullptr) _seManager->RegisterSE("start", _seMap["start"]);
	if (_seMap["TitleCall"] != nullptr) _seManager->RegisterSE("TitleCall", _seMap["TitleCall"]);

	//スタートボタンにバインド
	_startButton = Cast<UButton>(GetWidgetFromName("Start"));
	if (_startButton) {
		if (!_startButton->OnPressed.IsBound()) {
			_startButton->OnPressed.AddDynamic(this, &UTitle::StartPressed);
		}
	}

	//エンドボタンにバインド
	_endButton = Cast<UButton>(GetWidgetFromName("GameEnd"));
	if (_endButton) {
		if (!_endButton->OnPressed.IsBound()) {
			_endButton->OnPressed.AddDynamic(this, &UTitle::EndPressed);
		}
	}

	//オプションボタンにバインド
	_optionButton = Cast<UButton>(GetWidgetFromName("Option"));
	if (_optionButton) {
		if (!_optionButton->OnPressed.IsBound()) {
			_optionButton->OnPressed.AddDynamic(this, &UTitle::OptionPressed);
		}
	}

	//開始時の暗幕を取得、透明度は最大
	_beginblackoutImage = Cast<UImage>(GetWidgetFromName("BlackOut"));
	_beginblackoutImage->SetRenderOpacity(1.0f);

	//タイトルロゴを取得
	_titleLogo = Cast<UImage>(GetWidgetFromName("TitleLogo"));
	_titleLogo->SetRenderOpacity(1.0f);
}

// 関数名：BeginTick
// 引　数：float InDeltaTime
// 戻り値：void
// 処理内容：開始時数秒の処理
void UTitle::BeginTick(float InDeltaTime)
{
	//前半の処理
	if (!_beginAfter)
	{
		_beginblackoutOpacity += InDeltaTime * 2.0f;

		//0.5秒経過したら
		if (_beginblackoutOpacity >= 1.0f)
		{
			_beginAfter = true;
		}
	}
	//後半の処理
	else
	{		
		//開始時の暗幕の透明度を徐々に下げる
		_beginblackoutOpacity -= InDeltaTime * 2.0f;
		_beginblackoutImage->SetRenderOpacity(_beginblackoutOpacity);

		//透明になったら
		if (_beginblackoutOpacity < 0.0f)
		{
			//暗幕を消す
			_beginblackoutImage->RemoveFromParent();
		}
	}
}

// 関数名：TitleLogoScaleUpDown
// 引　数：float InDeltaTime
// 戻り値：void
// 処理内容：タイトルロゴの拡縮
void UTitle::TitleLogoScaleUpDown(float InDeltaTime)
{
	//拡大
	if (_logoScaleUpDown)
	{
		_logoRenderScale += InDeltaTime / 10.0f;

		if (_logoRenderScale > 1.0f)
		{
			//次のフレームからロゴを縮小させる
			_logoScaleUpDown = false;
		}
		else
		{
			//ロゴのサイズを適用
			_titleLogo->SetRenderScale(FVector2D(_logoRenderScale, _logoRenderScale));
		}
	}
	//縮小
	else
	{
		_logoRenderScale -= InDeltaTime / 10.0f;

		if (_logoRenderScale < 0.85f)
		{
			//次のフレームからロゴを拡大させる
			_logoScaleUpDown = true;
		}
		else
		{
			//ロゴのサイズを適用
			_titleLogo->SetRenderScale(FVector2D(_logoRenderScale, _logoRenderScale));
		}
	}
		
}

// 関数名：StartPressed
// 引　数：なし
// 戻り値：void
// 処理内容：ステージセレクトに遷移するボタン
void UTitle::StartPressed()
{
	//暗幕のUIを生成し、ステージセレクトへ遷移
	pm_ui_blackout = CreateWidget<UBlackOut>(GetWorld(), m_ui_blackout);
	if (pm_ui_blackout){
		pm_ui_blackout->AddToViewport();
		pm_ui_blackout->SetOpenLevelName("Master_StageSelect");
	}

	//スタート音を鳴らす
	if (_seMap["start"] != nullptr) _seManager->PlaySE2D("start");
}

// 関数名：EndPressed
// 引　数：なし
// 戻り値：void
// 処理内容：ゲームを強制終了させるボタン
void UTitle::EndPressed()
{
	UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this,0),
		EQuitPreference::Quit, false);
}

// 関数名：OptionPressed
// 引　数：なし
// 戻り値：void
// 処理内容：ゲームを強制終了させるボタン
void UTitle::OptionPressed()
{
	//オプションのUIを生成
	pm_ui_OptionTitle = CreateWidget<UUserWidget>(GetWorld(), m_ui_OptionTitle);
	if (pm_ui_OptionTitle){
		pm_ui_OptionTitle->AddToViewport();
	}

	//セレクト音を鳴らす
	if (_seMap["select"] != nullptr) _seManager->PlaySE2D("select");
}