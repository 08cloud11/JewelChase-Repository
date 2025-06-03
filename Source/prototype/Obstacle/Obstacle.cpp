//--------------------------------------------------------------------
// ファイル名：Obstacle.cpp
// 概要　　　：障害物の基底クラス
// 作成者　　：山内　将吾
// 更新内容　：2024/01/30	クラスの作成
//--------------------------------------------------------------------

#include "prototype/Obstacle/Obstacle.h"
#include "prototype/Player/PlayerBullet.h"
#include "Components/BoxComponent.h"

// コンストラクタ
AObstacle::AObstacle()
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
	// 必要に応じて、パフォーマンス向上のために切ることもできる。
	PrimaryActorTick.bCanEverTick = true;

	//// カプセルコリジョンにオーバーラップを設定
	//if (_pBoxComponent)
	//{
	//	_pBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
	//}
}

// ゲームスタート時、または生成時に呼ばれる処理
void AObstacle::BeginPlay()
{
	Super::BeginPlay();

	// コリジョンコンポーネントをオーバーラップに設定
	//_CollisionComponentsToOverlap->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::OnOverlapBegin);
}

// 毎フレーム処理
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 関数名：OnBeiginOverLap
// 引数：後に記載
// 戻り値：なし
// 内容：何かしらに当たった瞬間に呼び出される関数
void AObstacle::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 【デバッグ用】
	//UE_LOG(LogTemp, Log, TEXT("Obstacle hit something"))

	//当たったアクターのタグがプレイヤーの球なら処理
	if (OtherActor->ActorHasTag("PlayerBullet"))
	{
		if (IsValid(OtherActor))
		{
			APlayerBullet* PlayerBullet = Cast<APlayerBullet>(OtherActor);
			Hp -= PlayerBullet->GetBulletAttack();
		}
	}
}


