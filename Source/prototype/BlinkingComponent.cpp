//--------------------------------------------------------------------
// ファイル名：BlinkingComponent.cpp
// 概要　　　：点滅するためのコンポーネント
// 作成者　　：山内　将吾
// 更新内容　：2025/1/10	クラスの作成
//--------------------------------------------------------------------

#include "BlinkingComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

// コンストラクタ
UBlinkingComponent::UBlinkingComponent()
	: OpacityValue(1.0f)
	, _midPoint(0.0f)
	, _isBlinking(false)
{
    // 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
    // 必要に応じて、パフォーマンス向上のために切ることもできる。
    PrimaryComponentTick.bCanEverTick = true;
}

// ゲームスタート時、または生成時に呼ばれる処理
void UBlinkingComponent::BeginPlay()
{
    Super::BeginPlay();

	// 値の増減において必要な数値の計算
	_midPoint = (_opacityMaxValue + _opacityMinValue) / 2.0f;	// 透明度の最大値と最小値の中心値
	_amplitude = (_opacityMaxValue - _opacityMinValue) / 2.0f;	// 透明度の振れ幅 
}

// 毎フレーム処理
void UBlinkingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// フラグが上がるまで点滅させる
	if (_isBlinking)
	{
		_updateBlinking(DeltaTime);
	}
	else
	{
		_timer = 0.0f;
	}
}

// 関数名：TriggerBlinking
// 引数：点滅にかかる全体の時間 float型
// 戻り値：なし
// 内容：点滅のトリガーをONにする関数(もし引数を渡せばその渡された時間分点滅させる) 
void UBlinkingComponent::TriggerBlinking(float blinkDuration)
{
	// 点滅開始
	_isBlinking = true;
	_blinkDuration = blinkDuration;
}


// 関数名：_updateBlinking
// 引数：１フレームにかかる時間
// 戻り値：なし
// 透明度を増減させる関数
void UBlinkingComponent::_updateBlinking(float _deltaTime)
{
	// フラグが経っていない場合処理しない
	if (!_isBlinking) return;

	// 経過時間計測
	_timer += _deltaTime; // 1フレームごとに時間を進める

	// 残り点滅時間に応じて点滅間隔を速めるための変数を計算
	float _remainingTime = _blinkDuration - _timer;	// 残り時間
	float _blinkingSpeed = FMath::Clamp(1.0f / _remainingTime, _blinkMinSpeed, _blinkMaxSpeed); // 指定した範囲内で点滅スピードを増やす
	// 点滅の進行度を計算
	float _progressCycle = FMath::Fmod(_timer * _blinkingSpeed, 1.0f);

	// Sinカーブを使った値の増減
	OpacityValue = _amplitude * FMath::Sin(2.0f * PI * _progressCycle) + _midPoint;

	// 点滅時間が終了した場合、点滅を停止して不透明に設定
	if (_timer >= _blinkDuration)
	{
		_isBlinking = false;  // 点滅を停止
		OpacityValue = 1.0f;  // 完全に不透明にする
	}

	// デバッグ用
	//UE_LOG(LogTemp, Log, TEXT("_progressInCycle: %f"), _progressInCycle);
	//UE_LOG(LogTemp, Log, TEXT("Current Value: %f"), OpacityValue);
}