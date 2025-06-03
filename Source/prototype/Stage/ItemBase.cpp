//--------------------------------------------------------------------
// ファイル名 ：ItemBase.cpp
// 概要       ：アイテムの基盤のクラス。機能は特にない
// 作成者     ：0231本間
// 更新内容   ：1/19　作成
//--------------------------------------------------------------------


#include "ItemBase.h"

// Sets default values
AItemBase::AItemBase()
	:bget(false)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	SetBeginZpos();
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bget) 
	{
		UptoDestroy();
	}
}

void AItemBase::SetBeginZpos()
{
	BeginZpos = GetActorLocation().Z;
}

float AItemBase::GetBeginZpos()
{
	return BeginZpos;
}

void AItemBase::bgetON()
{
	bget = true;
	getZpos = GetActorLocation().Z;
}

void AItemBase::UptoDestroy()
{
	FVector pos = GetActorLocation();
	SetActorLocation(FVector(pos.X, pos.Y, pos.Z + 10.0f));

	if (GetActorLocation().Z > getZpos + 150.0f)
	{
		Destroy();
	}
}