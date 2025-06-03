//--------------------------------------------------------------------
// ファイル名：LineControllerComponent.cpp
// 概要　　　：線描画アクターを使うためのコンポーネントクラス
// 作成者　　：山内　将吾
// 更新内容　：2024/12/15	クラスの作成
//--------------------------------------------------------------------

#include "LineControllerComponent.h"
#include "PredictionLine.h"

// コンストラクタ
ULineControllerComponent::ULineControllerComponent()
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
	// 必要に応じて、パフォーマンス向上のために切ることもできる。
	PrimaryComponentTick.bCanEverTick = true;
}

// ゲームスタート時、または生成時に呼ばれる処理
void ULineControllerComponent::BeginPlay()
{
	Super::BeginPlay();
}

// ゲーム終了時、または破棄時に呼ばれる処理
void ULineControllerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// オーナーが破棄されるときのみ処理
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		// 残った予測線を破棄
		DestroyPredictionLineActor();

		// デバッグ確認用
		//UE_LOG(LogTemp, Log, TEXT("Destroy PridictionLineActor!"));
	}
}

// 関数名：UpdateLineActor
// 引数：線を描画する際の 始点、終点
// 戻り値：なし
// 内容：動的に線を描画する関数
void ULineControllerComponent::UpdateLineActor(FVector& Start, FVector& End)
{
	if (_predictionLineFlag) { return; }
	// デバッグ用
	//UE_LOG(LogTemp, Log, TEXT("StartPoint: %s, EndPoint: %s"), *Start.ToString(), *End.ToString());

	// 線を描画する
	if (_pPredictionLineActor != nullptr)
	{
		// オフセットで調整した始点と終点を渡す
		_pPredictionLineActor->UpdateLine(Start +_lineOffset, End);
	}
	else
	{
		// 線描画アクターをスポーンさせて初期化
		SpawnAndInitializeLineActor();
	}
}

// 関数名：SpawnAndInitializeLineActor
// 引数：なし
// 戻り値：なし
// 内容：予測線をレベルに配置・初期化を行う関数
void ULineControllerComponent::SpawnAndInitializeLineActor()
{
	if (!LineActorsClass)  // スポーンするアクタークラスが設定されていない場合は何もしない
	{
		// デバッグ用
		//UE_LOG(LogTemp, Warning, TEXT("LineActorClass is not set!"));
		return;
	}

	// 線描画アクターをスポーン
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner(); // このコンポーネントを持つアクターを所有者とする
	FVector SpawnLocation = GetOwner()->GetActorLocation();	// オーナーの座標にスポーンさせる
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;	// スポーン場所に何があってもスポーンさせる
	_pPredictionLineActor = GetWorld()->SpawnActor<APredictionLine>(LineActorsClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

	// デバッグ用
	//if (_pPredictionLineActor)
	//{
	//	//UE_LOG(LogTemp, Log, TEXT("_pPredictionLineActor Actor: %s"));
	//}
	//else
	//{
	//	//UE_LOG(LogTemp, Error, TEXT("Failed to _pPredictionLineActor!"));
	//}
}

// 関数名：HidePredictionLineActor
// 引数：なし
// 戻り値：なし
// 内容：予測線を非表示にする関数
void ULineControllerComponent::HidePredictionLineActor()
{
	// _pPredictionLineActor が存在したら処理
	if (_pPredictionLineActor)
	{
		// Actorを非表示にする
		_pPredictionLineActor->SetActorHiddenInGame(true);
	}
	else
	{
		// デバッグ用
		//UE_LOG(LogTemp, Error, TEXT("Failed to _pPredictionLineActor!"));
	}
}

// 関数名：DisplayPredictionLineActor
// 引数：なし
// 戻り値：なし
// 内容：予測線を表示にする関数
void ULineControllerComponent::DisplayPredictionLineActor()
{
	// _pPredictionLineActor が存在したら処理
	if (_pPredictionLineActor)
	{
		// Actorを表示にする
		_pPredictionLineActor->SetActorHiddenInGame(false);
	}
	//else
	//{
	//	// デバッグ用
	//	//UE_LOG(LogTemp, Error, TEXT("Failed to PredictionLineActor!"));
	//}
}

// 関数名：DestroyPredictionLineActor
// 引数：なし
// 戻り値：なし
// 内容：予測線のアクターを削除する関数
void ULineControllerComponent::DestroyPredictionLineActor()
{
	if (_pPredictionLineActor != nullptr) // LineActorがなかった場合何もしない
	{
		_pPredictionLineActor->Destroy();
	}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Noting LineActorClass!"));
	//}
}