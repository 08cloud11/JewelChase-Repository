//--------------------------------------------------------------------
// ファイル名 ：BlackOut.cpp
// 概要       ：暗幕のUI
// 作成者     ：0231本間
// 更新内容   ：02/24　作成、
//--------------------------------------------------------------------

#include "BlackOut.h"
#include "Kismet/GameplayStatics.h"

void UBlackOut::NativeConstruct()
{
	Super::NativeConstruct();

	//暗幕の画像を取得し、透明にする
	_blackoutImage = Cast<UImage>(GetWidgetFromName("Image"));
	_blackoutImage->SetRenderOpacity(0.0f);
}

void UBlackOut::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	_time += InDeltaTime;

	_blackoutImage->SetRenderOpacity(_time * 1.5f);

	//次のレベルに遷移
	if (_blackoutImage->GetRenderOpacity() > 2.0f)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(_lvName));
	}
}