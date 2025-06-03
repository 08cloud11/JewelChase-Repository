//--------------------------------------------------------------------
// ファイル名：PoliceCar_Warning.cpp
// 概要　　　：パトカーの警告マークを制御するクラス
// 作成者　　：山内　将吾
// 更新内容　：2025/01/25	クラスの作成 
//--------------------------------------------------------------------

#include "PoliceCar_Warning.h"

// コンストラクタ
APoliceCar_Warning::APoliceCar_Warning()
	: OpacityValue(0.0f)
	, _destroyTime(0.0f)
	, _midPoint(0.0f)
	, _amplitude(0.0f)
	, _timer(0.0f)
	, _blinkingCurrentCount(0)
	, _isBlinking(true)
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
	// 必要に応じて、パフォーマンス向上のために切ることもできる。
	PrimaryActorTick.bCanEverTick = true;
}

// ゲームスタート時、または生成時に呼ばれる処理
void APoliceCar_Warning::BeginPlay()
{
	Super::BeginPlay();	

	// 値の増減において必要な数値の計算
	_midPoint = (_opacityMaxValue + _opacityMinValue) / 2.0f;	// 透明度の最大値と最小値の中心値
	_amplitude = (_opacityMaxValue - _opacityMinValue) / 2.0f;	// 透明度の振れ幅 
}

// 毎フレーム処理
void APoliceCar_Warning::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 時間測定
	_timer += DeltaTime;

	// フラグが上がるまで点滅させる
	if (_isBlinking)
	{
		_updateBlinking();
	}
	else
	{
		this->Destroy();
	}
}

// 関数名：_updateBlinking
// 引数：なし
// 戻り値：なし
// 透明度を滑らかに増減させる関数
void APoliceCar_Warning::_updateBlinking()
{
	// 進行度を正規化する
	float _progressInCycle = FMath::Fmod(_timer, _cycleDuration) / _cycleDuration;

	// Sinカーブを使った値の増減
	OpacityValue =_amplitude * FMath::Sin(2.0f * PI * _progressInCycle) + _midPoint;

	// 透明度が0.0f近くの値になった場合、サイクルを進め点滅を止める
	if (OpacityValue <= 0.0001f)
	{
		_blinkingCurrentCount++;  // 点滅回数をカウント
		//UE_LOG(LogTemp, Warning, TEXT("_blinkingCurrentCount: %i"), _blinkingCurrentCount);

		// 点滅回数が達した場合,点滅停止
		if (_blinkingMaxCount <= _blinkingCurrentCount)
		{
			_isBlinking = false; // 点滅停止
		}
	}

	// デバッグ用
	//UE_LOG(LogTemp, Log, TEXT("_progressInCycle: %f"), _progressInCycle);
	//UE_LOG(LogTemp, Log, TEXT("Current Value: %f"), OpacityValue);
}

