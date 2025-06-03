//--------------------------------------------------------------------
// ファイル名 ：ScoreManager.cpp
// 概要       ：UIでスコアを表示する際の補助を行う
// 作成者     ：0231本間
// 更新内容   ：01/10　作成、
//--------------------------------------------------------------------
#include "ScoreManager.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AScoreManager::AScoreManager()
	:_score(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AScoreManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AScoreManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 AScoreManager::Get_score()
{
	return _score;
}

void AScoreManager::AddScore(int32 addScore)
{
	_score += addScore;
	beforegetscore = addScore;

	m_ui_getscore = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*path)).LoadSynchronous();
	m_pui_getscore = CreateWidget<UUserWidget>(GetWorld(), m_ui_getscore);

	if (m_pui_getscore)
	{
		m_pui_getscore->AddToViewport();
	}
}

int32 AScoreManager::Get_beforescore()
{
	return beforegetscore;
}

int32 AScoreManager::Get_JewelNum()
{
	return _jewelNum;
}

void AScoreManager::AddJewelNum()
{
	_jewelNum++;
}