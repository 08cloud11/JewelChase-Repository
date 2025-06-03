//--------------------------------------------------------------------
// ファイル名：PlayerHitByPoliceCarComponent.cpp
// 概要　　　：プレイヤーがパトカーに当たった際に弾く処理を行うためのコンポーネント
// 作成者　　：山内　将吾
// 更新内容　：2024/02/01	クラスの作成
//--------------------------------------------------------------------

#include "prototype/Player/PlayerComponent/PlayerHitByPoliceCarComponent.h"

// コンストラクタ
UPlayerHitByPoliceCarComponent::UPlayerHitByPoliceCarComponent()
	: _playerActor(nullptr)
	, _policeCarActor(nullptr)
	, _playerLocation(0.0f,0.0f,0.0f)
	, _relativeLocation(0.0f,0.0f,0.0f)
	, _knockbackDirection(0.0f,0.0f,0.0f)
	, _crossProductZ(0.0f)
	, _timer(0.0f)
	, _playerMaxLocationY(0.0f)
	, _playerMinLocationY(0.0f)
	, _isHitPoliceCar(false)
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
	// 必要に応じて、パフォーマンス向上のために切ることもできる。
	PrimaryComponentTick.bCanEverTick = true;
}

// ゲームスタート時、または生成時に呼ばれる処理
void UPlayerHitByPoliceCarComponent::BeginPlay()
{
	Super::BeginPlay();
}

// 毎フレーム処理
void UPlayerHitByPoliceCarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// デバッグ用
	//UE_LOG(LogTemp, Log, TEXT("_pPlayerHitByPoliceCarComponent Tick"));

	// フラグが上がるまで点滅させる
	if (_isHitPoliceCar)
	{
		//時間計測
		_timer += DeltaTime;
		if (this->_timer < _knockbackDuration)
		{
			//UE_LOG(LogTemp, Log, TEXT("KnockBacking"));	// デバッグ用
			_knockBack();
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Knockback is over"));
			_isHitPoliceCar = false;
			//PrimaryComponentTick.bCanEverTick = false;
		}		
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("false _isHitPoliceCar"));	
	}
}

// 関数名：ClampPlayerYLocation
// 引数：playerMaxLocationY：プレイヤーのY座標の最大値, playerMinLocationY：プレイヤーのY座標の最小値
// 戻り値：なし
// プレイヤーのY座標の制限値を取得する関数
void UPlayerHitByPoliceCarComponent::ClampPlayerYLocation(float playerMaxLocationY, float playerMinLocationY)
{
	// 制限値を取得
	_playerMaxLocationY = playerMaxLocationY;
	_playerMinLocationY = playerMinLocationY;
}

// 関数名：TriggerKnockBack
// 引数： playerLocation：プレイヤーアクター, policeCarActor：当たったパトカーアクター
// 戻り値：なし
// 内容：弾きのトリガーをONにする関数
void UPlayerHitByPoliceCarComponent::TriggerKnockBack(AActor* playerActor, AActor* policeCarActor)
{
	// デバッグ用
	//UE_LOG(LogTemp, Log, TEXT("_isHitPoliceCar ON"));
	this->_timer = 0.0f;	// タイマーの初期化
	this->_isHitPoliceCar = true;				// 弾かれフラグをON

	_playerActor = playerActor;	// プレイヤーの座標を取得する
	_policeCarActor = policeCarActor;	// 当たったパトカーを取得する
	_calculateKnockbackDirection();		// ノックバックする方向を計算する
}

// 関数名：_CalculateKnockbackDirection
// 引数：なし
// 戻り値：なし
// 内容：ノックバックする方向を計算する関数
void  UPlayerHitByPoliceCarComponent::_calculateKnockbackDirection()
{
	if (!_playerActor) { return; }		// プレイヤーが存在するかチェック
	if (!_policeCarActor) { return; }	// パトカーが存在するかチェック

	// パトカーの座標、前方向ベクトルを取得
	FVector _policeCarLocation = _policeCarActor->GetActorLocation();	// パトカーの座標
	FVector _policeCarForwardVector = _policeCarActor->GetActorForwardVector(); // パトカーの前方向ベクトル

	// プレイヤーの座標を取得
	_playerLocation = _playerActor->GetActorLocation();

	// プレイヤーとパトカーの相対位置を計算
	_relativeLocation = _playerLocation - _policeCarLocation;

	// 相対位置と右方向ベクトルの外積を計算
	float _crossProduct = FVector::CrossProduct( _policeCarForwardVector, _relativeLocation ).Z;

	// 外積のZ成分を使って右か左かを判断
	if (_crossProduct > 0.0f)
	{
		// 外積のZ成分が正なら、プレイヤーはパトカーの「右側」にいる
		_knockbackDirection = _policeCarActor->GetActorRightVector();	// 右側に弾く
		//UE_LOG(LogTemp, Log, TEXT("knockback direction is RIGHT"));
	}
	else
	{
		// 外積のZ成分が負なら、プレイヤーはパトカーの「左側」にいる
		_knockbackDirection = -_policeCarActor->GetActorRightVector();	// 左側に弾く
		//UE_LOG(LogTemp, Log, TEXT("knockback direction is LEFT"));
	}

	// 弾く先に壁がある場合逆方向に弾く
	if (_isWallInDirection(_policeCarLocation))
	{
		_knockbackDirection *= -1;
		//UE_LOG(LogTemp, Log, TEXT("There was a wall in the knockback direction."));
	}
}

// 関数名：_isWallInDirection
// 引数：パトカーの座標
// 戻り値：逆方向に変えるかどうかのフラグ (true：変える / false：変えない)
// 弾く先に壁があるか確かめる関数
bool UPlayerHitByPoliceCarComponent::_isWallInDirection(const FVector policeCarLocation)
{
	// 弾いた先のポジションYを計算
	float _moveTargetLocationY = policeCarLocation.Y + (_knockbackDirection * _knockbackCheckDistancef).Y; // 指定したユニット先をチェック

	// プレイヤーの制限範囲を出ていたらtrueを返す 
	return (_moveTargetLocationY < _playerMinLocationY || _moveTargetLocationY > _playerMaxLocationY);
}

// 関数名：_knockBack
// 引数：なし
// 戻り値：なし
// 内容：パトカーにプレイヤーが当たった際に弾く関数
void UPlayerHitByPoliceCarComponent::_knockBack()
{
	// デバッグ用
	//UE_LOG(LogTemp, Log, TEXT("KnockBack execution"));	// デバッグ用	
	//UE_LOG(LogTemp, Log, TEXT("_knockbackDirection  is : %s"), _knockbackDirection);

	if (_playerActor != nullptr && _policeCarActor != nullptr)
	{
		// 弾き飛ばす前の始点
		FVector  _initialKnockbackLocation = FVector(_playerActor->GetActorLocation().X, _policeCarActor->GetActorLocation().Y, _playerActor->GetActorLocation().Z);
		// ノックバック先の位置を計算
		FVector _knockbackTargetLocation = _initialKnockbackLocation + (_knockbackDirection * _knockBackStrength);
		// 決まった方向に指定した強さでノックバックさせる
		_playerActor->SetActorLocation(_knockbackTargetLocation);
	}
}
