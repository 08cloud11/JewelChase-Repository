//--------------------------------------------------------------------
// ファイル名：IMovement
// 概要　　　：IsDamaged関数が置いてあるインターフェイス
// 作成者　　：坂上壱希
// 更新内容　：11月29日
//--------------------------------------------------------------------

#include "IDamaged.h"

UDamaged::UDamaged(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void IDamaged::IsDamaged(int damage)
{
}
