//--------------------------------------------------------------------
//制作者　：坂上　壱希
//制作日時：2024/11/22
//最終更新：2025/02/10
//概　要　：プレイヤーのベース部分
//--------------------------------------------------------------------
#include "PlayerCharacter.h"
#include "BlinkingComponent.h"
#include "Player/PlayerComponent/PlayerHitByPoliceCarComponent.h"
#include "JustJumpActorComponent.h"
#include "Blueprint/UserWidget.h"  // UUserWidgetのインクルード
#include "UI/JJPerfectWidget.h"
#include "Kismet/GameplayStatics.h"

#define JUMP_HEIGHT (_jumpTime * _jumpTime * (-_gravity) / 2) + (_jumpTime * _jumpPower)

class UPrimitiveComponent;
class APlayerController;
class UJJPerfectWidget;

APlayerCharacter::APlayerCharacter()
	: MyStatus(PlayerStatus::Running)
	, _beforeJumpPos(FVector::ZeroVector)
	, _jumpTime(0.0f)
	, _initialLimitBulletNum(_limitBulletNum)
	, _initialBulletAttack(_bulletAttack)
	, _isJumping(false)
	, _canControl(true)
	, _isReload(false)
	, _isJewelRush(false)
	, _isInvincible(false)
	, _isAttack(false)
	, _isJumpRigor(false)
	, _isBlinking(false)
{
	PrimaryActorTick.bCanEverTick = true;

	_pPlayerHitByPoliceCarComponent = CreateDefaultSubobject<UPlayerHitByPoliceCarComponent>(TEXT("_pPlayerHitByPoliceCarComponent"));	//　警察の当たったの処理を行うためのコンポーネント 
	_pBlinkingComponent = CreateDefaultSubobject<UBlinkingComponent>(TEXT("_pBlinkingComponent"));	// 点滅させるためのコンポーネント
	_pJustJumpActorComponent = CreateDefaultSubobject<UJustJumpActorComponent>(TEXT("_pJustJumpActorComponent"));	//　JunstJumpの処理を行うためのコンポーネント

	_originalMaterial0 = nullptr;
	_originalMaterial1 = nullptr;

	_capsuleComponent = GetCapsuleComponent();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	/*
	if (_playerDataTable)
	{
		TArray<FName> RowNames = _playerDataTable->GetRowNames();
		_data = *(_playerDataTable->FindRow<FPlayerSE>(RowNames[0], FString()));
	}
	*/

	//SEManagerを取得
	_seManager = USEManager::GetInstance(this);

	RegisterSE();

	//USEManager::GetInstance()->RegisterSE(TEXT("Attack"), AttackSE);

	if (GetMesh())
	{
		_originalMaterial0 = GetMesh()->GetMaterial(0);
		_originalMaterial1 = GetMesh()->GetMaterial(1);
	}

	// コンポーネントの登録・プレイヤーの可動範囲を設定する
	if (_pPlayerHitByPoliceCarComponent)
	{
		_pPlayerHitByPoliceCarComponent->ClampPlayerYLocation(_maxPosY, _minPosY);	// コンポーネントにプレイヤーの可動範囲を設定
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("No _pPlayerHitByPoliceCarComponent"));
	}

	//カーソルの表示
	ShowMouseCursor();

#if true
	//イベントのデリゲートをバインド
	if (UPrimitiveComponent* PrimitiveRoot = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		PrimitiveRoot->SetNotifyRigidBodyCollision(true);
		PrimitiveRoot->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		// Overlap イベントをバインド
		PrimitiveRoot->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	}
#endif
}

void APlayerCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	//PlayerのY座標を制限
	PlayerPosControl();

	//マウスの移動範囲を制限
	MouseRangeControl();

	FVector myPos = GetActorLocation();
	myPos.X = 0.0f;
	//X軸を0に固定
	SetActorLocation(myPos);

	if (MyStatus == PlayerStatus::Die)
	{
		return;
	}

	//ジャンプ中
	UpdateJump(deltaTime);

	//リロード条件
	if (_limitBulletNum <= 0u && !_isReload)
	{
		ReloadBulletTimer();
	}

	//死亡条件
	if (_hp <= 0u)
	{
		Dead();
	}

	//点滅条件
	if (_onBlinking)
	{
		_toggleTime -= deltaTime;

		if (_toggleTime <= 0.0f)
		{
			//点滅処理
			ToggleMaterial();
			_toggleTime = _initialToggleTime;
		}
	}
	if (MyStatus == PlayerStatus::Uncontroll)
	{
		//RunSEを停止
		if (_seMap["Run"] != nullptr)
		{
			_seManager->StopSE(TEXT("Run"));
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

	if (!_canControl)
	{
		return;
	}

	//左右移動
	if (!_isJumping)
	{
		playerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::Move);
	}

	playerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::JumpStart);
	//長押しで射撃できないwhy?↓
	//playerInputComponent->BindAction("Click", IE_Repeat, this, &APlayerCharacter::SpawnBullet);
	playerInputComponent->BindAction("Click", IE_Pressed, this, &APlayerCharacter::SpawnBullet);
	playerInputComponent->BindAction("JewelRush", IE_Pressed, this, &APlayerCharacter::JewelRushStart);
}

// BeginOverlap イベント
void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Log, TEXT("Player Hit %s"),*OtherActor->GetName());
	if (!OtherActor)
	{
		return;
	}
	//Enemyに当たったら処理
	if (OtherActor->ActorHasTag("Enemy") || OtherActor->ActorHasTag("Oil_Explosion"))
	{
		//1ダメージ受ける
		IsDamaged(1);

		if (OtherActor->ActorHasTag("PoliceCar") && !_isInvincible && !_isJewelRush)
		{
			if (_pPlayerHitByPoliceCarComponent)
			{
				//UE_LOG(LogTemp, Log, TEXT("Player Hit PoliceCar"));
				// ノックバック処理
				_pPlayerHitByPoliceCarComponent->TriggerKnockBack(this, OtherActor);

			}
		}

#if false
		UKismetSystemLibrary::PrintString(
			this,                            // コンテキストオブジェクト（通常は`this`）
			OtherActor->GetName(),			 // 表示する文字列
			true,                            // スクリーンに表示するか
			true,                            // コンソールに表示するか
			FColor::Red,                     // 文字の色
			2.0f                             // 表示時間（秒）
		);
#endif 
		return;
	}

	//配列の長さを取得
	const int length = sizeof(_jewelRushGageIncrementNum) / sizeof(_jewelRushGageIncrementNum[0]);

	for (int i = 0; i < length; i++)
	{
		//jewel系のtagだったら処理
		if (OtherActor->ActorHasTag(_jewelName[i]))
		{
			if (_seMap["Get_Jewel"] != nullptr)
			{
				_seManager->PlaySE2D(TEXT("Get_Jewel"));
			}

#if true
			if (_isJewelRush)
			{
				return;
			}
#endif

			//ジュエルラッシュゲージに加算
			_jewelRushGageNum += _jewelRushGageIncrementNum[i];

			//Maxより大きければMaxに補正
			if (_jewelRushGageNum > _jewelRushGageMaxNum)
			{
				_jewelRushGageNum = _jewelRushGageMaxNum;
			}

			return;
		}
	}

#if false
	//デバック用関数
	//当たったActorを表示
	UKismetSystemLibrary::PrintString(
		this,                            // コンテキストオブジェクト（通常は`this`）
		OtherActor->GetName(),			 // 表示する文字列
		true,                            // スクリーンに表示するか
		true,                            // コンソールに表示するか
		FColor::Red,                     // 文字の色
		2.0f                             // 表示時間（秒）
	);
#endif

	if (OtherActor->ActorHasTag("Heal"))
	{
		AddHP(1);

		if (_seMap["Get_Heal"] != nullptr)
		{
			_seManager->PlaySE2D(TEXT("Get_Heal"));
		}

		return;
	}
}

