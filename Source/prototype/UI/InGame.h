//--------------------------------------------------------------------
// ファイル名 ：InGame.h
// 概要       ：インゲーム内のUIをまとめる
// 作成者     ：0231本間
// 更新内容   ：02/24　作成、
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "prototype/Player/PlayerCharacter.h"
#include "Components/WrapBox.h"
#include "InGame.generated.h"

UCLASS()
class PROTOTYPE_API UInGame : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
private:
	void init();

	void UpdateHP();

	void UpdateBullet();

private:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> _player = nullptr;		//プレイヤー

	UPROPERTY(EditAnywhere, Category = "InGameUI")
	TMap<FName, TSubclassOf<UUserWidget>> _ingameUIs;	//インゲーム内のUIたち

	UPROPERTY()
	TObjectPtr<UWrapBox> _hpPanel;			//HPのパネル

	UPROPERTY()
	TObjectPtr<UWrapBox> _bulletPanel;		//残弾数のパネル

	UPROPERTY()
	TObjectPtr<UCanvasPanel> _reloadPanel;	//リロードのパネル

	float _gameoverTime = 0.0f;		//ゲームオーバーになってからの経過時間

	int _beforeHP;					//前回のHP

	int _beforebulletNum;			//前回の残弾数

	bool _bGameOver = false;		//ゲームオーバーになったか

	bool _createGameOverUI = true;	//ゲームオーバーUIを生成できるか
};
