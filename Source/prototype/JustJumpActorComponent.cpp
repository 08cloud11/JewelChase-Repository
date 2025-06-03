//--------------------------------------------------------------------
// ファイル名	：JustJumpActorComponent.cpp
// 概要　　　	：ジャストジャンプ
// 作成者　　	：神野琉生
// 更新内容　	：1/27 作成
//				  1/29 LineTraceSingleByObjectTypeを使ったレイキャストの作成（動作未確認）
//				  2/1  処理完成（未調整、未実装）
//                2/11 処理完成（未調整）
//                2/23 調整完了
//--------------------------------------------------------------------

// インクルード
#include "JustJumpActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"			// トレースのデバッグの時に必要なヘッダーファイル

// プロパティ変更を保持する関数
#if WITH_EDITOR
void UJustJumpActorComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// プロパティが変更されたことをエディタに通知
	Modify();  // これで変更履歴が記録される
}
#endif

UJustJumpActorComponent::UJustJumpActorComponent()
	: MyEJumpStatus(EJumpStatus::EmptyJump)
	, _ownerActor(nullptr)
	, _halfBoxSize(FVector::ZeroVector)
	, _distancePlayer(50.0f)
	, _rayEnd(200.0f)
	, _rayBoxHeight(0.0f)
	, _minJustJumpDistance(0.0f)
	, _maxJustJumpDistance(0.0f)
{
	PrimaryComponentTick.bCanEverTick = true;

	// 動的配列(TArray)の初期化
	_itemsActor.Reset();
	_smogsActor.Reset();
}

void UJustJumpActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// このスクリプトがついているActorを取得
	_ownerActor = GetOwner<AActor>();

	// レイキャストで無視するアクターを検索・保持
	FName ItemTag = "Item";
	FName SmogTag = "Smog";
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), ItemTag, _itemsActor);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), SmogTag, _smogsActor);
	// レイキャストで無視する物を設定
	_ignoreParams.AddIgnoredActor(_ownerActor);      // 自分自身を無視
	_ignoreParams.AddIgnoredActors(_smogsActor);	 // 霧のActor
	_ignoreParams.AddIgnoredActors(_itemsActor);	 // アイテムのActor

	// レイキャストで取得するチャンネルを設定
	TArray<ECollisionChannel> ObjectChannels = {
		ECollisionChannel::ECC_WorldDynamic,
		ECollisionChannel::ECC_WorldStatic
	};
	// すべてのチャンネルを取得するチャンネルに追加
	for (const ECollisionChannel& Channel : ObjectChannels)
	{
		_objectQueryParams.AddObjectTypesToQuery(Channel);
	}
}

void UJustJumpActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

