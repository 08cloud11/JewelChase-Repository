//--------------------------------------------------------------------
// ファイル名：PoliceAttackComponent.h
// 概要　　　：警察（敵）の攻撃するコンポーネント
// 作成者　　：山内　将吾
// 更新内容　：2024/12/03	クラスの作成
//--------------------------------------------------------------------

#pragma once

#include "prototype/Interface/IBulletFactory.h"		// 弾を打つ関数があるインターフェース
#include "CoreMinimal.h"
#include "EnemyAttackComponent.h"
#include "PoliceAttackComponent.generated.h"

// 前方宣言
class ULineControllerComponent;
class AEnemyBullet;
class USEManager;
class USoundBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROTOTYPE_API UPoliceAttackComponent : public UEnemyAttackComponent, public IBulletFactory
{
	GENERATED_BODY()

protected:
	// ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:
	// コンストラクタ
	UPoliceAttackComponent();
	// 毎フレーム処理
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// 各敵の攻撃を呼び出す関数
	void EnemyAttack(float _deltaTime) override;

	UFUNCTION()
	void SpawnBullet();	// 弾をインスタンス化する関数
	UFUNCTION()
	void DrawPredictionSplineWithCollision(float DeltaTime);	// 予測線を出す関数

	// 攻撃力を取得する関数
	int GetAttackPower()const { return _policeAttackPower; }

	// 警察の状態を取得するための関数
	UFUNCTION(BlueprintCallable, Category = "MyDunctions")
	EnemyStatus GetPoliceStatus()const { return EnemyState; }
	// 発射間隔を取得する関数
	UFUNCTION(BlueprintCallable)
	float GetTimeToFire() { return _timeToFire; }
	// 攻撃可能かどうかのフラグを取得する関数
	UFUNCTION(BlueprintCallable)
	bool GetAttackAbleFlag() { return _isAttackAble; }

	// 妥協処理
	UFUNCTION(BlueprintCallable)
	AActor* GetPoliceActor() { return _owner; }

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyBullet> _enemyBulletActor;	// 弾の設定 (BP側から弾Actorの指定)

	UPROPERTY()
	TArray<AActor*> _pItems;	// アイテムすべて（ジュエル・マグネット・回復）格納用
	UPROPERTY()
	TArray<AActor*> _pSmogs;	// 霧格納用

	UPROPERTY()
	USEManager* _seManager;			// サウンドマネージャー

	UPROPERTY(EditAnywhere)
	USoundBase* _gunSound;			// 警察の射撃音

	UPROPERTY()
	ULineControllerComponent* _pLineControllerComponent;		// 予測線を出すためのコンポーネント

	//UPROPERTY()
	AActor* _owner;	// オーナーのアクター

	UPROPERTY(EditAnywhere)
	FVector _enemyBulletOffset;	// 弾の発射位置のオフセット

	UPROPERTY(EditAnywhere)
	float _bulletSpeed;			// 弾のスピード
	UPROPERTY(EditAnywhere)
	float _timeToAttack;		// スポーンしてから予測線出すまでの時間
	UPROPERTY(EditAnywhere)
	float _enemyAttackCooldown;		// 球を打った後再度プレイヤーを狙う時間
	UPROPERTY(EditAnywhere)
	float _predictionStopToFireInterval;	// 予測線が止まってから発砲するまでの時間
	UPROPERTY(EditAnyWhere)
	float _timeToFire;			// 発射間隔(時間)

	UPROPERTY(EditAnywhere)
	int _policeAttackPower;		// 攻撃力

	FCollisionQueryParams _queryParamsToTrace;		// 無視するクエリパラメータ（トレース用）

	float _canAttackTimer;	// タイマー(出現してから予測線を出すまでの時間計測)
	float _bulletTimer;		// タイマー(予測線を出してから発射するまでの時間計測)
	bool _hasFired;			// 球を打ったかのフラグ
};
