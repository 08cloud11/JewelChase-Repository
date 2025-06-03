//--------------------------------------------------------------------
// ファイル名 ：InGameModeBase.h
// 概要       ：インゲーム画面のゲームモードを設定する
// 作成者     ：0231本間
// 更新内容   ：12/03　作成
//--------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API AInGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AInGameModeBase();
	void StartPlay();
};
