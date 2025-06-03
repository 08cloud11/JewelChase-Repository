//--------------------------------------------------------------------
// ファイル名：HelicopterAttackComponent.h
// 概要　　　：ヘリコプター（敵）の攻撃するコンポーネント
// 作成者　　：山内　将吾
// 更新内容　：2024/12/31	クラスの作成
//--------------------------------------------------------------------

#pragma once

#include "prototype/Interface/IBulletFactory.h"		// 弾を打つ関数があるインターフェース
#include "CoreMinimal.h"
#include "prototype/Stage/StageData.h"	// 妥協処理
#include "prototype/Stage/StageManager.h"	// 妥協処理
#include "EnemyAttackComponent.h"
#include "HelicopterAttackComponent.generated.h"

// 前方宣言
class ULineControllerComponent;
class AEnemyBullet;
class USEManager;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROTOTYPE_API UHelicopterAttackComponent : public UEnemyAttackComponent, public IBulletFactory
{
	GENERATED_BODY()

protected:
	// ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:
	// コンストラクタ
	UHelicopterAttackComponent();
	// 毎フレーム処理
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// 各敵の攻撃を呼び出す関数
	void EnemyAttack(float _deltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* _playerActor;		// プレイヤーのアクター
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEnemyBullet> EnemyBulletClass;	// 弾の設定 (BP側から弾Actorの指定)

	UFUNCTION()
	void DrawPredictionSplineWithCollision(float DeltaTime);	// 予測線を出す関数
	UFUNCTION()
	void SpawnBullet();	// 弾をインスタンス化する関数

	void EndPlay(const EEndPlayReason::Type EndPlayReason);		// ヘリコプターが削除された時に呼ぶ用

	void SetMoveTargetNumber(int moveTargetNumber);	// 妥協処理

	// 攻撃力を取得する関数
	int GetAttackPower()const { return _policeAttackPower; }
private:

	UPROPERTY()
	TArray<AActor*> _pItems;	// アイテムすべて（ジュエル・マグネット・回復）格納用
	UPROPERTY()
	TArray<AActor*> _pSmogs;	// 霧格納用

	UPROPERTY(EditAnywhere, Category = "MoveTargetActors")
	AActor* _moveTargetActors[3];	// 移動目標アクターの候補

	UPROPERTY()
	USEManager* _seManager;			// サウンドマネージャー

	UPROPERTY(EditAnywhere)
	USoundBase* _rotorSound;		// ヘリコプターの羽の音（ローター音）
	UPROPERTY(EditAnywhere)
	USoundBase* _gunSound;			// 警察の射撃音

	//UPROPERTY()
	AActor* _owner;				// オーナーのアクター
	UPROPERTY()
	AActor* _moveTargetActor;	// 移動目標アクター

	UPROPERTY()
	ULineControllerComponent* _pLineControllerComponent;		// 予測線を出すためのコンポーネント

	UPROPERTY(EditAnywhere)
	FVector _enemyBulletOffset;	// 弾の発射位置のオフセット

	UPROPERTY(EditAnywhere)
	float _canAttackTime;					// 出現してから定位置に移動する時間
	UPROPERTY(EditAnywhere)
	float _timeToAttack;					// 予測線出すまでの時間
	UPROPERTY(EditAnywhere)
	float _enemyAttackCooldown;				// 球を打った後再度プレイヤーを狙う時間
	UPROPERTY(EditAnywhere)
	float _predictionStopToFireInterval;	// 予測線が止まってから発砲するまでの時間
	UPROPERTY(EditAnyWhere)
	float _timeToFire;						// 発射間隔(時間)
	UPROPERTY(EditAnywhere)
	float _bulletSpeed;						// 弾のスピード

	UPROPERTY(EditAnywhere)
	int _policeAttackPower;		// 攻撃力

	FCollisionQueryParams _queryParamsToTrace;		// 無視するクエリパラメータ（トレース用）

	FVector _startPointToDrawPrediction;	// 予測線の始点
	FVector _endPointToDrawPrediction;		// 予測線の終点
	FRotator _rotationToFire;				// 予測線と球を打つ際の方向ベクトル
	float _canAttackTimer;					// タイマー(出現してから予測線を出すまでの時間計測)
	float _bulletTimer;						// タイマー(予測線を出してから発射するまでの時間計測)
	int _moveTargetNumber;					// 移動目標のアクターナンバー 妥協処理
	bool _hasFired;							// 球を打ったかのフラグ

	// 移動目標のターゲットを設定する関数 StageBaseで呼び出す用
	void _setMoveTargetActor();

	// 始点から終点への方向ベクトルを計算する関数
	FRotator CalculateRotationByPoints(const FVector& StartPoint, const FVector& EndPoint);

	// ターゲットのポイントまで移動する関数
	void _moveToTargetLocation(float _deltaTime);
};
