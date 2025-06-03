//--------------------------------------------------------------------
// ファイル名：predectionLine.h
// 概要　　　：予測線のActorクラス
// 作成者　　：山内　将吾
// 更新内容　：2024/12/15	クラスの作成
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PredictionLine.generated.h"

// 前方宣言
class USplineComponent;
class USplineMeshComponent;

UCLASS()
class PROTOTYPE_API APredictionLine : public AActor
{
	GENERATED_BODY()

protected:
	// ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:
	// コンストラクタ
	APredictionLine();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UStaticMesh* LineStaticMesh;	// UE側でスタティックメッシュ設定
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UMaterialInterface* LineMaterial;	// UE側でマテリアル設定

	// 線を更新する関数
	UFUNCTION(BlueprintCallable, Category = "Line")
	void UpdateLine(const FVector& StartPoint, const FVector& EndPoint);

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USplineComponent* _pSplineComponent;	// スプラインコンポーネント
	UPROPERTY()
	USplineMeshComponent* _pSplineMeshComponent;  // スプラインメッシュコンポーネントを格納するポインタ

	FVector _splineStartPoint;	// Splineの始点
	FVector _splineEndPoint;		// Splineの終点
};
