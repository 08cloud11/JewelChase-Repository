//--------------------------------------------------------------------
// ファイル名 ：InGame.cpp
// 概要       ：インゲーム内のUIをまとめる
// 作成者     ：0231本間
// 更新内容   ：02/24　作成、
//--------------------------------------------------------------------

#include "InGame.h"
#include "Kismet/GameplayStatics.h"

void UInGame::NativeConstruct()
{
	Super::NativeConstruct();

	//プレイヤーを参照し、代入
	TSubclassOf<APlayerCharacter> findClass;
	findClass = APlayerCharacter::StaticClass();
	_player = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), findClass));

	init();
}

void UInGame::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//ゲームオーバー
	if (_bGameOver)
	{
		_gameoverTime += InDeltaTime;

		//一定時間経過+ゲームオーバーのUIを生成できる
		if (_gameoverTime > 2.0f && _createGameOverUI)
		{
			UUserWidget* gameover = CreateWidget<UUserWidget>(GetWorld(), _ingameUIs["GameOver"]);
			gameover->AddToViewport();

			_createGameOverUI = false;
		}
	}
	//プレイ中
	else
	{
		UpdateHP();

		UpdateBullet();

		//残弾数が0
		if (_player->GetLimitBulletNum() <= 0)
		{
			UUserWidget* reload = CreateWidget<UUserWidget>(GetWorld(), _ingameUIs["Reload"]);
			_reloadPanel->AddChildToCanvas(reload);
		}
	}
}

// 関数名：init
// 引　数：なし
// 戻り値：void
// 処理内容：初期化処理
void UInGame::init()
{
	//---------HPのパネルを取得、初期値分UIを生成
	_hpPanel = Cast<UWrapBox>(GetWidgetFromName("WrapBox_HP"));

	for (int i = 0; i < _player->GetHP(); i++)
	{
		UUserWidget* hp = CreateWidget<UUserWidget>(GetWorld(), _ingameUIs["HP"]);
		_hpPanel->AddChildToWrapBox(hp);
	}

	_beforeHP = _player->GetHP();
	//------------------------------------------

	//---------残弾数のパネルを取得、初期値分UIを生成
	_bulletPanel = Cast<UWrapBox>(GetWidgetFromName("WrapBox_Bullet"));

	for (int i = 0; i < _player->GetLimitBulletNum(); i++)
	{
		UUserWidget* bullet = CreateWidget<UUserWidget>(GetWorld(), _ingameUIs["Bullet"]);
		_bulletPanel->AddChildToWrapBox(bullet);
	}

	_beforebulletNum = _player->GetLimitBulletNum();
	//------------------------------------------

	//ジュエルゲージUIを生成
	UUserWidget* jewelgauge = CreateWidget<UUserWidget>(GetWorld(), _ingameUIs["Jewel"]);
	Cast<UCanvasPanel>(GetWidgetFromName("Jewel_Panel"))->AddChildToCanvas(jewelgauge);

	//ゴールまでの距離のUIを生成
	UUserWidget* distanceGoal = CreateWidget<UUserWidget>(GetWorld(), _ingameUIs["Distance"]);
	Cast<UCanvasPanel>(GetWidgetFromName("Distance_Panel"))->AddChildToCanvas(distanceGoal);

	//スコアUIを生成
	UUserWidget* score = CreateWidget<UUserWidget>(GetWorld(), _ingameUIs["Score"]);
	Cast<UCanvasPanel>(GetWidgetFromName("Score_Panel"))->AddChildToCanvas(score);

	//リロードのパネルを取得
	_reloadPanel = Cast<UCanvasPanel>(GetWidgetFromName("Reload_Panel"));
}

// 関数名：UpdateHP
// 引　数：なし
// 戻り値：void
// 処理内容：HPUIを更新
void UInGame::UpdateHP()
{
	//HPに変動がある
	if (_beforeHP != _player->GetHP())
	{
		//前回より低い
		if (_beforeHP > _player->GetHP())
		{
			//減らす
			_hpPanel->RemoveChildAt(_player->GetHP());

			_beforeHP = _player->GetHP();

			//HPが0
			if (_beforeHP <= 0)
			{
				_bGameOver = true;

				//プレイヤーを鈍化させる
				_player->CustomTimeDilation = 0.6f;
			}
		}
		//前回より高い
		else 
		{
			//HPUI追加
			UUserWidget* hp = CreateWidget<UUserWidget>(GetWorld(), _ingameUIs["HP"]);
			_hpPanel->AddChildToWrapBox(hp);

			_beforeHP = _player->GetHP();
		}
	}
}

// 関数名：UpdateBullet
// 引　数：なし
// 戻り値：void
// 処理内容：残弾数UIを更新
void UInGame::UpdateBullet()
{
	//残弾数に変動がある
	if (_beforebulletNum != _player->GetLimitBulletNum())
	{
		//前回より低い
		if (_beforebulletNum > _player->GetLimitBulletNum())
		{
			//減らす
			_bulletPanel->RemoveChildAt(_player->GetLimitBulletNum());

			_beforebulletNum = _player->GetLimitBulletNum();
		}
		//前回より高い
		else
		{
			//差分を増やす
			for (int i = _beforebulletNum; i < _player->GetLimitBulletNum(); i++)
			{
				UUserWidget* bullet = CreateWidget<UUserWidget>(GetWorld(), _ingameUIs["Bullet"]);
				_bulletPanel->AddChildToWrapBox(bullet);
			}

			_beforebulletNum = _player->GetLimitBulletNum();
		}
	}
}