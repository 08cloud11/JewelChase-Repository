//--------------------------------------------------------------------
// ファイル名：IMovement
// 概要　　　：MoveRight関数が置いてあるインターフェイス
// 作成者　　：坂上壱希
// 更新内容　：11/25
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "IMovement.generated.h"

UINTERFACE(BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class PROTOTYPE_API UMovement : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class PROTOTYPE_API IMovement
{
	GENERATED_IINTERFACE_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Move")
	virtual void Move(float moveSpeed) = 0;
};
