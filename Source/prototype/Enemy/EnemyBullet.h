//--------------------------------------------------------------------
// ファイル名：EnemyBullet.h
// 概要　　　：敵の弾クラス
// 作成者　　：山内　将吾
// 更新内容　：2024/12/08	クラスの作成
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "prototype/Interface/IMovement.h"
#include "EnemyBullet.generated.h"

// 前方宣言
class UStaticMeshComponent;
class UEnemyBullet;
class UProjectileMovementComponent;
class USphereComponent;

class AEnemy;

UCLASS()
class PROTOTYPE_API AEnemyBullet : public AActor
{
	GENERATED_BODY()

protected:
	// ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:
	// コンストラクタ
	AEnemyBullet();
	// 毎フレーム処理
	virtual void Tick(float DeltaTime) override;

	// 衝突し始めた時に呼び出す関数（物理的接触は無し）
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 進む方向を設定する関数
	void SetDirection(FVector direction);
	// 弾のスピードを設定する関数
	void SetBulletSpeed(float _bulletSpeed);

private:
	UPROPERTY(EditAnywhere)
	float _timeToDisappear;
	UPROPERTY()
	USphereComponent* _pSphereComponent;	// スフィアコリジョンコンポーネント

	FVector _direction;		// 弾が飛ぶ方向
	float _moveSpeed;	// 弾のスピード
	float _timer;	// タイマー
};
