//--------------------------------------------------------------------
// ファイル名：OilCan.cpp
// 概要　　　：オイル缶クラス
// 作成者　　：山内　将吾
// 更新内容　：2024/01/30	クラスの作成
//--------------------------------------------------------------------

#include "prototype/Obstacle/OilCan.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

// コンストラクタ
AOilCan::AOilCan()
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
	// 必要に応じて、パフォーマンス向上のために切ることもできる。
	PrimaryActorTick.bCanEverTick = true;

	// コンポーネントを生成する	
	_pCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));	// カプセルコリジョン（当たり判定用）
	RootComponent = _pCapsuleComponent;
}

// ゲームスタート時、または生成時に呼ばれる処理
void AOilCan::BeginPlay()
{
	Super::BeginPlay();

	// カプセルコリジョンが正常に作成されたか確認
	if (_pCapsuleComponent)
	{
		// スフィアコンポーネントを親アクターのRootComponentにアタッチ
		//_pCapsuleComponent->SetupAttachment(RootComponent);
		// AObstacle 親クラスにあるオーバーラップ用コリジョンの配列に追加
		_CollisionComponentsToOverlap = Cast<UPrimitiveComponent>(_pCapsuleComponent);
	}
	else
	{
		// デバッグ用
		//UE_LOG(LogTemp, Warning, TEXT("No oil can _pCapsuleComponent"))
	}

	//妥協処理
	_pSphereComponent = FindComponentByClass<USphereComponent>();
	if (_pSphereComponent)
	{
		// デバッグ用 　オーバーラップにバインドする
		//_pSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AOilCan::OnOverlapBegin);
		// コリジョン判定を無効にする
		_pSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//UE_LOG(LogTemp, Warning, TEXT("Sphere Component found"));
	}
	else
	{
		// デバッグ用
		//UE_LOG(LogTemp, Warning, TEXT("Shere Component do not find"));
	}
}

// 毎フレーム処理
void AOilCan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 妥協処理
	// 親アクターの位置に基づいて子コンポーネントの位置を更新
	if (_pSphereComponent)
	{
		_pSphereComponent->SetWorldLocation(RootComponent->GetComponentLocation());
	}
}

// 関数名：OnBeiginOverLap
// 引数：後に記載
// 戻り値：なし
// 内容：何かしらに当たった瞬間に呼び出される関数
void AOilCan::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// デバッグ用
	//UE_LOG(LogTemp, Log, TEXT("OilCan hit something"));
	//UE_LOG(LogTemp, Log, TEXT("Hit Actor is %s"), *OtherActor->GetName());	

	//当たったアクターのタグがプレイヤーの球なら処理
	if (OtherActor->ActorHasTag("PlayerBullet"))
	{
		// 基底クラスのオーバーラップも処理
		AObstacle::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}

// 関数名：TurnOnExplosionCollision
// 引数：なし
// 戻り値：なし
// 内容：爆発のコリジョンを有効にする関数(Blueprint呼び出し)
void AOilCan::TurnOnExplosionCollision()
{
	// コリジョン判定を有効にする
	if (_pSphereComponent)
	{
		_pSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("_pSphereComponent is not Active"));
	}
}