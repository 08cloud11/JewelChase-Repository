//--------------------------------------------------------------------
//制作者　：坂上　壱希
//制作日時：2024/11/22
//最終更新：2025/02/10
//概　要　：プレイヤーのベース部分
//--------------------------------------------------------------------

#pragma once

#pragma region interface
#include "IMovement.h"
#include "IDamaged.h"
#include "IDead.h"
#include "IBulletFactory.h"
#pragma endregion
#include "SEManager.h"
#include "PlayerBullet.h"
#include "PlayerSEData.h"
#include "Components/CapsuleComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class PlayerStatus : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Running UMETA(DisplayName = "Running"),
	Jumping UMETA(DisplayName = "Jumping"),
	Damaged UMETA(DisplayName = "Damaged"),
	Invincible UMETA(DisplayName = "Invincible"),
	Die UMETA(DisplayName = "Die"),
	Uncontroll UMETA(DisplayName = "Uncontroll"),
	Goal UMETA(DisplayName = "Goal")
};

class UBlinkingComponent;
class UPlayerHitByPoliceCarComponent;
class UJustJumpActorComponent;

UCLASS()
class PROTOTYPE_API APlayerCharacter final : public ACharacter, public IMovement, public IBulletFactory, public IDamaged, public IDead
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:

	virtual void Tick(float deltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;

	//----------------------------------
	// 関数名：OnOverlapBegin
	// 引　数：UPrimitiveComponent* ,AActor* ,UPrimitiveComponent* ,FVector ,FHitResult&
	// 戻り値：void
	// 当たった瞬間に処理する関数
	//----------------------------------
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

#pragma region JumpFunction
	//ゲープロのまま
	void UpdateJump(float deltaTime);
	void JumpStart();
	void JumpEnd();
	//animationが正しく動かないからごり押し
	//ジャンプ後の硬直
	void Jumprigor();
#pragma endregion

	//----------------------------------
	// 関数名：Move
	// 引　数：float
	// 戻り値：void
	// 横移動の関数
	//----------------------------------
	virtual void Move(float moveSpeed) override;

	//----------------------------------
	// 関数名：PlayerPosControl
	// 引　数：無し
	// 戻り値：void
	// 横移動の関数
	//----------------------------------
	void PlayerPosControl();

	//----------------------------------
	// 関数名：IsDamaged
	// 引　数：int
	// 戻り値：void
	// ダメージを受けた際の関数
	//----------------------------------
	/*UFUNCTION(BlueprintCallable, Category = "Transformation")
	virtual void IsDamaged(int damage)override;*/
	void IsDamaged(int damage)override;

	//----------------------------------
	// 関数名：IDead
	// 引　数：int
	// 戻り値：void
	// 死ぬときの処理
	//----------------------------------
	virtual void Dead()override;

	//----------------------------------
	// 関数名：InvincibleTime
	// 引　数：無し
	// 戻り値：void
	// ダメージ後の無敵時間
	//----------------------------------
	UFUNCTION(BlueprintCallable)
	void InvincibleTime();

	void InvincibleTimeEnd();

#pragma region MouseFunction
	//----------------------------------
	// 関数名：SetMouseCursor
	// 引　数：無し
	// 戻り値：void
	// カーソルを表示する関数
	//----------------------------------
	void ShowMouseCursor();

	//----------------------------------
	// 関数名：MouseRangeControl
	// 引　数：無し
	// 戻り値：void
	// カーソルの移動範囲を制御する関数
	//----------------------------------
	void MouseRangeControl();
#pragma endregion

#pragma region BulletFunction
	//----------------------------------
	// 関数名：GetViewToWorldPos
	// 引　数：FVector& ,FVector&
	// 戻り値：bool
	// カーソルのスクリーン座標をワールド座標に変換し返す関数
	//----------------------------------
	bool GetViewToWorldPos(FVector& location, FVector& direction);

	//----------------------------------
	// 関数名：SpawnBullet
	// 引　数：無し
	// 戻り値：void
	// トランプを生成する関数
	//----------------------------------
	virtual void SpawnBullet()override;

	//----------------------------------
	// 関数名：ReloadBulletTimer
	// 引　数：無し
	// 戻り値：void
	// 弾をリロードするまでの処理
	//----------------------------------
	void ReloadBulletTimer();

	//----------------------------------
	// 関数名：ReloadBullet
	// 引　数：無し
	// 戻り値：void
	// 弾をリロードする関数
	//----------------------------------
	UFUNCTION(BlueprintCallable)
	void ReloadBullet();

#pragma endregion 

	//----------------------------------
	// 関数名：JewelRushStart
	// 引　数：無し
	// 戻り値：void
	// ラッシュの最初にしておきたいことを処理する
	//----------------------------------
	void JewelRushStart();

	//----------------------------------
	// 関数名：JewelRushTime
	// 引　数：無し
	// 戻り値：void
	// ラッシュ中
	//----------------------------------
	void JewelRushTime();

	//----------------------------------
	// 関数名：JewelRushEnd
	// 引　数：無し
	// 戻り値：void
	// ラッシュの最後にしておきたいことを処理する
	//----------------------------------
	void JewelRushEnd();

	//----------------------------------
	// 関数名：JewelRushBlinking
	// 引　数：無し
	// 戻り値：void
	// ラッシュ中に点滅させる関数
	//----------------------------------
	void ToggleMaterial();

	//フラグを切り替える関数
	void ChangeOnBlinkingFlag();

	//----------------------------------
	// 関数名：SpawnParticleEffect
	// 引　数：無し
	// 戻り値：void
	// パーティクルを生成する関数
	//----------------------------------
	void SpawnParticleEffect(FVector offset);

	//----------------------------------
	// 関数名：RegisterSE
	// 引　数：無し
	// 戻り値：void
	// SEをSEManagerに登録する関数
	//----------------------------------
	void RegisterSE();

	UFUNCTION(BlueprintCallable, Category = "GetFunctions")
	PlayerStatus GetPlayerStatus()const { return MyStatus; }

	UFUNCTION(BlueprintCallable, Category = "SetFunctions")
	void SetPlayerStatus(PlayerStatus status) { MyStatus = status; }

	UFUNCTION(BlueprintCallable, Category = "GetFunctions")
	float GetReloadBulletTime()const { return _reloadBulletTime; }

	UFUNCTION(BlueprintCallable, Category = "GetFunctions")
	float GetBulletAttack()const { return _bulletAttack; }

	//現在のジュエルゲージを取得
	UFUNCTION(BlueprintCallable, Category = "GetFunctions")
	uint8 GetJewelRushgageNum()const { return _jewelRushGageNum; }
	//ジュエルゲージの最大数を取得
	UFUNCTION(BlueprintCallable, Category = "GetFunctions")
	uint8 GetJewelRushgageMaxNum()const { return _jewelRushGageMaxNum; }

	//残弾数を取得
	UFUNCTION(BlueprintCallable, Category = "GetFunctions")
	uint8 GetLimitBulletNum()const { return _limitBulletNum; }
	//残弾数の初期値を取得
	UFUNCTION(BlueprintCallable, Category = "GetFunctions")
	uint8 GetInitialLimitBulletNum()const { return _initialLimitBulletNum; }

#pragma region HP
	UFUNCTION(BlueprintGetter, Category = "GetFunctions")
	uint8 GetHP()const { return _hp; }
	UFUNCTION(BlueprintSetter, Category = "SetFunctions")
	void SetHP(uint8 hp) {
		if (hp >= _MAX_HP)
		{
			return;
		}
		_hp = hp;
	}

	UFUNCTION()
	void AddHP(uint8 addHp) {
		if ((_hp + addHp) > _MAX_HP)
		{
			return;
		}
		_hp += addHp;
	}
#pragma endregion

	UFUNCTION(BlueprintCallable, Category = "GetFunctions")
	bool GetisJewelRush()const { return _isJewelRush; }

	UFUNCTION(BlueprintCallable, Category = "GetFunctions")
	bool GetIsAttack()const { return _isAttack; }

	UFUNCTION(BlueprintCallable, Category = "GetFunctions")
	bool GetIsJumping()const { return _isJumping; }


	UPROPERTY(BlueprintReadWrite, Category = "PlayerStatus")
	PlayerStatus MyStatus;		//プレイヤーの状態

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	TSubclassOf<class APlayerBullet> PlayerBulletClass;		//生成する弾のクラス

	// JJ時PerfectのときのUI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> WBP_JJPerfectClass;

private:
	/*
	UPROPERTY(EditAnywhere, Category = "Data")
	TSoftObjectPtr<UDataTable> _playerDataTable;	//プレイヤーのSE

	FPlayerSE _data;	//SEデータ
	*/
#pragma region JewelRushMaterial
	// JewelRush中のマテリアル
	UPROPERTY(EditAnywhere, Category = "Materials")
	TObjectPtr<UMaterialInterface> _jewelRushMaterial0;
	UPROPERTY(EditAnywhere, Category = "Materials")
	TObjectPtr<UMaterialInterface> _jewelRushMaterial1;

	// 元のマテリアルを保存する
	UPROPERTY()
	TObjectPtr<UMaterialInterface> _originalMaterial0;
	UPROPERTY()
	TObjectPtr<UMaterialInterface> _originalMaterial1;
#pragma endregion

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPlayerHitByPoliceCarComponent> _pPlayerHitByPoliceCarComponent = nullptr;		//警察の当たったの処理を行うためのコンポーネント

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBlinkingComponent> _pBlinkingComponent = nullptr;		//点滅させるためのコンポーネント

	UPROPERTY(EditAnywhere)
	TObjectPtr<UJustJumpActorComponent> _pJustJumpActorComponent = nullptr;		//ジャストジャンプの処理を行うためのコンポーネント

#pragma region Sound

	//TMap を使って SE を管理
	UPROPERTY(EditAnywhere, Category = "Sound")
	TMap<FName, TObjectPtr<USoundBase>> _seMap;		//SEのマップ

	UPROPERTY()
	TObjectPtr<USEManager> _seManager = nullptr;	//SEManager

	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<UParticleSystem> _jewelRushEffect = nullptr;		//ジュエルラッシュ中のエフェクト
#pragma endregion

	UPROPERTY();
	TObjectPtr<UCapsuleComponent> _capsuleComponent;	//自身のカプセルコンポーネント

	UPROPERTY(EditAnywhere, meta = (AllowPreserveRatio), Category = "Bullet")
	FVector _bulletOffsetPos;	//弾の生成位置
	FVector _beforeJumpPos;		//ジャンプする前の座標

	UPROPERTY(EditAnywhere, Category = "Bullet")
	FVector _offsetBulletPos = FVector(0.10, 0.0, -0.10);	//弾の生成位置

	UPROPERTY(EditAnywhere, meta = (AllowPreserveRatio), Category = "Cursor")
	FVector2D _maxCursorPos = FVector2D(1920.0, 700.0);		//カーソルの最大移動範囲
	UPROPERTY(EditAnywhere, meta = (AllowPreserveRatio), Category = "Cursor")
	FVector2D _minCursorPos = FVector2D(0.0, 0.0);		//カーソルの最小移動範囲

	FName _jewelName[4] = { "Jewel_blue","Jewel_red","Jewel_yellow","Jewel_purple" };	//ジュエルの名前

	UPROPERTY(EditAnywhere, Category = "Move")
	float _rotation = 35.0f;	//移動する際の回転角度Z(Yaw)
	UPROPERTY(EditAnywhere, Category = "Move")
	float _moveSpeed = 7.0f;	//移動速度

	UPROPERTY(EditAnywhere, Category = "Move")
	float _maxPosY = 500.0f;	//最大移動距離
	UPROPERTY(EditAnywhere, Category = "Move")
	float _minPosY = -500.0f;	//最大移動距離

#pragma region Jump
	UPROPERTY(EditAnywhere, Category = "Jump")
	float _jumpPower = 1000.0f;		//ジャンプ力
	float _jumpTime;						//
	UPROPERTY(EditAnywhere, Category = "Jump")
	float _gravity = 2000.0f;	//重力

	UPROPERTY(EditAnywhere, Category = "Jump")
	float _jumpRigorTime = 0.2f;	//ジャンプ後の硬直時間
#pragma endregion	

	UPROPERTY(EditAnywhere)
	float _invincibleTime = 2.0f;	//無敵時間

	UPROPERTY(EditAnywhere, Category = "JewelRush")
	float _jewelRushTime = 10.0f;	//ジュエルラッシュを行う時間
	UPROPERTY(EditAnywhere, Category = "JewelRush")
	float _blinkingStartTime = 7.0f;		//ジュエルラッシュ中の点滅時間
	float _toggleTime = 0.2f;					//点滅の間隔
	const float _initialToggleTime = _toggleTime;	//初期の点滅の間隔

	UPROPERTY(EditAnywhere, Category = "Bullet")
	float _reloadBulletTime = 2.0f;		//弾のリロード時間

	UPROPERTY(EditAnywhere, Category = "Color")
	FColor _color = FColor::Red;

	UPROPERTY(EditAnywhere, Category = "Bullet", Meta = (ClampMin = "0", ClampMax = "5", UIMin = "0", UIMax = "13"))
	uint8 _limitBulletNum = 13u;	//残弾数

	uint8 _initialLimitBulletNum;	//最初の弾の弾数を保持

	UPROPERTY(EditAnywhere, Category = "Bullet")
	uint8 _bulletAttack = 1u;	//弾の攻撃力

	uint8 _initialBulletAttack;		//最初の弾の攻撃力を保持

	UPROPERTY(EditAnywhere, Category = "JewelRush")
	uint8 _jewelRushBulletAttack = 2u;	//ラッシュ中の攻撃力

	uint8 _jewelRushGageNum;					//現在のラッシュゲージ
	UPROPERTY(EditAnywhere, Category = "JewelRush")
	uint8 _jewelRushGageMaxNum = 100u;	//最大ラッシュゲージ数

	uint8 _jewelRushGageIncrementNum[4] = { 1u,5u,10u,30u };	//ジュエルごとのゲージ増加量

#pragma region HP
	//BPでhpのgetとsetができるようになる
	UPROPERTY(EditAnywhere, Category = "HP", BlueprintGetter = GetHP, BlueprintSetter = SetHP, meta = (ClampMin = "0", ClampMax = "5"))
	uint8 _hp = 3u;		//体力
	const uint8 _MAX_HP = 5u;	//最大体力

#pragma endregion

	bool _isJumping;			//ジャンプできるか
	bool _canControl;			//入力できるか
	bool _isReload;				//リロード中か
	bool _isJewelRush;			//ジュエルラッシュ中か
	bool _isInvincible;			//無敵時間中か
	bool _isAttack;				//攻撃中か

	bool _isJumpRigor;			//ジャンプ後の硬直中か
	bool _isBlinking;			//点滅を切り替える
	bool _onBlinking;			//点滅中か
};
