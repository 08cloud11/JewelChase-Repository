//--------------------------------------------------------------------
// ファイル名：EnemyBullet.cpp
// 概要　　　：敵の弾クラス
// 作成者　　：山内　将吾
// 更新内容　：2024/12/08	クラスの作成
//--------------------------------------------------------------------

#include "EnemyBullet.h"
#include "Enemy.h"
#include "Components/SphereComponent.h"

// コンストラクタ
AEnemyBullet::AEnemyBullet()
	: _timer(0.0f)
{
    // 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
    // 必要に応じて、パフォーマンス向上のために切ることもできる。
	PrimaryActorTick.bCanEverTick = true;
}

// ゲームスタート時、または生成時に呼ばれる処理
void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();	

	// オーバーラップ用のコンポーネントを取得
	_pSphereComponent = FindComponentByClass<USphereComponent>();

	if (_pSphereComponent)
	{
		// スフィアコリジョンにオーバーラップを設定
		_pSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBullet::OnOverlapBegin);
	}
	else
	{
		// デバッグ用
		//UE_LOG(LogTemp, Warning, TEXT("No _pSphereComponent"))
	}
}

// 毎フレーム処理
void AEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 時間測定
	_timer += DeltaTime;

	// 弾を指定した方向に飛ばす
	SetActorLocation(this->GetActorLocation() + (_direction * _moveSpeed));

	// 指定した時間経過してたら消去
	if (_timeToDisappear <= _timer)
	{
		this->Destroy();
	}
}

// 関数名：SetDirection
// 引数：進む方向
// 戻り値：なし
// 内容：各Enemyから渡された方向を設定する関数
void AEnemyBullet::SetDirection(FVector direction)
{
	// 進む方向の設定
	_direction = direction;
}

// 弾のスピードを設定する関数
void AEnemyBullet::SetBulletSpeed(float _bulletSpeed)
{
	// 弾のスピードを設定
	_moveSpeed = _bulletSpeed;
}

// 関数名：OnBeiginOverLap
// 引数：後に記載
// 戻り値：なし
// 内容：何かしらに当たった瞬間に呼び出される関数
void AEnemyBullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	// デバッグ用
	//UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), OtherActor);
	
	// オーナー以外のアクターとのオーバーラップを確認
	if (OtherActor != GetOwner() && !OtherActor->ActorHasTag("Item"))
	{
		this->Destroy();
		// デバッグ用
		//UE_LOG(LogTemp, Log, TEXT("EnemyBullet Destroy"));
	}
}

