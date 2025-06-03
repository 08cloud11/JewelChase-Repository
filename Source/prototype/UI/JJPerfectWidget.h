// インクルードガード
#pragma once

// インクルード
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JJPerfectWidget.generated.h"

UCLASS()
class PROTOTYPE_API UJJPerfectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// アニメーションを格納する変数
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* PerfectAnimation;
};
