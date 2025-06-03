//--------------------------------------------------------------------
// ファイル名 ：InGameController.h
// 概要       ：インゲーム画面のコントローラ
// 作成者     ：0231本間
// 更新内容   ：12/03　作成
//--------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InGameController.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API AInGameController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
};
