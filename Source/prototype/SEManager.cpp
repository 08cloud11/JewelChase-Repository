//--------------------------------------------------------------------
//制作者　：坂上　壱希
//制作日時：2025/02/04
//最終更新：2025/02/10
//概　要　：SEを管理する
//--------------------------------------------------------------------

#include "SEManager.h"
#include "Kismet/GameplayStatics.h"

USEManager* USEManager::GetInstance(UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	return GameInstance ? GameInstance->GetSubsystem<USEManager>() : nullptr;
}

void USEManager::RegisterSE(FName key, USoundBase* sound)
{
	if (!sound)
	{
		return;
	}

	_seMap.Add(key, sound);
}

void USEManager::PlaySE2D(FName key)
{
	if (_seMap.Contains(key))
	{
		UE_LOG(LogTemp, Log, TEXT("PlaySE2D : %s"), *key.ToString());
		
		// 一回だけ再生（ループしない）
		UGameplayStatics::PlaySound2D(GWorld, _seMap[key]);
	}
}

void USEManager::PlaySE3D(UObject* worldContextObject, USoundBase* sound, FName key, FVector location)
{
	if (sound)
	{
		UGameplayStatics::PlaySoundAtLocation(worldContextObject, sound, location);
	}
}

void USEManager::PlaySE2DLoop(FName key)
{
	if (_seMap.Contains(key))
	{
		// すでにループ再生されているなら何もしない
		if (_loopingSEMap.Contains(key))
		{
			return;
		}

		// AudioComponent を作成
		UAudioComponent* AudioComp = UGameplayStatics::SpawnSound2D(GWorld, _seMap[key]);
		if (AudioComp)
		{
			AudioComp->Play();
			_loopingSEMap.Add(key, AudioComp);
		}
	}
}

void USEManager::PlayeSE3DLoop(UObject* worldContextObject, FName key)
{
	if (_seMap.Contains(key))
	{
		// すでにループ再生されているなら何もしない
		if (_loopingSEMap.Contains(key))
		{
			return;
		}

		// AudioComponent を作成
		UAudioComponent* AudioComp = UGameplayStatics::SpawnSound2D(worldContextObject, _seMap[key]);
		if (AudioComp)
		{
			AudioComp->Play();
			_loopingSEMap.Add(key, AudioComp);
		}
	}
}

void USEManager::StopSE(FName key)
{
	if (_loopingSEMap.Contains(key))
	{
		UAudioComponent* AudioComp = _loopingSEMap[key];
		if (AudioComp)
		{
			//多分クラッシュ原因
			AudioComp->Stop();
		}
		_loopingSEMap.Remove(key);
	}
}