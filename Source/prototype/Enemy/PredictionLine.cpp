
//--------------------------------------------------------------------
// ファイル名：PredectionLine.cpp
// 概要　　　：予測線のActorクラス
// 作成者　　：山内　将吾
// 更新内容　：2024/12/15	クラスの作成
//--------------------------------------------------------------------


#include "PredictionLine.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

// コンストラクタ
APredictionLine::APredictionLine()
	: _pSplineMeshComponent(nullptr)
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
	// 必要に応じて、パフォーマンス向上のために切ることもできる。
	PrimaryActorTick.bCanEverTick = false;

	// Spline Componentの作成
	_pSplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	RootComponent = _pSplineComponent;
	_pSplineComponent->SetMobility(EComponentMobility::Movable);	// 動的に設定
}

// ゲームスタート時、または生成時に呼ばれる処理
void APredictionLine::BeginPlay()
{
	Super::BeginPlay();

	// 生成時の座標を設定
	_pSplineComponent->ClearSplinePoints(true);
	_pSplineComponent->AddSplinePoint(_splineStartPoint, ESplineCoordinateSpace::World, true);
	_pSplineComponent->AddSplinePoint(_splineEndPoint, ESplineCoordinateSpace::World, true);
	_pSplineComponent->UpdateSpline();
}

// 関数名：UpdateLine
// 引数：線を描画する際の 始点、終点
// 戻り値：なし
// 内容：動的に線を描画する関数
void APredictionLine::UpdateLine(const FVector& startPoint, const FVector& endPoint)
{
	if (!_pSplineComponent) { /*UE_LOG(LogTemp, Warning, TEXT("Not SplineComponent"));*/ return; }
	if (!GetOwner()) { return; }
	// デバッグ用
	//UE_LOG(LogTemp, Log, TEXT("hennkougomae　StartPoint: %s, EndPoint: %s"), *StartPoint.ToString(), *EndPoint.ToString());
	//UE_LOG(LogTemp, Log, TEXT("Draw Line"));

	// 座標を更新
	_splineStartPoint = startPoint;
	_splineEndPoint = endPoint;

	// すでにスプラインメッシュがある場合前のスプラインメッシュを壊す
	if (IsValid(_pSplineMeshComponent))
	{
		// コンポーネントを削除
		_pSplineMeshComponent->UnregisterComponent();	 // コンポーネントの登録解除
		_pSplineMeshComponent->DestroyComponent();      // コンポーネントの破棄

		// ポインタをnullptrに設定
		_pSplineMeshComponent = nullptr;
	}

	// スプラインメッシュを生成
	USplineMeshComponent* _pNewSplineMesh = NewObject<USplineMeshComponent>(this);
	_pNewSplineMesh->SetMobility(EComponentMobility::Movable);	// 動的に設定
	_pNewSplineMesh->SetStaticMesh(LineStaticMesh);		// あらかじめ設定したメッシュ
	_pNewSplineMesh->SetMaterial(0, LineMaterial);	// 使用するマテリアル
	_pNewSplineMesh->SetStartAndEnd(_splineStartPoint, FVector::ZeroVector, _splineEndPoint, FVector::ZeroVector, true);

	// コンポーネントを登録
	_pNewSplineMesh->RegisterComponent();

	// 新しいスプラインメッシュを代入
	_pSplineMeshComponent = _pNewSplineMesh;

	// デバッグ用
	//UE_LOG(LogTemp, Log, TEXT("Prediction StartPoint: %s, EndPoint: %s"), *StartPoint.ToString(), *EndPoint.ToString());
}


