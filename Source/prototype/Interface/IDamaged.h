//--------------------------------------------------------------------
// ファイル名：IDamaged
// 概要　　　：IsDamaged関数が置いてあるインターフェイス
// 作成者　　：坂上壱希
// 更新内容　：11/29
//--------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "IDamaged.generated.h"

UINTERFACE(BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class PROTOTYPE_API UDamaged : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class PROTOTYPE_API IDamaged
{
	GENERATED_IINTERFACE_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Transformation")
	virtual void IsDamaged(int damage) = 0;
};