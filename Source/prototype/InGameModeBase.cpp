//--------------------------------------------------------------------
// ファイル名 ：InGameModeBase.cpp
// 概要       ：インゲーム画面のゲームモードを設定する
// 作成者     ：0231本間
// 更新内容   ：12/03　作成
//--------------------------------------------------------------------

#include "InGameModeBase.h"
#include "InGameController.h"

AInGameModeBase::AInGameModeBase()
    : Super()
{
    //使用するコントローラをインゲームコントローラに設定
    PlayerControllerClass = AInGameController::StaticClass();

    //DefaultPawnClassを設定
    static ConstructorHelpers::FClassFinder<AActor> Character(TEXT("/Game/0116_sakagami/BP/BP_PlayerCharacter"));
    if (Character.Class != nullptr)
    {
        DefaultPawnClass = Character.Class;
    }
}

void AInGameModeBase::StartPlay()
{
    Super::StartPlay();
}