#pragma region JumpFunction
void APlayerCharacter::UpdateJump(float deltaTime)
{
	if (!_isJumping)
	{
		return;
	}
	float nowJumpHeight = JUMP_HEIGHT;

	_jumpTime += deltaTime;

	FVector nowPos = GetActorLocation();

	if (nowJumpHeight < 0.0f)
	{
		JumpEnd();
	}
	else
	{
		SetActorLocation(FVector(nowPos.X, nowPos.Y, _beforeJumpPos.Z + nowJumpHeight), true);
	}
}

void APlayerCharacter::JumpStart()
{
	if (MyStatus == PlayerStatus::Uncontroll)
	{
		return;
	}

	if (MyStatus == PlayerStatus::Damaged)
	{
		return;
	}

	if (MyStatus == PlayerStatus::Die)
	{
		return;
	}

	if (!_canControl)
	{
		return;
	}

	if (_isJumpRigor)
	{
		return;
	}

	if (!_isJumping)
	{
		_isJumping = true;

		//RunSEを停止
		if (_seMap["Run"] != nullptr)
		{
			_seManager->StopSE(TEXT("Run"));
		}

		//JumpSEを再生
		if (_seMap["Jump"] != nullptr)
		{
			_seManager->PlaySE2D(TEXT("Jump"));
		}
		if (_seMap["JumpVoice"] != nullptr)
		{
			_seManager->PlaySE2D(TEXT("JumpVoice"));
		}

#if false
		if (_pJustJumpActorComponent->JustJumpRay() == _pJustJumpActorComponent->JumpStatus::JustJump)
		{
			UKismetSystemLibrary::PrintString(
				this,                            // コンテキストオブジェクト（通常は`this`）
				"JustJump",			 // 表示する文字列
				true,                            // スクリーンに表示するか
				true,                            // コンソールに表示するか
				FColor::Green,                     // 文字の色
				2.0f                             // 表示時間（秒）
			);

			ReloadBullet();
		}
#endif // false

		_beforeJumpPos = GetActorLocation();

		FRotator NewRotation = FRotator(0.0f, 90.0f, 0.0f);
		//↓何故かYaw(Z軸回転)だけがゲームに反映されない
		//SetActorRotation(NewRotation);

		GetMesh()->SetWorldRotation(NewRotation);	//代用案

		// ジャストジャンプの処理
		EJumpStatus EJumpStatus = _pJustJumpActorComponent->JustJumpRay();

		if (_limitBulletNum >= 13u || _isReload == true) return;	// 早期リターン

		if (EJumpStatus == EJumpStatus::NormalJump)
		{
			for (int i = 0; i < 3; i++)
			{
				++_limitBulletNum;
			}
		}
		else if (EJumpStatus == EJumpStatus::JustJump)
		{
			for (int i = 0; i < 5; i++)
			{
				++_limitBulletNum;
			}

			// PerfectとUIを表示
			if (WBP_JJPerfectClass)
			{
				// ウィジェットを作成
				UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), WBP_JJPerfectClass);

				if (Widget == nullptr)
				{
					return;  // 早期リターン
				}

				// ウィジェットを画面に追加
				Widget->AddToViewport();

				// キャストしてアニメーションを取得
				UJJPerfectWidget* PerfectWidget = Cast<UJJPerfectWidget>(Widget);
				if (PerfectWidget && PerfectWidget->PerfectAnimation)
				{
					PerfectWidget->PlayAnimation(PerfectWidget->PerfectAnimation);
				}
			}
		}
	}
}

void APlayerCharacter::JumpEnd()
{
	if (_seMap["Jump"] != nullptr)
	{
		_seManager->StopSE(TEXT("Jump"));
	}
	if (_seMap["JumpVoice"] != nullptr)
	{
		_seManager->StopSE(TEXT("JumpVoice"));
	}

	_jumpTime = 0.0f;

	_isJumping = false;

#if true
	_isJumpRigor = true;

	//タイマー用のローカル変数
	FTimerHandle TimerHandle;

	//n秒後に関数を呼び出す
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::Jumprigor, _jumpRigorTime, false);
#endif // 0

	if (MyStatus == PlayerStatus::Damaged)
	{
		return;
	}

	MyStatus = PlayerStatus::Running;
}

