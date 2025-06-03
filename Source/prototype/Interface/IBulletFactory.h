//--------------------------------------------------------------------
// ファイル名：IBulletFactory
// 概要　　　：弾をインスタンス化する関数のインターフェース
// 作成者　　：坂上壱希
// 更新内容　：11/25
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "IBulletFactory.generated.h"

UINTERFACE(BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class PROTOTYPE_API UBulletFactory : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class PROTOTYPE_API IBulletFactory
{
	GENERATED_IINTERFACE_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Transformation")
	virtual void SpawnBullet() = 0;
};

