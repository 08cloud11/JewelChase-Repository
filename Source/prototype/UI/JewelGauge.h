//--------------------------------------------------------------------
// ファイル名 ：JewelGauge.h
// 概要       ：ジュエルゲージのUI
// 作成者     ：0231本間
// 更新内容   ：02/24　作成、
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/Image.h"
#include "prototype/SEManager.h"
#include "prototype/Player/PlayerCharacter.h"
#include "Engine/TimerHandle.h"
#include "JewelGauge.generated.h"

UCLASS()
class PROTOTYPE_API UJewelGauge : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativePreConstruct() override;

	void NativeConstruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void IncreaseGauge(float InDeltaTime);

	void DecreaseGauge(float InDeltaTime);

	void GaugeUpAnim();

	void MaxGaugeAnim(float InDeltaTime);

	void AnimReset();

private:
	UPROPERTY()
	TObjectPtr<AActor> _player = nullptr;		//プレイヤー

	UPROPERTY()
	TObjectPtr<USEManager> _seManager = nullptr;	//SEManager

	UPROPERTY(EditAnywhere, Category = "canJewelSE")
	TObjectPtr<USoundBase> _canJewelSE;	//溜まった時のSE

	UPROPERTY()
	TMap<FName, TObjectPtr<UImage>> _gaugeAnimImages;	//溜まった時のアニメーション画像

	UPROPERTY()
	UMaterialInstanceDynamic* _jewelGaugeMat;	//ゲージのマテリアル

	UPROPERTY()
	FTimerHandle _timer;	//関数の再帰処理をディレイをかけて行うのに必要なハンドル

	float _maxVal;	//ゲージの最大値

	float _beforeVal = 0.0f;	//前回変更があった時のゲージの値

	float _currentVal = 0.0f;	//現在のアニメーション中のゲージ量

	float _rushTime = 10.0f;	//ジュエルラッシュ持続時間

	float _decreaseTime = 0.0f;	//ジュエルラッシュ中、ゲージを減少させる変数

	float _animTime = 0.0f;		//ゲージが溜まってる時の一画像の描画タイム

	int _animIndex = 0;			//ゲージが溜まってる時の現在の表示画像番号

	bool _backIndex = false;	//ゲージが溜まってる時の描画番号を増加、減少させるか

	bool _canPlaySE = true;		//ゲージが溜まった時のSEを一度だけ鳴らすための変数
};