void APlayerCharacter::Jumprigor()
{
	_isJumpRigor = false;
}
#pragma endregion 

void APlayerCharacter::Move(float AxisValue)
{
	if (_isJumping)
	{
		return;
	}

	if (MyStatus == PlayerStatus::Uncontroll)
	{
		return;
	}

	if (MyStatus == PlayerStatus::Damaged)
	{
		return;
	}

	if (MyStatus == PlayerStatus::Die)
	{
		return;
	}

	MyStatus = PlayerStatus::Running;

	//RunSEを再生
	if (_seMap["Run"] != nullptr)
	{
		_seManager->PlaySE2DLoop("Run");
	}
	//右
	if (AxisValue > 0.0f)
	{
		FRotator NewRotation = FRotator(0.0f, 90.0f + _rotation, 0.0f);
		//↓何故かYaw(Z軸回転)だけがゲームに反映されない
		//SetActorRotation(NewRotation);

		GetMesh()->SetWorldRotation(NewRotation);	//代用案
	}
	//左
	else if (AxisValue < 0.0f)
	{
		FRotator newRotation = FRotator(0.0f, 90.0f + (-_rotation), 0.0f);
		//SetActorRotation(NewRotation);

		GetMesh()->SetWorldRotation(newRotation);
	}
	else
	{
		FRotator newRotation = FRotator(0.0f, 90.0f, 0.0f);
		//SetActorRotation(NewRotation);

		GetMesh()->SetWorldRotation(newRotation);
	}

#if false
	//デバック用関数
	//当たったActorを表示
	UKismetSystemLibrary::PrintString(
		this,                            // コンテキストオブジェクト（通常は`this`）
		FString::FromInt(AxisValue),			 // 表示する文字列
		true,                            // スクリーンに表示するか
		true,                            // コンソールに表示するか
		FColor::Red,                     // 文字の色
		2.0f                             // 表示時間（秒）
	);
#endif

	MyStatus = PlayerStatus::Running;
	// 右方向のベクトルを取得し、その方向に移動
	//現在座標を取得
	FVector mypos = GetActorLocation();
	//現在座標から加算分を計算
	mypos.Y += AxisValue * _moveSpeed;
	//実際の座標に加算
	SetActorLocation(mypos);
}

void APlayerCharacter::PlayerPosControl()
{
	FVector nowLocation = GetActorLocation();
	FVector newLocation = nowLocation;

	if (nowLocation.Y > _maxPosY)
	{
		newLocation.Y = _maxPosY;
		SetActorLocation(newLocation);
	}
	else if (nowLocation.Y < _minPosY)
	{
		newLocation.Y = _minPosY;
		SetActorLocation(newLocation);
	}
}

void APlayerCharacter::IsDamaged(int damage)
{
	if (_isInvincible)
	{
		return;
	}

	if (_isJewelRush)
	{
		return;
	}

	if (MyStatus == PlayerStatus::Damaged)
	{
		return;
	}

	MyStatus = PlayerStatus::Damaged;

	//if (_seMap["Run"] != nullptr)
	//{
	//	_seManager->StopSE("Run");
	//}

	//if (_seMap["Jump"] != nullptr)
	//{
	//	_seManager->StopSE("Jump");
	//}

	_hp -= damage;

	if (_hp > 0)
	{
		//ダメージ音声
		if (_seMap["Damage"] != nullptr)
		{
			_seManager->PlaySE2D("Damage");
		}
		//ダメージ音声
		if (_seMap["DamageVoice"] != nullptr)
		{
			_seManager->PlaySE2D("DamageVoice");
		}
	}
}

