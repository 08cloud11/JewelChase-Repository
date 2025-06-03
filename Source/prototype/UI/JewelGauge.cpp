//--------------------------------------------------------------------
// ファイル名 ：JewelGauge.cpp
// 概要       ：ジュエルゲージのUI
// 作成者     ：0231本間
// 更新内容   ：02/24　作成、
//--------------------------------------------------------------------

#include "JewelGauge.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UJewelGauge::NativePreConstruct()
{
	Super::NativePreConstruct();

	//アニメーション用画像を取得、管理
	_gaugeAnimImages.Add("0", Cast<UImage>(GetWidgetFromName("Image_0")));
	_gaugeAnimImages.Add("1", Cast<UImage>(GetWidgetFromName("Image_1")));
	_gaugeAnimImages.Add("2", Cast<UImage>(GetWidgetFromName("Image_2")));
	_gaugeAnimImages.Add("3", Cast<UImage>(GetWidgetFromName("Image_3")));
	_gaugeAnimImages.Add("4", Cast<UImage>(GetWidgetFromName("Image_4")));
	_gaugeAnimImages.Add("5", Cast<UImage>(GetWidgetFromName("Image_5")));
	_gaugeAnimImages.Add("6", Cast<UImage>(GetWidgetFromName("Image_6")));

	//ゲージのマテリアルを取得、管理
	_jewelGaugeMat = Cast<UImage>(GetWidgetFromName("Image_front"))->GetDynamicMaterial();
}

void UJewelGauge::NativeConstruct()
{
	Super::NativeConstruct();

	//プレイヤーを参照し、代入
	TSubclassOf<APlayerCharacter> findClass;
	findClass = APlayerCharacter::StaticClass();
	_player = UGameplayStatics::GetActorOfClass(GetWorld(), findClass);

	_maxVal = float(Cast<APlayerCharacter>(_player)->GetJewelRushgageMaxNum());

	//SEManagerを取得し、溜まった時の音を登録
	_seManager = USEManager::GetInstance(GetWorld());
	_seManager->RegisterSE("canJewel", _canJewelSE);
}

void UJewelGauge::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//ジュエルラッシュ中
	if (Cast<APlayerCharacter>(_player)->GetisJewelRush())
	{
		DecreaseGauge(InDeltaTime);

		//アニメーション用変数をリセット
		if (_animIndex != 0 || _backIndex || _animTime != 0.0f)
		{
			AnimReset();
		}
	}
	//ジュエルラッシュ中じゃない
	else
	{
		//前回のゲージ量と変更があった時
		if (float(Cast<APlayerCharacter>(_player)->GetJewelRushgageNum()) != _beforeVal)
		{
			IncreaseGauge(InDeltaTime);
		}
	}
}

// 関数名：IncreaseGauge
// 引　数：float InDeltaTime
// 戻り値：void
// 処理内容：ゲージ増加処理
void UJewelGauge::IncreaseGauge(float InDeltaTime)
{
	_beforeVal = (float(Cast<APlayerCharacter>(_player)->GetJewelRushgageNum()) * 2.0f) / _maxVal;

	GaugeUpAnim();

	//ゲージが溜まっていたら
	if (_beforeVal >= 2.0f)
	{
		_decreaseTime = 0.0f;
		MaxGaugeAnim(InDeltaTime);

		if (_canPlaySE)
		{
			//溜まった時のSEを一度だけ鳴らす
			_seManager->PlaySE2D("canJewel");
			_canPlaySE = false;
		}
	}
	//溜まってないとき
	else
	{
		_canPlaySE = true;
	}
}

// 関数名：IncreaseGauge
// 引　数：float InDeltaTime
// 戻り値：void
// 処理内容：ゲージ減少処理
void UJewelGauge::DecreaseGauge(float InDeltaTime)
{
	_decreaseTime += InDeltaTime;

	//ラッシュタイムと現在の経過時間を引いた値をゲージに適用
	float gaugeval = ((_rushTime - _decreaseTime) / _rushTime) * 2.0f;
	_jewelGaugeMat->SetScalarParameterValue("Value", gaugeval);
}

