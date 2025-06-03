//--------------------------------------------------------------------
// ファイル名：EnemyAttackComponent.cpp
// 概要　　　：プレイヤーにダメージを与えるコンポーネントクラス
// 作成者　　：山内　将吾
// 更新内容　：2024/12/02	クラスの作成 
//--------------------------------------------------------------------

#include "EnemyAttackComponent.h"
#include "Components/SplineComponent.h"

// コンストラクタ
UEnemyAttackComponent::UEnemyAttackComponent()
	: EnemyState(EnemyStatus::Idle)
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
	// 必要に応じて、パフォーマンス向上のために切ることもできる。
	PrimaryComponentTick.bCanEverTick = true;
}


// ゲームスタート時、または生成時に呼ばれる処理
void UEnemyAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

// 毎フレーム処理
void UEnemyAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

//// 関数名：GetEnemyPower
//// 引数：なし
//// 戻り値：敵の攻撃力
//// 内容：各敵の攻撃力を取得するための関数
//int UEnemyAttackComponent::GetEnemyPower()
//{
//	return _enemyPower;
//}

// 関数名：CanAttack
// 引数：
// 戻り値：攻撃できるかどうかのフラグ
// 内容：攻撃できるかどうかを返す関数
bool UEnemyAttackComponent::CanAttack() const
{
	return _isAttackAble;
}

// 関数名：EnemyAttack
// 引数：なし
// 戻り値：なし
// 内容：各Enemyの攻撃の処理を呼び出す仮想(virtual)関数
void UEnemyAttackComponent::EnemyAttack(float deltaTime)
{
	// 【デバッグ用】
	//UE_LOG(LogTemp, Log, TEXT("Invoke EnemyAttack()"));

	// 設定していない
}	