void APlayerCharacter::Dead()
{
	if (_hp > 0u)
	{
		return;
	}

	if (_capsuleComponent)
	{
		_capsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//RunSEを停止
	if (_seMap["Run"] != nullptr && MyStatus != PlayerStatus::Die)
	{
		_seManager->StopSE(TEXT("Run"));
	}

	if (_seMap["Dead"] != nullptr)
	{
		_seManager->PlaySE2D(TEXT("Dead"));
	}
	if (_seMap["Damage"] != nullptr)
	{
		_seManager->PlaySE2D(TEXT("Damage"));
	}

	MyStatus = PlayerStatus::Die;

}

void APlayerCharacter::InvincibleTime()
{
	if (MyStatus == PlayerStatus::Die)
	{
		return;
	}

	_isInvincible = true;

#if false
	UKismetSystemLibrary::PrintString(
		this,                            // コンテキストオブジェクト（通常は`this`）
		"InvincibleTime",			 // 表示する文字列
		true,                            // スクリーンに表示するか
		true,                            // コンソールに表示するか
		FColor::Red,                     // 文字の色
		2.0f                             // 表示時間（秒）
	);
#endif

	// 点滅させる
	_pBlinkingComponent->TriggerBlinking(_invincibleTime);

	MyStatus = PlayerStatus::Invincible;

	//タイマー用のローカル変数
	FTimerHandle TimerHandle;

	//n秒後に関数を呼び出す
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::InvincibleTimeEnd, _invincibleTime, false);
}

void APlayerCharacter::InvincibleTimeEnd()
{
	_isInvincible = false;

	MyStatus = PlayerStatus::Running;
}

#pragma region MouseFunction
void APlayerCharacter::ShowMouseCursor()
{
	//if (MyStatus == PlayerStatus::Uncontroll)
	//{
	//	return;
	//}
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	//// カーソルを表示
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true; // カーソルを表示
	}

	//// 必要に応じて入力モードを設定
	//FInputModeGameAndUI InputMode;
	//SetInputMode(InputMode);
}

void APlayerCharacter::MouseRangeControl()
{
	if (MyStatus == PlayerStatus::Uncontroll)
	{
		return;
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	FVector2D nowCursorPos;
	//Cursorの座標を取得
	if (PlayerController->GetMousePosition(nowCursorPos.X, nowCursorPos.Y))
	{
		if (nowCursorPos == FVector2D::ZeroVector)
		{
			return;
		}


		//マウスの座標を制限
		nowCursorPos.X = FMath::Clamp(nowCursorPos.X, _minCursorPos.X, _maxCursorPos.X);
		nowCursorPos.Y = FMath::Clamp(nowCursorPos.Y, _minCursorPos.Y, _maxCursorPos.Y);
		PlayerController->SetMouseLocation(nowCursorPos.X, nowCursorPos.Y);

#if false	//デバック用
		//マウスの座標を表示
		FString debug = FString::Printf(TEXT("X: %.2f, Y: %.2f"), nowCursorPos.X, nowCursorPos.Y);

		UKismetSystemLibrary::PrintString(
			this,                            // コンテキストオブジェクト（通常は`this`）
			debug,			 // 表示する文字列
			true,                            // スクリーンに表示するか
			true,                            // コンソールに表示するか
			FColor::Red,                     // 文字の色
			2.0f                             // 表示時間（秒）
		);
#endif
	}
}
#pragma endregion 

#pragma region BulletFunction
bool APlayerCharacter::GetViewToWorldPos(FVector& location, FVector& direction)
{
	//マウスの座標
	float MouseX, MouseY;
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController == nullptr)
	{
		return false;
	}

	//コンパイルが長すぎるので一旦保留
#if false
	PlayerController->GetMousePosition(MouseX, MouseY);

	FMatrix view;
	FMatrix proj;
	FMatrix vp;
	FMinimalViewInfo MinimalViewInfo;

	auto pCameraMng = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	MinimalViewInfo = pCameraMng->POV;
	UGameplayStatics::GetViewProjectionMatrix(viewInfo, view, proj, vp);

	//マウスの位置
	FVector mousePos(MouseX, MouseY, 0.0f);


	//プロジェクションの逆行列

	//viewの逆行列

#endif

	// マウスの位置を取得
	if (PlayerController->GetMousePosition(MouseX, MouseY))
	{
		// スクリーン座標をワールド座標と方向に変換
		//ScreenX, ScreenY: スクリーン上の位置（X, Y座標）
		//WorldLocation : ワールド空間の基準位置（通常、カメラの位置に近い）
		//WorldDirection : 画面上の点からワールド空間に向かう方向ベクトル
		return PlayerController->DeprojectScreenPositionToWorld(MouseX, MouseY, location, direction);
	}
	return false;
}

