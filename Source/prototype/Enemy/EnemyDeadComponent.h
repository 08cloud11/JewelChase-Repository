//--------------------------------------------------------------------
// ファイル名：EnemyDeadComponent.h
// 概要　　　：死亡するためのコンポーネントクラス
// 作成者　　：山内　将吾
// 更新内容　：2024/11/29	クラスの作成 
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyDeadComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROTOTYPE_API UEnemyDeadComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:
	// コンストラクタ
	UEnemyDeadComponent();
	// 毎フレームの更新処理
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
