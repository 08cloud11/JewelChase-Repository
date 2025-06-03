//--------------------------------------------------------------------
// ファイル名：TrashCan.cpp
// 概要　　　：ゴミ箱クラス
// 作成者　　：山内　将吾
// 更新内容　：2024/02/12 クラスの作成
//--------------------------------------------------------------------

#include "prototype/Obstacle/TrashCan.h"
#include "prototype/Stage/StageManager.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// コンストラクタ
ATrashCan::ATrashCan()
	: DropItemValue(DropItems::Jewel_Blue)
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
	// 必要に応じて、パフォーマンス向上のために切ることもできる。
	PrimaryActorTick.bCanEverTick = true;

	// コンポーネントを生成する	
	_pBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("_pBoxComponent"));	// カプセルコリジョン（当たり判定用）
}

// ゲームスタート時、または生成時に呼ばれる処理
void ATrashCan::BeginPlay()
{
	Super::BeginPlay();

	// カプセルコリジョンが正常に作成されたか確認
	if (_pBoxComponent)
	{
		// AObstacle 親クラスにあるオーバーラップ用コリジョンの配列に追加
		_CollisionComponentsToOverlap = Cast<UPrimitiveComponent>(_pBoxComponent);
	}
	else
	{
		// デバッグ用
		//UE_LOG(LogTemp, Warning, TEXT("No oil can _pBoxComponent"))
	}
}

// 関数名：OnBeiginOverLap
// 引数：後に記載
// 戻り値：なし
// 内容：何かしらに当たった瞬間に呼び出される関数
void ATrashCan::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

// 関数名：SpawnDropItem
// 引数：なし
// 戻り値：なし
// 内容：アイテムをドロップさせる関数
void ATrashCan::SpawnDropItem()
{
	// スポーンする位置（自身の位置）
	FVector SpawnLocation = GetActorLocation(); // 自身の位置を取得
	// スポーンの回転（この場合はアイテムが正面を向くようにする）
	FRotator SpawnRotation = FRotator::ZeroRotator;
	// スポーンさせるアクターの変数
	AActor* SpawnedItem = nullptr;

	// eunm 型に対応したドロップアイテムを設定する
	if (DropItemValue == DropItems::Jewel_Blue)
	{
		// 青ジュエル
		SpawnedItem = GetWorld()->SpawnActor<AActor>(_pDropItems[0], SpawnLocation, SpawnRotation);
	}
	else if (DropItemValue == DropItems::Jewel_Red)
	{
		// 赤ジュエル
		SpawnedItem = GetWorld()->SpawnActor<AActor>(_pDropItems[0], SpawnLocation, SpawnRotation);
	}
	else if (DropItemValue == DropItems::Jewel_Yellow)
	{
		// 黄ジュエル
		SpawnedItem = GetWorld()->SpawnActor<AActor>(_pDropItems[0], SpawnLocation, SpawnRotation);
	}
	else if (DropItemValue == DropItems::Jewel_Purple)
	{
		// 紫ジュエル
		SpawnedItem = GetWorld()->SpawnActor<AActor>(_pDropItems[0], SpawnLocation, SpawnRotation);
	}
	else if (DropItemValue == DropItems::HpItem)
	{
		// 体力回復
		SpawnedItem = GetWorld()->SpawnActor<AActor>(_pDropItems[0], SpawnLocation, SpawnRotation);
	}
	else if (DropItemValue == DropItems::Magnet)
	{
		// マグネット
		SpawnedItem = GetWorld()->SpawnActor<AActor>(_pDropItems[0], SpawnLocation, SpawnRotation);
	}
	else
	{
		// デバッグ用
		//UE_LOG(LogTemp, Warning, TEXT("do not set DropItemValue"));
	}

	// 妥協処理
	// アニメーションを適用
	if (SpawnedItem != nullptr)
	{
		//AStageManager型のオブジェクトを探し、管理
		TSubclassOf<AStageManager> findClass;
		findClass = AStageManager::StaticClass();

		AActor* stageManager = UGameplayStatics::GetActorOfClass(GetWorld(), findClass);

		FName ItemTag = "Item";
		TArray<AActor*> Items;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), ItemTag, Items);

		Items.Push(SpawnedItem);

		if (stageManager)
		{
			Cast<AStageManager>(stageManager)->SetItems(Items);
		}
		else
		{
			// デバッグ用
			//UE_LOG(LogTemp, Warning, TEXT("No stageManager"));
		}
	}
}