void APlayerCharacter::SpawnBullet()
{
	if (MyStatus == PlayerStatus::Uncontroll)
	{
		return;
	}

	if (MyStatus == PlayerStatus::Damaged)
	{
		return;
	}

	if (MyStatus == PlayerStatus::Die)
	{
		return;
	}

	if (_limitBulletNum <= 0u)
	{
		return;
	}

	// 発射位置と向き
	if (PlayerBulletClass)
	{
		//AttackSEを再生
		if (_seMap["Shooting"] != nullptr)
		{
			_seManager->PlaySE2D(TEXT("Shooting"));
		}

		_limitBulletNum--;

		if (_isJewelRush)
		{
			_limitBulletNum = _initialLimitBulletNum;
		}

		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

		if (!PlayerController)
		{
			return;
		}

		FHitResult HitResult;	//当たった物

		AActor* ClickedActor = nullptr;	//クリックしたアクター  山内

		//クリックしたものを取得
		if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
		{
			//アクターを取得
			ClickedActor = HitResult.GetActor();
		}
		//床しか取得できない
		//GetHitResultUnderCursorだとコリジョンプレセットがBlockAllかCustomじゃないと当たらない
		//代用案
		//LineTraceMultiByChannel
		//OverlapMultiByChannel

		//つクリックしたアクターがいればがつクリックしたのがEnemyタグだったら処理
		if (ClickedActor && ClickedActor->ActorHasTag("Enemy"))
		{
#if false	//デバック用
			//マウスの座標を表示

			UKismetSystemLibrary::PrintString(
				this,                            // コンテキストオブジェクト（通常は`this`）
				ClickedActor->GetName(),			 // 表示する文字列
				true,                            // スクリーンに表示するか
				true,                            // コンソールに表示するか
				FColor::Red,                     // 文字の色
				2.0f                             // 表示時間（秒）
			);
#endif

			//PlayerBulletをワールドにスポーン
			APlayerBullet* Bullet = GetWorld()->SpawnActor<APlayerBullet>(PlayerBulletClass);

			if (Bullet) // 山内　（クラッシュ原因直した）
			{
				Bullet->SetActorLocation(GetActorLocation() + _offsetBulletPos);	//山内（中央からトランプが出てくるの直した）

				// クリックしたアクターがワールドに存在する時だけ方向を伝える
				if (IsValid(ClickedActor))Bullet->SetTarget(ClickedActor); //山内（安全面を考えて）
			}
		}

		//クリックしたアクターがいないもしくはクリックしたアクターがEnemyで無ければ処理
		if (!ClickedActor || !ClickedActor->ActorHasTag("Enemy"))
		{
			FVector MuzzleLocation = GetActorLocation() + _bulletOffsetPos;  // 発射位置
			FRotator MuzzleRotation = GetActorRotation();  // 発射角度

			//バレットのターゲットを取得
			FVector location, direction;

			if (GetViewToWorldPos(location, direction)) {
				direction = ((location + (direction * 1000.0f)) - MuzzleLocation).GetSafeNormal();
			}
			else {
				//上手くいかなかった場合はここで何とかする
			}
			//PlayerBulletをワールドにスポーン
			APlayerBullet* Bullet = GetWorld()->SpawnActor<APlayerBullet>(PlayerBulletClass, MuzzleLocation, MuzzleRotation);

			if (!Bullet || direction == FVector::ZeroVector)
			{
				return;
			}

			Bullet->SetDirection(direction + _offsetBulletPos);
		}
	}
}

