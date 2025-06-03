//--------------------------------------------------------------------
// ファイル名：IMovement
// 概要　　　：MoveRight関数が置いてあるインターフェイス
// 作成者　　：坂上壱希
// 更新内容　：11/25
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "IDead.generated.h"

UINTERFACE(BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class PROTOTYPE_API UDead : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class PROTOTYPE_API IDead
{
	GENERATED_IINTERFACE_BODY()

public:
	//UFUNCTION(BlueprintCallable, Category = "Transformation")
	virtual void Dead() = 0;
};
