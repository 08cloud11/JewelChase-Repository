//--------------------------------------------------------------------
// ファイル名：EnemyAttackComponent.h
// 概要　　　：プレイヤーにダメージを与えるコンポーネントクラス
// 作成者　　：山内　将吾
// 更新内容　：2024/12/02	クラスの作成 
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyAttackComponent.generated.h"

// 前方宣言
class USplineComponent;

// 敵のステータスをenum型で制御する 妥協ほんとはEnemyクラスに宣言したい
UENUM(BlueprintType) // Blueprintで使用可能にするマクロ
enum class EnemyStatus : uint8
{
	Idle UMETA(DisplayName = "Idle"),	// 待機状態
	Style UMETA(DisplayName = "Style"),	// 構え状態
	Fire UMETA(DisplayName = "Fire"),	// 発砲状態
	Die UMETA(DisplayName = "Die"),		// 死亡状態
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROTOTYPE_API UEnemyAttackComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:
	// コンストラクタ
	UEnemyAttackComponent();

	// 毎フレーム処理
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, Category = "EnemyStatus")
	EnemyStatus EnemyState;	// 敵のステータス　妥協ほんとはEnemyクラスに宣言したい

	bool _isAttackAble;	// 攻撃できるかどうかのフラグ

	// 各敵の攻撃を呼び出す関数
	UFUNCTION()
	virtual void EnemyAttack(float _deltaTine);

	// 攻撃力を取得する関数
	virtual int GetAttackPower()const { return _enemyAttackPower; }

	// 攻撃できるかどうかのフラグを返す関数
	bool CanAttack() const;

private:
	UPROPERTY(EditAnywhere)
	int _enemyAttackPower;	// 攻撃力
};
