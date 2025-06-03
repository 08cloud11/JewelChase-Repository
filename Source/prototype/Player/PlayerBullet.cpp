//--------------------------------------------------------------------
// ファイル名：Bullet.cpp
// 概要　　　：弾
// 作成者　　：坂上壱希
// 更新内容　：12/02
//--------------------------------------------------------------------

#include "PlayerBullet.h"
#include "Kismet/GameplayStatics.h"

APlayerBullet::APlayerBullet()
	: _target(nullptr)
	, _timer(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerBullet::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerBullet::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	//if (_target && _target->GetWorld())
	//{
	//	ChaseTarget(_target);
	//}	山内
	// ターゲットが存在するか否か　山内
	if (IsValid(_target))// 山内
	{
		ChaseTarget(_target);
	}
	else
	{
		Move(_moveSpeed);
	}

	DestroyBullet(deltaTime);
}

void APlayerBullet::Move(float moveSpeed)
{
#if false
	//実際の動き
	if (_targetPos == FVector::ZeroVector)
	{
		return;
	}
	FVector myPos = GetActorLocation();


#else
	//テスト用の動き
	//FVector3f mypos = static_cast<FVector3f>(GetActorLocation());
	//mypos.X += moveSpeed;
	//SetActorLocation(static_cast<FVector>(mypos));

	SetActorLocation(GetActorLocation() + _direction * moveSpeed);

#endif // 0
}

void APlayerBullet::ChaseTarget(AActor* targetActor)
{
	const float VELOCITY_COMPENSTATION = 12.0f;	//速度補正値

	//ターゲットがいなければ削除する
	if (!IsValid(_target))
	{
		this->Destroy();
	}

	// ターゲットがいれば処理
	if (_target && targetActor)
	{
		FVector targetLocation = targetActor->GetActorLocation();
		FVector myLocation = GetActorLocation();

		// ターゲット方向の正規化ベクトル
		FVector direction = (targetLocation - myLocation).GetSafeNormal();
		_direction = direction; //山内（ターゲットが存在する間に飛ぶ方向を持っておく）

		// 速度を考慮して移動
		float deltaTime = GetWorld()->GetDeltaSeconds();
		SetActorLocation(myLocation + direction * (_moveSpeed + VELOCITY_COMPENSTATION));// 山内（トランプのスピードが遅い原因直した）試したところ12が一番近いスピードになる
	}
}

void APlayerBullet::DestroyBullet(float deltaTime)
{
	_timer += deltaTime;
	if (_timer >= _destroyTime)
	{
		_timer = 0.0f;
		this->Destroy();
	}
}

void APlayerBullet::SetDirection(FVector direction)
{
	_direction = direction;
}

void APlayerBullet::SetTarget(AActor* target)
{
	_target = target;
}

void APlayerBullet::SetBulletAttack(int Attack)
{
	BulletAttack = Attack;
}

int APlayerBullet::GetBulletAttack()
{
	return BulletAttack;
}