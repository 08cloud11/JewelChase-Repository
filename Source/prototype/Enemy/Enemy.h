//--------------------------------------------------------------------
// ファイル名：Enemy.h
// 概要　　　：敵（敵に対してダメージを与えるもの）の根幹クラス
// 作成者　　：山内　将吾
// 更新内容　：2024/11/25	Enemyクラスの作成
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enemy.generated.h"

//	前方宣言
class USceneConponent;
class UEnemyAttackComponent;
class UEnemyDamagedComponent;
class UEnemyDeadComponent;
class UFlyAwayActorComponent;

UCLASS()
class PROTOTYPE_API AEnemy : public APawn
{
	GENERATED_BODY()

protected:
	// ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:
	// コンストラクタ
	AEnemy();

	// 毎フレーム処理
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int _hp;	// 体力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Score;	// スコア

private:

	UPROPERTY()
	TArray<UPrimitiveComponent*> _pCollisionComponent;		// コリジョンコンポーネント

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EnemyComponent, meta = (AllowPrivateAccess = "true"))
	UEnemyAttackComponent* _pAttackComponent;		        // 攻撃するためのコンポーネント

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EnemyComponent, meta = (AllowPrivateAccess = "true"))
	UEnemyDamagedComponent* _pDamagedComponent;		        // ダメージを受けるためのコンポーネント

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EnemyComponent, meta = (AllowPrivateAccess = "true"))
	UEnemyDeadComponent* _pDeadComponent;		            // 消滅（死ぬ）ためのコンポーネント

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EnemyComponent, meta = (AllowPrivateAccess = "true"))
	UFlyAwayActorComponent* _pFlyAwayActorComponent;		// 吹っ飛ぶためのコンポーネント


	// 衝突し始めた時に呼び出す関数（物理的接触は無し）
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// デバッグ用
	//UFUNCTION()
	//	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	//		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
