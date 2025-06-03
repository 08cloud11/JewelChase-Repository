//--------------------------------------------------------------------
// ファイル名：Enemy.cpp
// 概要　　　：敵（敵に対してダメージを与えるもの）の根幹クラス
// 作成者　　：山内　将吾
// 更新内容　：2024/11/25	Enemyクラスの作成
//--------------------------------------------------------------------

#include "Enemy.h"
#include "EnemyAttackComponent.h"
#include "EnemyDamagedComponent.h"
#include "EnemyDeadComponent.h"
#include "prototype/Player/PlayerCharacter.h"
#include "prototype/FlyAwayActorComponent.h"
#include "prototype/Player/PlayerBullet.h"

// コンストラクタ
AEnemy::AEnemy()
	: _hp(0)
	, _pAttackComponent(nullptr)
	, _pDamagedComponent(nullptr)
	, _pDeadComponent(nullptr)
	, _pFlyAwayActorComponent(nullptr)
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
	// 必要に応じて、パフォーマンス向上のために切ることもできる。
	PrimaryActorTick.bCanEverTick = true;

	_pAttackComponent = CreateDefaultSubobject<UEnemyAttackComponent>(TEXT("_pAttackComponent"));		        // 攻撃するためのコンポーネント
	//_pDamagedComponent = CreateDefaultSubobject<UEnemyDamagedComponent>(TEXT("_pDamagedComponent"));	        // ダメージを受けるためのコンポーネント
	//_pDeadComponent = CreateDefaultSubobject<UEnemyDeadComponent>(TEXT("_pDeadComponent"));				    // 死ぬためのコンポーネント
	_pFlyAwayActorComponent = CreateDefaultSubobject<UFlyAwayActorComponent>(TEXT("_pFlyAwayActorComponent"));	// 攻撃するためのコンポーネント
}

// ゲームスタート時、または生成時に呼ばれる処理
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// アクターについている UPrimitiveComponent を取得
	GetComponents<UPrimitiveComponent>(_pCollisionComponent);

	for (int32 i = _pCollisionComponent.Num() - 1; i >= 0; i--)
	{
		if (!_pCollisionComponent[i]->GetCollisionEnabled() || !_pCollisionComponent[i]->GetGenerateOverlapEvents())
		{
			_pCollisionComponent.RemoveAt(i);  // コリジョン無効なコンポーネントを除外
		}
	}
	//UE_LOG(LogTemp, Log, TEXT("Found %d valid collision components"), _pCollisionComponent.Num());	// デバッグ用
	for (UPrimitiveComponent* Comp : _pCollisionComponent)
	{
		//UE_LOG(LogTemp, Log, TEXT("Valid Collision Component: %s"), *Comp->GetName());	// デバッグ用
		// オーバーラップにバインドする
		Comp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
	}
}

// 毎フレーム処理
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 攻撃処理を毎フレーム呼び出す
	if (_pAttackComponent != nullptr)
	{
		// 攻撃可能か？
		if (_pAttackComponent->CanAttack())
		{
			_pAttackComponent->EnemyAttack(DeltaTime); //UE_LOG(LogTemp, Log, TEXT("_pAttackComponent"));
		}
	}
}

// 関数名：OnBeiginOverLap
// 引数：後に記載
// 戻り値：なし
// 内容：何かしらに当たった瞬間に呼び出される関数
void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 【デバッグ用】
	//UE_LOG(LogTemp, Log, TEXT("Enemy hit something"))
	//UE_LOG(LogTemp, Log, TEXT("Hit Actor is %s"), *OtherActor->GetName());	

	//// デバッグ用
	//if (OtherActor->ActorHasTag("Oil_Explosion"))
	//{
	//	UE_LOG(LogTemp, Log, TEXT("Hit Oil_Explosion"));
	//}

	// 当たったアクターのタグがプレイヤーの球なら処理
	if (OtherActor->ActorHasTag("PlayerBullet"))
	{
		//UE_LOG(LogTemp, Log, TEXT("Hit Oil_Explosion")); // デバッグ用
		if (IsValid(OtherActor))
		{
			APlayerBullet* PlayerBullet = Cast<APlayerBullet>(OtherActor);
			if (IsValid(PlayerBullet))
			{
				_hp -= PlayerBullet->GetBulletAttack();
				//UE_LOG(LogTemp, Log, TEXT("HP %i"),_hp);   // デバッグ用(これ落ちる)
			}
		}
	}
	else if (OtherActor->ActorHasTag("Player"))
	{
		APlayerCharacter* _playerCharacter = Cast<APlayerCharacter>(OtherActor);
		// プレイヤーがジュエルラッシュ中の場合
		if (_playerCharacter->GetisJewelRush())
		{
			for (int32 i = _pCollisionComponent.Num() - 1; i >= 0; i--)
			{
				// コリジョンをオフにする
				_pCollisionComponent[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}

			// 自分を吹っ飛ばすフラグを逆（true）にする
			_pFlyAwayActorComponent->ChangeFlyFlag();
		}
	}
}

// デバッグ用
//void AEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
//	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	// デバッグ用
//	if (OtherActor->ActorHasTag("Oil_Explosion"))
//	{
//		UE_LOG(LogTemp, Log, TEXT("HitEnd Oil_Explosion"));
//	}
//}