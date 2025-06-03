//--------------------------------------------------------------------
// ファイル名 ：StageManager.cpp
// 概要       ：ステージ生成の為の番号、csvパスを管理するクラス
// 作成者     ：0231本間
// 更新内容   ：11/29　作成、
//--------------------------------------------------------------------
#include "StageManager.h"
#include "prototype/Player/PlayerCharacter.h"
#include "prototype/Stage/ItemBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AStageManager::AStageManager()
	:_bgoalspawn(false)
	, toDefaultAddZpos(0.0f)
	, bItemsFloating(true)
	, bMagnet(false)
	, magnetTime(0.0f)
	, _bStart(false)
	, _bGoal(false)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	srand(time(NULL));
}

// Called when the game starts or when spawned
void AStageManager::BeginPlay()
{
	Super::BeginPlay();

	//BPのプロパティで設定したレーン数が７より大きかったら
	if (_stagelane > 7) { _stagelane = 7; }
}

// Called every frame
void AStageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateFloating();

	approachItem(DeltaTime);
}

// 関数名：Get_csvpass
// 引　数：なし
// 戻り値：TSoftObjectPtr<UDataTable>
// 処理内容：次に生成するステージのcsvパスを返す
TSoftObjectPtr<UDataTable> AStageManager::Get_csvpath()
{
	_stagenum++;
	return _csvpath[_stagenum - 1];
}

// 関数名：Get_goalpath
// 引　数：なし
// 戻り値：TSoftObjectPtr<UDataTable>
// 処理内容：ゴールオブジェクトを配置したステージのcsvパスを返す
TSoftObjectPtr<UDataTable> AStageManager::Get_goalpath()
{
	_bgoalspawn = true;
	return _goalpath;
}

// 関数名：Get_stagenum
// 引　数：なし
// 戻り値：int16
// 処理内容：ステージ番号を返す
int32 AStageManager::Get_stagenum()
{
	return _stagenum;
}

// 関数名：Get_maxstagenum
// 引　数：なし
// 戻り値：int16
// 処理内容：最大ステージ番号を返す
int32 AStageManager::Get_maxstagenum()
{
	return _maxstagenum;
}

// 関数名：Get_stagelane
// 引　数：なし
// 戻り値：int8
// 処理内容：ステージのレーン数を返す
int8 AStageManager::Get_stagelane()
{
	return _stagelane;
}

// 関数名：Get_movespeed
// 引　数：なし
// 戻り値：float
// 処理内容：ステージの移動速度を返す
float AStageManager::Get_movespeed()
{
	return _movespeed;
}

// 関数名：Get_bgoalspawn
// 引　数：なし
// 戻り値：bool
// 処理内容：ゴールステージをスポーンしたかを返す
bool AStageManager::Get_bgoalspawn()
{
	return _bgoalspawn;
}

// 関数名：UpdateFloating
// 引　数：なし
// 戻り値：なし
// 処理内容：アイテムの浮遊を更新する
void AStageManager::UpdateFloating()
{
	if (bItemsFloating)
	{
		toDefaultAddZpos += zmoveLength;

		if (toDefaultAddZpos > 25.0f) {
			toDefaultAddZpos = 25.0f;
			bItemsFloating = false;
		}
	}
	else
	{
		toDefaultAddZpos -= zmoveLength;

		if (toDefaultAddZpos < -25.0f) {
			toDefaultAddZpos = -25.0f;
			bItemsFloating = true;
		}
	}

	for (auto Item : Items)
	{
		if (IsValid(Item) && !Cast<AItemBase>(Item)->Getbget())
		{
			FVector beforepos = Item->GetActorLocation();
			FVector newpos = FVector(beforepos.X, beforepos.Y, Cast<AItemBase>(Item)->GetBeginZpos() + toDefaultAddZpos);

			Item->SetActorLocation(newpos);
		}
	}
}

// 関数名：SetItems
// 引　数：TArray<AActor*>
// 戻り値：なし
// 処理内容：UpdateFloating()で更新するアイテムをステージベースから受け取る
void AStageManager::SetItems(TArray<AActor*> Others)
{
	Items = Others;
}

// 関数名：_bMagnetOn
// 引　数：なし
// 戻り値：なし
// 処理内容：マグネットを取得したときにアイテムを近づかせる
//			 bool型をtrueにする
void  AStageManager::_bMagnetOn()
{
	bMagnet = true;
}

// 関数名：approachItem
// 引　数：float DeltaTime
// 戻り値：なし
// 処理内容：マグネットを取得しているときにアイテムをプレイヤに接近させる
void AStageManager::approachItem(float DeltaTime)
{
	if (bMagnet) {
		TSubclassOf<APlayerCharacter> findClass;
		findClass = APlayerCharacter::StaticClass();

		AActor* pPlayer = UGameplayStatics::GetActorOfClass(GetWorld(), findClass);
		FVector PlayerLocation = pPlayer->GetActorLocation();

		float DistanceX;
		float DistanceY;
		float DistanceZ;
		float Distancetotal;

		for (auto Item : Items)
		{
			if (IsValid(Item) && Item->GetActorLocation().X < 1500.0f && !Cast<AItemBase>(Item)->Getbget()) {
				FVector ItemLocation = Item->GetActorLocation();

				DistanceX = PlayerLocation.X - ItemLocation.X;
				DistanceY = PlayerLocation.Y - ItemLocation.Y;
				DistanceZ = PlayerLocation.Z - ItemLocation.Z;

				Distancetotal = abs(DistanceX) + abs(DistanceY) + abs(DistanceZ);

				float SetXpos = ItemLocation.X - (abs(DistanceX) / Distancetotal) * magnetmoveLength;

				float SetYpos;

				if (DistanceY > 0.0f) SetYpos = ItemLocation.Y + (abs(DistanceY) / Distancetotal) * magnetmoveLength;
				else SetYpos = ItemLocation.Y - (abs(DistanceY) / Distancetotal) * magnetmoveLength;

				float SetZpos = ItemLocation.Z;

				if (DistanceZ < 0.0f) SetZpos = ItemLocation.Z - (abs(DistanceZ) / Distancetotal) * magnetmoveLength * 5.0;
				else  SetZpos = ItemLocation.Z + (abs(DistanceZ) / Distancetotal) * magnetmoveLength * 2.35;
				

				FVector newItemLocation = FVector(SetXpos, SetYpos, SetZpos);

				Item->SetActorLocation(newItemLocation);
			}
		}

		magnetTime += DeltaTime;

		if (magnetTime > 5.0f)
		{
			bMagnet = false;
			magnetTime = 0.0f;

			for (auto Item : Items)
			{
				if (IsValid(Item))
				{
					Cast<AItemBase>(Item)->SetBeginZpos();
				}
			}
		}
	}
}