//----------------------------------
// 関数名：JustJumpRay
// 引　数：なし
// 戻り値：EJumpStatus型
// ジャストジャンプか判断するレイの作成
//----------------------------------
EJumpStatus UJustJumpActorComponent::JustJumpRay()
{
	// ジャンプの列挙型を空のジャンプにする
	MyEJumpStatus = EJumpStatus::EmptyJump;

	// 無視するパラメータにワールドにあるアイテムを追加
	AddItemToIgnore();

	if (!_ownerActor)
	{
		return MyEJumpStatus;	// 早期リターン
	}

	// プレイヤーの距離
	FVector PlayerLocation = _ownerActor->GetActorLocation();
	// プレイヤーの回転
	FRotator PlayerRotation = _ownerActor->GetActorRotation();
	// レイキャストの高さを設定
	PlayerLocation.Z += _rayBoxHeight;
	// ボックスの始点
	FVector TraceStart = PlayerLocation + PlayerRotation.Vector() * _distancePlayer;
	// ボックスの終点
	FVector TraceEnd = TraceStart + PlayerRotation.Vector() * _rayEnd;

	// レイキャストを実行
	FHitResult Hit;
	GetWorld()->SweepSingleByObjectType(
		OUT Hit,									  // 当たったもの
		TraceStart,								      // 始点座標
		TraceEnd,									  // 終点座標
		FQuat::Identity,							  // トレースの形状の回転
		_objectQueryParams,							  // 衝突チャネル
		FCollisionShape::MakeBox(_halfBoxSize),       // トレースの形状を指定
		_ignoreParams								  // 無視するアクターの設定
	);

#if false
	// ジャストジャンプのパラメータの値をログに出力
	UE_LOG(LogTemp, Warning, TEXT("Params.HalfBoxSize: %s"), *_halfBoxSize.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Params.DistancePlayer: %f"), _distancePlayer);
	UE_LOG(LogTemp, Warning, TEXT("Params.RayBoxHeight: %f"), _rayBoxHeight);
	UE_LOG(LogTemp, Warning, TEXT("Params.MinJustJumpDistance: %f"), _minJustJumpDistance);
	UE_LOG(LogTemp, Warning, TEXT("Params.MaxJustJumpDistance: %f"), _maxJustJumpDistance);

	// レイキャストの範囲をデバッグ用に表示
	DrawDebugLine(
		GetWorld(),          // 描画を行うワールド
		TraceStart,          // 始点
		TraceEnd,            // 終点
		FColor::Green,       // 線の色
		false,               // true: 永続的に描画 / false: 一定時間で消える
		1.0f,                // 描画が消えるまでの時間（-1.0f ならフレームごとに更新）
		0,                   // 描画の優先度（0: 通常, 1: 上位）
		1.0f                 // 線の太さ
	);

	//デバッグ用のRayBoxの表示
	DrawDebugBox(
		GetWorld(),				          // 描画を行うワールド
		(TraceStart + TraceEnd) / 2.0f,   // ボックスの中心座標
		_halfBoxSize,		              // ボックスの半径（半分のサイズ）
		FQuat::Identity,		          // ボックスの回転
		FColor::Red,			          // ボックスの色
		false,					          // true: 永続的に描画 / false: 一定時間で消える
		1.0f,					          // 描画が消えるまでの時間（-1.0f ならフレームごとに更新）
		0,						          // 描画の優先度（0: 通常, 1: 上位）
		2.0f					          // ボックスの線の太さ
	);

	// ジャストジャンプの中心座標
	FVector JustJumpCenter = PlayerLocation + PlayerRotation.Vector() * (_distancePlayer + (_maxJustJumpDistance - _minJustJumpDistance) / 2.0f);

	// ジャストジャンプの範囲のデバッグ用のRayBoxの表示
	DrawDebugBox(
		GetWorld(),				// 描画を行うワールド
		JustJumpCenter,		    // ボックスの中心座標
		_halfBoxSize,	        // ボックスの半径（半分のサイズ）
		FQuat::Identity,		// ボックスの回転
		FColor::Blue,			// ボックスの色
		false,					// true: 永続的に描画 / false: 一定時間で消える
		1.0f,					// 描画が消えるまでの時間（-1.0f ならフレームごとに更新）
		0,						// 描画の優先度（0: 通常, 1: 上位）
		2.0f					// ボックスの線の太さ
	);

	// ジャストジャンプの始点
	FVector JustJumpRayStart = PlayerLocation + PlayerRotation.Vector() * (_distancePlayer + _minJustJumpDistance);
	// ジャストジャンプの終点
	FVector JustJumpRayEnd = TraceStart + PlayerRotation.Vector() * _maxJustJumpDistance;

	// レイキャストの範囲をデバッグ用に表示
	DrawDebugLine(
		GetWorld(),          // 描画を行うワールド
		JustJumpRayStart,    // 始点
		JustJumpRayEnd,      // 終点
		FColor::Blue,        // 線の色
		false,               // true: 永続的に描画 / false: 一定時間で消える
		1.0f,                // 描画が消えるまでの時間（-1.0f ならフレームごとに更新）
		0,                   // 描画の優先度（0: 通常, 1: 上位）
		1.0f                 // 線の太さ
	);
#endif

	// Actorとヒットしたら行う処理
	if (Hit.bBlockingHit == true && Hit.GetActor())
	{
		// ヒットしたものからActorの取得
		const AActor* HitActor = Hit.GetActor();

		if (HitActor == nullptr)
		{
			MyEJumpStatus = EJumpStatus::EmptyJump;	// 早期リターン
			return MyEJumpStatus;
		}

		if (HitActor->ActorHasTag(FName("Enemy")))
		{
			// TraceStartからLocationまでの距離
			float HitDistance = Hit.Distance;

			if (_minJustJumpDistance <= HitDistance && HitDistance <= _maxJustJumpDistance)
			{
				MyEJumpStatus = EJumpStatus::JustJump;    // ジャストジャンプ
			}
			else
			{
				MyEJumpStatus = EJumpStatus::NormalJump;  // 通常のジャンプ
			}
		}
	}
	else
	{
		// 何にもヒットしていない場合
	}

	return MyEJumpStatus;
}

//----------------------------------
// 関数名：AddItemToIgnore
// 引　数：なし
// 戻り値：なし
// ワールドにあるアイテムを無視するリストに追加
//----------------------------------
void UJustJumpActorComponent::AddItemToIgnore()
{
	// 動的配列(TArray)の初期化
	_itemsActor.Reset();

	// レイキャストで無視するアクターを検索・保持
	FName ItemTag = "Item";
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), ItemTag, _itemsActor);
	_ignoreParams.AddIgnoredActors(_itemsActor);
}