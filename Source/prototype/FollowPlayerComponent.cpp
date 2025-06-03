//--------------------------------------------------------------------
// ファイル名：FollowPlayerComponent.h
// 概要　　　：プレイヤーを追従するコンポーネント
// 作成者　　：神野琉生
// 更新内容　：1/17 作成
//				    プレイヤーを追従する動作に関する変数と関数の作成（未完成）
//                  演出中かを管理するフラグの変更を行う関数の定義
//             1/19 演出中かを管理するフラグを逆にする関数の内容を変更
//                  プレイヤーを追従させる処理の関数の内容を変更
//--------------------------------------------------------------------

// インクルード
#include "FollowPlayerComponent.h"
#include "Kismet/GameplayStatics.h"

UFollowPlayerComponent::UFollowPlayerComponent()
	: _playerPawn(nullptr)
	, _isDirectionFlag(true)
{
	PrimaryComponentTick.bCanEverTick = true;
}

UFollowPlayerComponent::~UFollowPlayerComponent()
{
}

void UFollowPlayerComponent::BeginPlay()
{
	Super::BeginPlay();

	// このコンポーネントがついているActorの座標を変数に格納する
	_currentLocation = GetOwner()->GetActorLocation();

	// ターゲットを設定する
	SetPlayerPawn();
}

void UFollowPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// プレイヤーを追従する処理
	if (_isDirectionFlag == false)
	{
		FollowPlayer();
	}
}

//----------------------------------
// 関数名：ChangeIsDirectionFlag
// 引　数：なし
// 戻り値：なし
// 演出中かを管理するフラグを逆にする関数
//----------------------------------
void UFollowPlayerComponent::ChangeIsDirectionFlag()
{
	_isDirectionFlag = !_isDirectionFlag;
}

//----------------------------------
// 関数名：SetPlayerPawn
// 引　数：なし
// 戻り値：なし
// プレイヤーのPawnを取得する関数
//----------------------------------
void UFollowPlayerComponent::SetPlayerPawn()
{
	// プレイヤーのPawnを取得する
	_playerPawn = UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0);

	// プレイヤーの座標を格納する
	if (_playerPawn != nullptr)
	{
		_playerLocation = _playerPawn->GetActorLocation();
	}
}


//----------------------------------
// 関数名：FollowPlayer
// 引　数：なし
// 戻り値：なし
// プレイヤーを追従させる処理の関数
//----------------------------------
void UFollowPlayerComponent::FollowPlayer()
{
	if (_playerPawn == nullptr)
	{
		// プレイヤーのPawnを取得する関数
		SetPlayerPawn();
		return;
	}

	// プレイヤーの座標を格納する
	_playerLocation = _playerPawn->GetActorLocation();

	// 新しい座標を作成する
	FVector TargetLocation = FVector(_currentLocation.X, _playerLocation.Y, _currentLocation.Z);

	// 現在の座標から目標座標に補間して新しい座標を計算
	FVector NewLocation = FMath::VInterpTo(
		_currentLocation,					// 現在の座標
		TargetLocation,						// 目標座標
		GetWorld()->GetDeltaSeconds(),		// フレームごとのデルタ時間
		_cameraMoveSpeed					// 補間速度
	);

	if (GetOwner())
	{
		// 新しい座標を現在の座標に格納
		_currentLocation = NewLocation;
		// 座標を更新する
		GetOwner()->SetActorLocation(NewLocation);
	}
}

