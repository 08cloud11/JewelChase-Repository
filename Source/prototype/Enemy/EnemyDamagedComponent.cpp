//--------------------------------------------------------------------
// ファイル名：EnemyDamagedComponent.cpp
// 概要　　　：敵のダメージを受けるコンポーネントクラス
// 作成者　　：山内　将吾
// 更新内容　：2024/11/29	クラスの作成 
//           ：2024/12/05	もしかしたらこのコンポーネントいらないかも・・・
//--------------------------------------------------------------------

#include "EnemyDamagedComponent.h"

// Sets default values for this component's properties
UEnemyDamagedComponent::UEnemyDamagedComponent()
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
	// 必要に応じて、パフォーマンス向上のために切ることもできる。
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// ゲームスタート時、または生成時に呼ばれる処理
void UEnemyDamagedComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...	
}

// 毎フレームの更新処理
void UEnemyDamagedComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// 関数名：IsDamagedEffect
// 引数：
// 戻り値：なし
// 
void UEnemyDamagedComponent::IsDamagedEffect()
{
	;
}
