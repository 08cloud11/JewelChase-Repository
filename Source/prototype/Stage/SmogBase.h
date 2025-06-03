//--------------------------------------------------------------------
// ファイル名 ：SmogBase.h
// 概要       ：霧の基盤のクラス。
// 作成者     ：0231本間
// 更新内容   ：1/19　作成
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmogBase.generated.h"

UCLASS()
class PROTOTYPE_API ASmogBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmogBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	AActor* pStageMng;
	float moveSpeed;
};
