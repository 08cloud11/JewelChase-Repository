//--------------------------------------------------------------------
// ファイル名：EnemyDamagedComponent.h
// 概要　　　：敵のダメージを受けるコンポーネントクラス
// 作成者　　：山内　将吾
// 更新内容　：2024/11/29	クラスの作成 
//--------------------------------------------------------------------

#pragma once

#include "prototype/Interface/IDamaged.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyDamagedComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROTOTYPE_API UEnemyDamagedComponent : public UActorComponent
{
	// UEのオブジェクトクラスに必ず先頭に書くマクロ
	GENERATED_BODY()

protected:
	// ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:
	// コンストラクタ
	UEnemyDamagedComponent();
	// 毎フレームの更新処理
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ダメージを受けた時
	//void UpDateDamaged(float deltatime);

	// ダメージを受ける関数
	void IsDamaged(int damage);
	// ダメージ演出
	void IsDamagedEffect();


private:

	//UPROPERTY(EditAnywhere)
	//int	 PlayerAttackPower;		// 【デバッグ用】Playerの攻撃力　
};
