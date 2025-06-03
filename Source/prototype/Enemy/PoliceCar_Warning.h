//--------------------------------------------------------------------
// ファイル名：PoliceCar_Warning.h
// 概要　　　：パトカーの警告マークを制御するクラス
// 作成者　　：山内　将吾
// 更新内容　：2025/01/25	クラスの作成 
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoliceCar_Warning.generated.h"

UCLASS()
class PROTOTYPE_API APoliceCar_Warning : public AActor
{
	GENERATED_BODY()

protected:
	// ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:
	// コンストラクタ
	APoliceCar_Warning();
	// 毎フレーム処理
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float OpacityValue;	// オパシティの値

private:
	UPROPERTY(EditAnywhere)
	float _opacityMaxValue;		// 透明度の最大値
	UPROPERTY(EditAnywhere)
	float _opacityMinValue;		// 透明度の最小値
	UPROPERTY(EditAnywhere)
	float _destroyTime;	// スポーンしてから消滅するまでの時間
	UPROPERTY(EditAnywhere)
	float _cycleDuration;	// 一回の点滅にかかる時間

	UPROPERTY(EditAnywhere)
	int32 _blinkingMaxCount;		// 点滅を行う(増減で１)回数

	float _midPoint;	// 透明度の最大値と最小値の中心値
	float _amplitude;	// 透明度の振れ幅
	float _timer;	// 経過時間測定用
	int32 _blinkingCurrentCount;	// 現在の点滅回数
	bool _isBlinking;	// 点滅をさせるかどうかのフラグ

	void _updateBlinking();	// 透明度を滑らかに増減させる関数
};