// 関数名：GaugeUpAnim
// 引　数：なし
// 戻り値：void
// 処理内容：ゲージの徐々に増える処理
void UJewelGauge::GaugeUpAnim()
{
	_currentVal = ((_beforeVal - _currentVal) * 0.25f) + _currentVal;

	//差が0.000001以内なら
	if (UKismetMathLibrary::NearlyEqual_FloatFloat(_currentVal, _beforeVal, 1.0E-6F))
	{
		_currentVal = _beforeVal;
		_jewelGaugeMat->SetScalarParameterValue("Value", _currentVal);
	}
	else
	{
		_jewelGaugeMat->SetScalarParameterValue("Value", _currentVal);
		
		//もう一度この関数を呼び出す
		GetWorld()->GetTimerManager().SetTimer(_timer, this, &UJewelGauge::GaugeUpAnim, 0.016f, false, 0.0f);
	}	
}

// 関数名：MaxGaugeAnim
// 引　数：なし
// 戻り値：void
// 処理内容：ゲージが溜まった時のアニメーション処理
void UJewelGauge::MaxGaugeAnim(float InDeltaTime)
{
	_animTime += InDeltaTime;

	//一定時間経過した
	if (_animTime > 0.15f)
	{
		//現在のインデックスの画像を非表示にする
		switch (_animIndex)
		{
		case 0:
			_gaugeAnimImages["0"]->SetRenderOpacity(0.0f);
			break;
		case 1:
			_gaugeAnimImages["1"]->SetRenderOpacity(0.0f);
			break;
		case 2:
			_gaugeAnimImages["2"]->SetRenderOpacity(0.0f);
			break;
		case 3:
			_gaugeAnimImages["3"]->SetRenderOpacity(0.0f);
			break;
		case 4:
			_gaugeAnimImages["4"]->SetRenderOpacity(0.0f);
			break;
		case 5:
			_gaugeAnimImages["5"]->SetRenderOpacity(0.0f);
			break;
		case 6:
			_gaugeAnimImages["6"]->SetRenderOpacity(0.0f);
			break;
		}

		//インデックスを減らす
		if (_backIndex)
		{
			_animIndex--;

			if (_animIndex == 0) { _backIndex = false; }
		}
		//インデックスを増やす
		else
		{
			_animIndex++;

			if (_animIndex == 6) { _backIndex = true; }
		}

		_animTime = 0.0f;
	}
	//一定時間経過してない
	else
	{
		//現在のインデックスの画像を表示する
		switch (_animIndex)
		{
		case 0:
			_gaugeAnimImages["0"]->SetRenderOpacity(1.0f);
			break;
		case 1:
			_gaugeAnimImages["1"]->SetRenderOpacity(1.0f);
			break;
		case 2:
			_gaugeAnimImages["2"]->SetRenderOpacity(1.0f);
			break;
		case 3:
			_gaugeAnimImages["3"]->SetRenderOpacity(1.0f);
			break;
		case 4:
			_gaugeAnimImages["4"]->SetRenderOpacity(1.0f);
			break;
		case 5:
			_gaugeAnimImages["5"]->SetRenderOpacity(1.0f);
			break;
		case 6:
			_gaugeAnimImages["6"]->SetRenderOpacity(1.0f);
			break;
		}
	}
}

// 関数名：AnimReset
// 引　数：なし
// 戻り値：void
// 処理内容：ジュエルラッシュ開始後、一度アニメーション関連をリセット
void UJewelGauge::AnimReset()
{
	_gaugeAnimImages["0"]->SetRenderOpacity(0.0f);
	_gaugeAnimImages["1"]->SetRenderOpacity(0.0f);
	_gaugeAnimImages["2"]->SetRenderOpacity(0.0f);
	_gaugeAnimImages["3"]->SetRenderOpacity(0.0f);
	_gaugeAnimImages["4"]->SetRenderOpacity(0.0f);
	_gaugeAnimImages["5"]->SetRenderOpacity(0.0f);
	_gaugeAnimImages["6"]->SetRenderOpacity(0.0f);

	_animIndex = 0;
	_animTime = 0.0f;
	_backIndex = false;
}