//--------------------------------------------------------------------
// ファイル名：IMovement
// 概要　　　：MoveRight関数が入っているインターフェース
// 作成者　　：坂上壱希
// 更新内容　：11月25日
//--------------------------------------------------------------------

#include "IMovement.h"

UMovement::UMovement(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void IMovement::Move(float moveSpeed)
{
}