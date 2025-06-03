//--------------------------------------------------------------------
// ファイル名：BlinkingComponent.h
// 概要　　　：点滅するためのコンポーネント
// 作成者　　：山内　将吾
// 更新内容　：2025/1/10	クラスの作成
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlinkingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOTYPE_API UBlinkingComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
    // ゲームスタート時、または生成時に呼ばれる処理
    virtual void BeginPlay() override;

public:
    // コンストラクタ
    UBlinkingComponent();
    // 毎フレーム処理
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float OpacityValue;	// オパシティの値

	void TriggerBlinking(float blinkDuration);	// 点滅のトリガーをONにする関数 

private:
	UPROPERTY(EditAnywhere)
		float _opacityMaxValue;		// 透明度の最大値
	UPROPERTY(EditAnywhere)
		float _opacityMinValue;		// 透明度の最小値
	UPROPERTY(EditAnywhere)
		float _blinkMinSpeed;		// 点滅の最遅スピード
	UPROPERTY(EditAnywhere)
		float _blinkMaxSpeed;		// 点滅を最速スピード
	UPROPERTY()
		float _blinkDuration;		// 点滅を行う全体の時間

	float _midPoint;	// 透明度の最大値と最小値の中心値
	float _amplitude;	// 透明度の振れ幅
	float _timer;	// 経過時間測定用
	bool _isBlinking;	// 点滅をさせるかどうかのフラグ

	void _updateBlinking(float _deltaTime);	// 透明度を滑らかに増減させる関数
};