void APlayerCharacter::ReloadBulletTimer()
{
	if (_isJewelRush)
	{
		return;
	}

	_isReload = true;

	//タイマー用のローカル変数
	FTimerHandle TimerHandle;

#if true
	//Reload_StartSEを再生
	if (_seMap["Reload_Start"] != nullptr)
	{
		_seManager->PlaySE2DLoop(TEXT("Reload_Start"));
	}
#endif // 0

	//n秒後に関数を呼び出す
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::ReloadBullet, _reloadBulletTime, false);
}

void APlayerCharacter::ReloadBullet()
{
#if true
	//Reload_StartSEを停止
	if (_seMap["Reload_Start"] != nullptr)
	{
		_seManager->StopSE(TEXT("Reload_Start"));
	}

	//Reload_EndSEを再生
	if (_seMap["Reload_End"] != nullptr)
	{
		_seManager->PlaySE2D(TEXT("Reload_End"));
	}
#endif // 0

	_limitBulletNum = _initialLimitBulletNum;

	_isReload = false;
}
#pragma endregion 

//左シフトを押したら呼び出される
void APlayerCharacter::JewelRushStart()
{
	//ゲージが溜まっていなかったら処理
	if (_jewelRushGageNum < _jewelRushGageMaxNum)
	{
		return;
	}

	if (MyStatus == PlayerStatus::Uncontroll)
	{
		return;
	}

	if (MyStatus == PlayerStatus::Damaged)
	{
		return;
	}

	if (MyStatus == PlayerStatus::Die)
	{
		return;
	}

	if (_isJewelRush)
	{
		return;
	}

	if (_seMap["JewelRush_Start"] != nullptr)
	{
		_seManager->PlaySE2D(TEXT("JewelRush_Start"));
	}

	//エフェクトを生成
	SpawnParticleEffect(FVector::ZeroVector);

	_isJewelRush = true;

	//残弾数を初期値に変更
	_limitBulletNum = _initialLimitBulletNum;

	_jewelRushGageNum = 0u;

	//マテリアルを変更
	if (GetMesh())
	{
		//エレメント0番目のマテリアルを変更
		if (_jewelRushMaterial0)
		{
			GetMesh()->SetMaterial(0, _jewelRushMaterial0);
		}
		//エレメント1番目のマテリアルを変更
		if (_jewelRushMaterial1)
		{
			GetMesh()->SetMaterial(1, _jewelRushMaterial1);
		}
	}

	JewelRushTime();

}

void APlayerCharacter::JewelRushTime()
{
	if (!_isJewelRush)
	{
		return;
	}

	_bulletAttack = _jewelRushBulletAttack;

	//タイマー用のローカル変数
	FTimerHandle TimerHandle;

	FTimerHandle _blinkStartTimerHandle;

	//n秒後に点滅開始
	GetWorld()->GetTimerManager().SetTimer(_blinkStartTimerHandle, this, &APlayerCharacter::ChangeOnBlinkingFlag, _blinkingStartTime, false);

	//多分タイマーをリセットする
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	//n秒後に関数を呼び出す
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::JewelRushEnd, _jewelRushTime, false);

#if false
	UKismetSystemLibrary::PrintString(
		this,                            // コンテキストオブジェクト（通常は`this`）
		FString::FromInt(_jewelRushTime),			 // 表示する文字列
		true,                            // スクリーンに表示するか
		true,                            // コンソールに表示するか
		FColor::Red,                     // 文字の色
		2.0f                             // 表示時間（秒）
	);
#endif
}

