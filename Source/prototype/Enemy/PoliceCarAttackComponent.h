//--------------------------------------------------------------------
// ファイル名：PoliceCarAttackComponent.h
// 概要　　　：パトカーを制御するクラス
// 作成者　　：山内　将吾
// 更新内容　：2025/01/26	クラスの作成 
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "EnemyAttackComponent.h"
#include "PoliceCarAttackComponent.generated.h"

// 前方宣言
class APoliceCar_Warning;
class UBoxComponent;
class USoundBase;
class USEManager;

UCLASS()
class PROTOTYPE_API UPoliceCarAttackComponent : public UEnemyAttackComponent
{
	GENERATED_BODY()
protected:
	// ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:
	// コンストラクタ
	UPoliceCarAttackComponent();
	// 毎フレーム処理
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// 各敵の攻撃を呼び出す関数
	void EnemyAttack(float _deltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<APoliceCar_Warning> _policeCar_WarningClass;	// 弾の設定 (BP側から弾Actorの指定)

	UPROPERTY()
	USEManager* _seManager;		// サウンドマネージャーインスタンスを取得

	UPROPERTY(EditAnywhere)
	USoundBase* _warningSound;		// パトカーの登場音
	UPROPERTY(EditAnywhere)
	USoundBase* _sirenSound;		// パトカーのサイレン音

	//UPROPERTY()
	AActor* _owner;	// オーナーのアクター

	UPROPERTY()
	UBoxComponent* _pBoxCollisionComponent;		// UEで作成したBoxコリジョンコンポーネントの格納用 

	UPROPERTY(EditAnywhere)
	FVector _warningActorSpawnLocation;	// 警告を出す位置
	UPROPERTY()
	FVector _initialLocation;			// 初期位置
	UPROPERTY(EditAnywhere)
	FVector _forwardTargetLocation;		// 前方の定位置の場所（スポーンした場所から足す分の値）

	UPROPERTY(EditAnywhere)
	float _appearStartTime;	// 登場しだすまでの時間
	UPROPERTY(EditAnywhere)
	float _appearDuration;	// 登場にかかる時間
	UPROPERTY(EditAnywhere)
	float _exitStartTime;	// 退場しだすまでの時間
	UPROPERTY(EditAnywhere)
	float _exitMoveSpeed;	// 退場する際のスピード
	UPROPERTY(EditAnywhere)
	float _exitDuration;	// 退場にかかる時間

	FVector _appearTargetLocation;	// 登場する際の目標位置

	float _currentSpeed;	// 現在の移動速度
	float _timer;			// タイマー(万能タイマー)
	float _exitTimer;		// タイマー(退場用タイマー)
	float _appearTimer;		// タイマー(登場用のタイマー)

	bool _isAppear;	// 登場したかどうかのフラグ
	bool _isSound;	// サウンドを鳴らしたかどうかのフラグ　妥協処理

	// 目的地まで移動しながら登場させる関数
	void _appearAtLocation(float deltaTime);
	// 後退させて退場させる関数
	void _moveBackAndExit(float deltaTime);
};
