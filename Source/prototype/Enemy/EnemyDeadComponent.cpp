//--------------------------------------------------------------------
// ファイル名：EnemyDeadComponent.cpp
// 概要　　　：死亡するためのコンポーネントクラス
// 作成者　　：山内　将吾
// 更新内容　：2024/11/29	クラスの作成 
//--------------------------------------------------------------------

#include "EnemyDeadComponent.h"

// コンストラクタ
UEnemyDeadComponent::UEnemyDeadComponent()
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
	// 必要に応じて、パフォーマンス向上のために切ることもできる。
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// ゲームスタート時、または生成時に呼ばれる処理
void UEnemyDeadComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// 毎フレームの更新処理
void UEnemyDeadComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

