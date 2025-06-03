//--------------------------------------------------------------------
// ファイル名 ：ItemBase.h
// 概要       ：アイテムの基盤のクラス。機能は特にない
// 作成者     ：0231本間
// 更新内容   ：1/19　作成
//--------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

UCLASS()
class PROTOTYPE_API AItemBase : public AActor
{
	GENERATED_BODY()

private:	
	void UptoDestroy();

protected:	
	virtual void BeginPlay() override;	

public:
	AItemBase();
	virtual void Tick(float DeltaTime) override;

	void SetBeginZpos();
	float GetBeginZpos();
	UFUNCTION(BlueprintCallable)
	void bgetON();
	bool Getbget() const { return bget; }

private:
	float BeginZpos;
	bool bget;
	float getZpos;
};