void APlayerCharacter::JewelRushEnd()
{
	if (_seMap["JewelRush_End"] != nullptr)
	{
		_seManager->PlaySE2D(TEXT("JewelRush_End"));
	}

	_isJewelRush = false;

	_onBlinking = false;

	_bulletAttack = _initialBulletAttack;

	//_jewelRushTime = _initialJewelRushTime;

	//マテリアルを元に戻す
	if (GetMesh())
	{
		//エレメント0番目のマテリアルを元に戻す
		if (_originalMaterial0)
		{
			GetMesh()->SetMaterial(0, _originalMaterial0);
		}
		//エレメント1番目のマテリアルを元に戻す
		if (_originalMaterial1)
		{
			GetMesh()->SetMaterial(1, _originalMaterial1);
		}
	}
}

void APlayerCharacter::ToggleMaterial()
{
	_isBlinking = !_isBlinking;

	if (_isBlinking)
	{
		// マテリアルを点滅用のものに変更
		if (_isBlinking)
		{
			GetMesh()->SetMaterial(0, _jewelRushMaterial0);
			GetMesh()->SetMaterial(1, _jewelRushMaterial1);
		}
	}
	else
	{
		// マテリアルを通常のものに戻す
		if (_originalMaterial0 && _originalMaterial1)
		{
			GetMesh()->SetMaterial(0, _originalMaterial0);
			GetMesh()->SetMaterial(1, _originalMaterial1);
		}
	}
}

void APlayerCharacter::ChangeOnBlinkingFlag()
{
	_onBlinking = !_onBlinking;
}

void APlayerCharacter::SpawnParticleEffect(FVector offset)
{
	if (_jewelRushEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			_jewelRushEffect,
			FVector::ZeroVector + offset,
			GetActorRotation()
		);
	}
}

void APlayerCharacter::RegisterSE()
{
	// **USEManager に SE を登録**
	if (_seMap["Jump"] != nullptr)
	{
		_seManager->RegisterSE(TEXT("Jump"), _seMap["Jump"]);
	}
	if (_seMap["Run"] != nullptr)
	{
		_seManager->RegisterSE(TEXT("Run"), _seMap["Run"]);
	}
	if (_seMap["JumpVoice"] != nullptr)
	{
		_seManager->RegisterSE(TEXT("JumpVoice"), _seMap["JumpVoice"]);
	}
	if (_seMap["Damage"] != nullptr)
	{
		_seManager->RegisterSE(TEXT("Damage"), _seMap["Damage"]);
	}
	if (_seMap["Reload_Start"] != nullptr)
	{
		_seManager->RegisterSE(TEXT("Reload_Start"), _seMap["Reload_Start"]);
	}
	if (_seMap["Reload_End"] != nullptr)
	{
		_seManager->RegisterSE(TEXT("Reload_End"), _seMap["Reload_End"]);
	}
	if (_seMap["JewelRush_Start"] != nullptr)
	{
		_seManager->RegisterSE(TEXT("JewelRush_Start"), _seMap["JewelRush_Start"]);
	}
	if (_seMap["JewelRush_End"] != nullptr)
	{
		_seManager->RegisterSE(TEXT("JewelRush_End"), _seMap["JewelRush_End"]);
	}
	if (_seMap["Get_Jewel"] != nullptr)
	{
		_seManager->RegisterSE(TEXT("Get_Jewel"), _seMap["Get_Jewel"]);
	}
	if (_seMap["Get_Heal"] != nullptr)
	{
		_seManager->RegisterSE(TEXT("Get_Heal"), _seMap["Get_Heal"]);
	}
	if (_seMap["Can_JewelRush"] != nullptr)
	{
		_seManager->RegisterSE(TEXT("Can_JewelRush"), _seMap["Can_JewelRush"]);
	}
	if (_seMap["DamageVoice"] != nullptr)
	{
		_seManager->RegisterSE(TEXT("DamageVoice"), _seMap["DamageVoice"]);
	}
	if (_seMap["Shooting"] != nullptr)
	{
		_seManager->RegisterSE(TEXT("Shooting"), _seMap["Shooting"]);
	}
	if (_seMap["Dead"] != nullptr)
	{
		_seManager->RegisterSE(TEXT("Dead"), _seMap["Dead"]);
	}
}