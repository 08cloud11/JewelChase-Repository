//--------------------------------------------------------------------
// ファイル名 ：InGameController.cpp
// 概要       ：インゲーム画面のコントローラ
// 作成者     ：0231本間
// 更新内容   ：12/03　作成
//--------------------------------------------------------------------

#include "InGameController.h"

void AInGameController::BeginPlay()
{
    Super::BeginPlay();

    //引数でインプット対象をプレイヤ、UI両方に設定
    SetInputMode(FInputModeGameAndUI());
}