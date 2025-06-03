//--------------------------------------------------------------------
//制作者　：坂上　壱希
//制作日時：2025/02/04
//最終更新：2025/02/10
//概　要　：SEを管理する
//--------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "SEManager.generated.h"

	/* このManagerの使い方
	//#include"SEManager.h"をインクルードする
	//SEを登録する場合はUSoundBase*型の変数を
	//UPROPERTY(EditAnywhere)で用意しBP側でアタッチする

	UPROPERTY(EditAnywhere)
	USoundBase*　Sound;		//登録したいSEまたはBGM

	//インスタンスを取得
	USEManager _seManager = USEManager::GetInstance(this);

	// SE を登録
	_seManager->RegisterSE(TEXT("Sound"), Sound);

	// 2D SE を 1 回だけ再生
	_seManager->PlaySE2D(TEXT("Sound"));

	// 2D SE をループ再生
	_seManager->PlaySE2DLoop(TEXT("Sound"));

	// ループSEを停止
	_seManager->StopSE(TEXT("Sound"));
	*/

UCLASS()
class PROTOTYPE_API USEManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USEManager() {}
	//void BeginDestroy() override {}		//デストラクタ
	~USEManager() {}		//デストラクタ

	//シングルトンを取得
	UFUNCTION(BlueprintPure, Category = "MySingleton")
	static USEManager* GetInstance(UObject* WorldContextObject);

	//----------------------------------
	// 関数名：RegisterSE
	// 引　数：FName,USoundBase*
	// 戻り値：void
	// SEを登録
	//----------------------------------
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void RegisterSE(FName key, USoundBase* sound);

	//----------------------------------
	// 関数名：PlaySE2D
	// 引　数：UObject*,WorldContextObject,FName
	// 戻り値：void
	// SEを再生
	//----------------------------------
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlaySE2D(FName key);

	//----------------------------------
	// 関数名：PlaySE3D
	// 引　数：UObject*,WorldContextObject,FName
	// 戻り値：void
	// SEを再生
	//----------------------------------
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlaySE3D(UObject* worldContextObject, USoundBase* sound, FName key, FVector location);

	//----------------------------------
	// 関数名：PlaySE2DLoop
	// 引　数：無し
	// 戻り値：void
	// SEをループ再生
	//----------------------------------
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlaySE2DLoop(FName Key);

	//----------------------------------
	// 関数名：PlayeSE3DLoop
	// 引　数：無し
	// 戻り値：void
	// SEをループ再生
	//----------------------------------
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayeSE3DLoop(UObject* worldContextObject, FName key);

	//----------------------------------
	// 関数名：StopSE
	// 引　数：FName
	// 戻り値：void
	// SEを停止
	//----------------------------------
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void StopSE(FName key);

private:

	//SEデータ
	UPROPERTY()
	TMap<FName, TObjectPtr<USoundBase>> _seMap;

	//ループ用のAudioComponentを管理
	UPROPERTY()
	TMap<FName, TObjectPtr<UAudioComponent>> _loopingSEMap;
};
