//--------------------------------------------------------------------
// ファイル名	：JustJumpActorComponent.h
// 概要　　　	：ジャストジャンプ
// 作成者　　	：神野琉生
// 更新内容　	：1/27 作成
//				  2/1  処理は完成（未調整、未実装）
//                2/11 処理完成（未調整）
//				  2/23 調整完了
//--------------------------------------------------------------------

// インクルードガード
#pragma once

// インクルード
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JustJumpActorComponent.generated.h"

// ジャンプの種類の列挙型
UENUM(BlueprintType)
enum class EJumpStatus : uint8
{
	EmptyJump  UMETA(DisplayName = "EmptyJump"),
	NormalJump UMETA(DisplayName = "NormalJump"),
	JustJump   UMETA(DisplayName = "JustJump"),
	MaxNum     UMETA(Hidden)
};

// ジャストジャンプを行うときに使用する値の構造体
struct JustJumpParams
{
	FVector HalfBoxSize;		// レイキャストのBoxの大きさ
	float DistancePlayer;       // レイを表示するプレイヤーからの距離
	float RayEnd;               // レイの終点
	float RayBoxHeight;			// レイキャストの高さ
	float MinJustJumpDistance;	// ジャストジャンプの最小の距離
	float MaxJustJumpDistance;	// ジャストジャンプの最大の距離
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROTOTYPE_API UJustJumpActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// プロパティ変更を保持する関数
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UJustJumpActorComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ジャンプの種類の列挙型
	EJumpStatus MyEJumpStatus;

	//----------------------------------
	// 関数名：JumpStatusToDisplayName
	// 引　数：EJumpStatus型
	// 戻り値：FString型
	// ジャンプのステータスのUMETAを戻す関数 ※GetDisplayNameTextByValueがint64型なのでint64にキャストしています
	//----------------------------------
	UFUNCTION(BlueprintCallable, Category = "JumpStatus")
	FString JumpStatusToDisplayName(EJumpStatus State)
	{
		return StaticEnum<EJumpStatus>()->GetDisplayNameTextByValue(static_cast<int64>(State)).ToString();
	}

	//----------------------------------
	// 関数名：JustJumpRay
	// 引　数：なし
	// 戻り値：EJumpStatus型
	// ジャストジャンプか判断するレイの作成
	//----------------------------------
	EJumpStatus JustJumpRay();

	//----------------------------------
	// 関数名：AddItemToIgnore
	// 引　数：なし
	// 戻り値：なし
	// ワールドにあるアイテムを無視するリストに追加
	//----------------------------------
	void AddItemToIgnore();

private:
	// 除外するActorのポインター
	UPROPERTY()
	TArray<AActor*> _itemsActor;     // アイテム
	UPROPERTY()
	TArray<AActor*> _smogsActor;     // 霧

	// コンポーネントのついているActor
	UPROPERTY()
	AActor* _ownerActor;

	// レイキャストで取得する物
	FCollisionObjectQueryParams _objectQueryParams;

	// レイキャストで無視する物
	FCollisionQueryParams _ignoreParams;

	// レイキャストのBoxの大きさ
	UPROPERTY(EditAnywhere, Category = "JustJumpRayParams")
	FVector _halfBoxSize;

	// レイを表示するプレイヤーからの距離
	UPROPERTY(EditAnywhere, Category = "JustJumpRayParams")
	float _distancePlayer;
	// レイの終点
	UPROPERTY(EditAnywhere, Category = "JustJumpRayParams")
	float _rayEnd;
	// レイキャストの高さ
	UPROPERTY(EditAnywhere, Category = "JustJumpRayParams")
	float _rayBoxHeight;
	// ジャストジャンプの最小の距離
	UPROPERTY(EditAnywhere, Category = "JustJumpRayParams")
	float _minJustJumpDistance;
	// ジャストジャンプの最大の距離
	UPROPERTY(EditAnywhere, Category = "JustJumpRayParams")
	float _maxJustJumpDistance;
};
