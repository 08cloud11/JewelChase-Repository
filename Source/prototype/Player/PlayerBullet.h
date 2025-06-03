//--------------------------------------------------------------------
// ファイル名：Bullet.h
// 概要　　　：弾
// 作成者　　：坂上壱希
// 更新内容　：12/02
//--------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "prototype/Interface/IMovement.h"
#include "PlayerBullet.generated.h"

UCLASS()
class PROTOTYPE_API APlayerBullet final : public AActor , public IMovement
{
	GENERATED_BODY()
	
public:	
	APlayerBullet();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float deltaTime) override;

	//----------------------------------
	// 関数名：Move
	// 引　数：void
	// 戻り値：void
	// 弾の動き
	//----------------------------------
	virtual void Move(float moveSpeed)override;

	void ChaseTarget(AActor* targetActor);

	//----------------------------------
	// 関数名：DestroyBullet
	// 引　数：float
	// 戻り値：void
	// 弾を消す処理
	//----------------------------------
	void DestroyBullet(float deltaTime);

	//----------------------------------
	// 関数名：SetDirection
	// 引　数：float
	// 戻り値：FVector
	// 弾が向かう方向を設定
	//----------------------------------
	void SetDirection(FVector direction);

	void SetBulletAttack(int Attack);

	void SetTarget(AActor* target);

	int GetBulletAttack();

private:

	UPROPERTY()
	TObjectPtr<AActor> _target;

	//弾が向かう方向
	FVector _direction = FVector::ZeroVector;

	//弾の速度
	UPROPERTY(EditAnywhere)
	float _moveSpeed = 50.0f;

	//消える時間
	UPROPERTY(EditAnywhere)
	float _destroyTime = 2.0f;

	float _timer;

	UPROPERTY(EditAnywhere)
	int BulletAttack = 1;
};
