//--------------------------------------------------------------------
// ファイル名	：FollowPlayerComponent.h
// 概要　　　	：プレイヤーを追従するコンポーネント
// 作成者　　	：神野琉生
// 更新内容　	：1/17 作成
//                     プレイヤーを追従する動作で必要な座標を格納する変数と関数の宣言
//                     演出中のフラグを宣言
//				  1/19 カメラの補間速度を宣言
//--------------------------------------------------------------------

// インクルードガード
#pragma once

// インクルード
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FollowPlayerComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROTOTYPE_API UFollowPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFollowPlayerComponent();
	~UFollowPlayerComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//----------------------------------
	// 関数名：ChangeIsDirectionFlag
	// 引　数：なし
	// 戻り値：なし
	// 演出中かを管理するフラグを逆にする関数
	//----------------------------------
	UFUNCTION(BlueprintCallable, Category = "FollowPlayerComponent")
	void ChangeIsDirectionFlag();

	//----------------------------------
	// 関数名：SetPlayerPawn
	// 引　数：なし
	// 戻り値：なし
	// プレイヤーのPawnを取得する関数
	//----------------------------------
	void SetPlayerPawn();

private:
	//----------------------------------
	// 関数名：FollowPlayer
	// 引　数：なし
	// 戻り値：なし
	// プレイヤーを追従させる処理の関数
	//----------------------------------
	void FollowPlayer();

	// プレイヤーのPawn
	UPROPERTY()
	APawn* _playerPawn;

	// このコンポーネントがついているActorの座標
	FVector _currentLocation;
	// プレイヤーの座標
	FVector _playerLocation;

	// カメラの補間速度（この値が小さければ小さいほどゆっくり動く）
	UPROPERTY(EditAnywhere)
	float _cameraMoveSpeed;

	// 演出中なのかを管理するフラグ
	bool _isDirectionFlag;
};