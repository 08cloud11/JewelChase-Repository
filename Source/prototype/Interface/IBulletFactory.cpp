//--------------------------------------------------------------------
// ファイル名：IBulletFactory
// 概要　　　：弾をインスタンス化する関数のインターフェース
// 作成者　　：坂上壱希
// 更新内容　：11/25
//--------------------------------------------------------------------

#include "IBulletFactory.h"

UBulletFactory::UBulletFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void IBulletFactory::SpawnBullet()
{
}