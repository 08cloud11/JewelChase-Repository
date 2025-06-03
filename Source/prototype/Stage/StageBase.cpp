//--------------------------------------------------------------------
// ファイル名 ：StageBase.cpp
// 概要       ：ステージ（障害物や敵）の生成を制御するクラス
// 作成者     ：0231本間
// 更新内容   ：11/25　作成、プロパティとメソッドを作成
//              11/29  ステージマネージャーからのcsvパス取得を調整中
//--------------------------------------------------------------------

#include "StageBase.h"
#include "StageData.h"
#include "StageManager.h"
#include "prototype/Enemy/HelicopterAttackComponent.h"
#include "prototype/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

//コンストラクタ
AStageBase::AStageBase()
    : _bspawnstage(false)
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

//生成時のみ処理
void AStageBase::BeginPlay()
{
    Super::BeginPlay();

    pStageMng = SearchStageMng();
    pPlayer = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());

    _stagelane = Cast<AStageManager>(pStageMng)->Get_stagelane();
    _movespeed = Cast<AStageManager>(pStageMng)->Get_movespeed();

    //ステージ番号が最大ではないときステージを読み込む
    if (Cast<AStageManager>(pStageMng)->Get_stagenum() <= Cast<AStageManager>(pStageMng)->Get_maxstagenum())
    {
        //ステージマネージャからcsvを取得
        StageDataTable = Cast<AStageManager>(pStageMng)->Get_csvpath();

        SpawnObject();
    }
    //ステージ番号が最大のときゴールを読み込む
    else
    {
        //ステージマネージャからcsvを取得
        StageDataTable = Cast<AStageManager>(pStageMng)->Get_goalpath();

        SpawnObject();
    }
    FName ItemTag = "Item";
    TArray<AActor*> Items;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), ItemTag, Items);

    Cast<AStageManager>(pStageMng)->SetItems(Items);
   
}

//毎フレーム処理
void AStageBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

     if (Cast<APlayerCharacter>(pPlayer)->GetHP() > 0 && Cast<AStageManager>(pStageMng) ->get_bStart())
     {
        SpawnNextStage();

        DeleteStage();

        Move();
     }
}

// 関数名：SpawnObject
// 引　数：なし
// 戻り値：void
// 処理内容：障害物をステージに配置し、親子関係を構築
void AStageBase::SpawnObject()
{
    CommonSpawn(-1000.0f);

    FTransform SpawnTr;
    FVector pos;
    AActor* spawnActor = nullptr;

    if (StageDataTable == nullptr)
    {
        // 参照できない場合は同期読み.
        StageDataTable.LoadSynchronous();
    }

    if (StageDataTable)
    {
        // データテーブルから全データを取得する.
        TArray<FName> RowNames = StageDataTable->GetRowNames();

        for (auto RowName : RowNames)
        {
            const FMapStruct* data = StageDataTable->FindRow<FMapStruct>(RowName, FString());
            if (!data)
            {
                continue;
            }

            //オブジェクトが生成範囲外のものなら生成しない
            if (data->ylane > _stagelane)
            {

            }

            //csvのレーン情報を読み取りポジション決定
            float X = 500.0f * float(data->xlane);
            float Y = 180.0f * float(data->ylane - 4);
            float Z = 50.0f * float(data->zlane);

            pos = GetActorLocation() + FVector(X, Y, Z);

            SpawnTr.SetLocation(pos);

            //csvデータのオブジェクトタイプを参照しスポーン
            switch (data->ObjecType)
            {
            case 1://ゴミ箱
                if (_obstacles[0] != nullptr) {
                    spawnActor = GetWorld()->SpawnActor<AActor>(_obstacles[0], SpawnTr);
                }
                break;
            case 2://街灯
                if (_obstacles[1] != nullptr) {
                    spawnActor = GetWorld()->SpawnActor<AActor>(_obstacles[1], SpawnTr);
                }
                break;
            case 3://バリケード
                if (_obstacles[2] != nullptr) {
                    spawnActor = GetWorld()->SpawnActor<AActor>(_obstacles[2], SpawnTr);
                }
                break;
            case 4://車（神野カラー）
                if (_obstacles[3] != nullptr) {
                    spawnActor = GetWorld()->SpawnActor<AActor>(_obstacles[3], SpawnTr);
                }
                break;
            case 5://車（坂上カラー）
                if (_obstacles[4] != nullptr) {
                    spawnActor = GetWorld()->SpawnActor<AActor>(_obstacles[4], SpawnTr);
                }
                break;
            case 6://車（福田カラー）
                if (_obstacles[5] != nullptr) {
                    spawnActor = GetWorld()->SpawnActor<AActor>(_obstacles[5], SpawnTr);
                }
                break;
            case 7://車（山内カラー）
                if (_obstacles[6] != nullptr) {
                    spawnActor = GetWorld()->SpawnActor<AActor>(_obstacles[6], SpawnTr);
                }
                break;
            case 8://車（本間カラー）
                if (_obstacles[7] != nullptr) {
                    spawnActor = GetWorld()->SpawnActor<AActor>(_obstacles[7], SpawnTr);
                }
                break;
            case 9://車（森本カラー）
                if (_obstacles[8] != nullptr) {
                    spawnActor = GetWorld()->SpawnActor<AActor>(_obstacles[8], SpawnTr);
                }
                break;
            case 10://オイル缶
                if (_obstacles[9] != nullptr) {
                    spawnActor = GetWorld()->SpawnActor<AActor>(_obstacles[9], SpawnTr);
                }
                break;


            case 11://青ジュエル
                if (_jewels[0] != nullptr) {
                    spawnActor = GetWorld()->SpawnActor<AActor>(_jewels[0], SpawnTr);
                }
                break;
            case 12://赤ジュエル
                if (_jewels[1] != nullptr) {
                    spawnActor = GetWorld()->SpawnActor<AActor>(_jewels[1], SpawnTr);
                }
                break;
            case 13://黄ジュエル
                if (_jewels[2] != nullptr) {
                    spawnActor = GetWorld()->SpawnActor<AActor>(_jewels[2], SpawnTr);
                }
                break;
            case 14://紫ジュエル
                if (_jewels[3] != nullptr) {
                    spawnActor = GetWorld()->SpawnActor<AActor>(_jewels[3], SpawnTr);
                }
                break;

            case 21://体力回復
                if (_Items[0] != nullptr) {
                    spawnActor = GetWorld()->SpawnActor<AActor>(_Items[0], SpawnTr);
                }
                break;
            case 22://マグネット
                if (_Items[1] != nullptr) {
                    spawnActor = GetWorld()->SpawnActor<AActor>(_Items[1], SpawnTr);
                }
                break;
            case 23://カード変化"J"
                if (_Items[2] != nullptr) {
                    spawnActor = GetWorld()->SpawnActor<AActor>(_Items[2], SpawnTr);
                }
                break;
            case 24://カード変化"Q"
                if (_Items[3] != nullptr) {
                    spawnActor = GetWorld()->SpawnActor<AActor>(_Items[3], SpawnTr);
                }
                break;
            case 25://カード変化"K"
                if (_Items[4] != nullptr) {
                    spawnActor = GetWorld()->SpawnActor<AActor>(_Items[4], SpawnTr);
                }
                break;

            case 31://警察
                if (_enemys[0] != nullptr) {
                    spawnActor = GetWorld()->SpawnActor<AActor>(_enemys[0], SpawnTr);
                }
                break;
            case 32://ヘリコプター
                if (_enemys[1] != nullptr) {
                    Cast<AStageManager>(pStageMng)->SetHeliStatus(int(data->HeliStatus));
                    spawnActor = GetWorld()->SpawnActor<AActor>(_enemys[1], SpawnTr);
                    //Cast<UHelicopterAttackComponent>(spawnActor)->SetMoveTargetNumber(data->HeliStatus); // クラッシュするよ
                    
                }
                break;
            case 33://ポリカー
                if (_enemys[2] != nullptr) {
                    spawnActor = GetWorld()->SpawnActor<AActor>(_enemys[2], SpawnTr);
                }
                break;
            case 91://Stage1ゴール
                if (_commons[3] != nullptr) {
                    CommonSpawn(1000.0f);
                    spawnActor = GetWorld()->SpawnActor<AActor>(_commons[3], SpawnTr);
                }
                break;
            case 92://Stage2ゴール
                if (_commons[4] != nullptr) {
                    CommonSpawn(1000.0f);
                    spawnActor = GetWorld()->SpawnActor<AActor>(_commons[4], SpawnTr);
                }
                break;
            case 93://Stage3ゴール
                if (_commons[5] != nullptr) {
                    CommonSpawn(1000.0f);
                    spawnActor = GetWorld()->SpawnActor<AActor>(_commons[5], SpawnTr);
                }
                break;
            default:
                break;
            }

            //StageBaseを親にし、親子関係を構築
            // 妥協処理
            if (IsValid(spawnActor) && data->ObjecType != 32 && data->ObjecType != 33) {
                spawnActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
            }
        }
    }
}

// 関数名：CommonSpawn
// 引　数：なし
// 戻り値：void
// 処理内容：全ステージ共通のオブジェクトをスポーン
void AStageBase::CommonSpawn(float xPos)
{
    FTransform SpawnTr;
    FVector pos;
    AActor* spawnActor = nullptr;

    float ypos = float(_stagelane) * 180.0f;

    //横の壁を作成
    for (int i = 1; i >= -1; i -= 2)
    {
        SpawnTr.SetRotation(FQuat(0.0f, 0.0f, 0.0f, 1.0f));
        pos = GetActorLocation() + FVector(xPos, float(i) * (ypos / 2.0f), 0.0f);
        SpawnTr.SetLocation(pos);

        if (i == 1) {
            FQuat newrotation = FQuat(0.0f, 0.0f, 180.0f, 1.0f);
            SpawnTr.SetRotation(newrotation);
        }

        spawnActor = GetWorld()->SpawnActor<AActor>(_commons[0], SpawnTr);

        //StageBaseを親にし、親子関係を構築
        if (IsValid(spawnActor)) {
            spawnActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
        }
    }

    //地面の動く床を生成
    {
        pos = GetActorLocation() + FVector(xPos, 0.0f, 0.0f);
        SpawnTr.SetLocation(pos);

        spawnActor = GetWorld()->SpawnActor<AActor>(_commons[1], SpawnTr);

        //StageBaseを親にし、親子関係を構築
        if (IsValid(spawnActor)) {
            spawnActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
        }
    }
}

// 関数名：SpawnEnemy
// 引　数：なし
// 戻り値：void
// 処理内容：エネミーを出現させる
void AStageBase::SpawnEnemy()
{

}

// 関数名：SpawnNextStage
// 引　数：なし
// 戻り値：void
// 処理内容：次のステージをスポーンさせる
void AStageBase::SpawnNextStage()
{
    if (!Cast<AStageManager>(pStageMng)->Get_bgoalspawn() && GetActorLocation().X < 1000.0f && !_bspawnstage)
    {
        FTransform SpawnTr;
        SpawnTr.SetLocation(FVector(5000.f, 0.f, 0.f));

        if (_commons[2] != nullptr) {
            GetWorld()->SpawnActor<AActor>(_commons[2], SpawnTr);
        }

        _bspawnstage = true;
    }
}

// 関数名：Move
// 引　数：なし
// 戻り値：void
// 処理内容：ステージをプレイヤ方向（-X）へ移動させる
void AStageBase::Move()
{
    FVector nextpos = GetActorLocation() + FVector(-_movespeed, 0.0f, 0.0f);

    SetActorLocation(nextpos);
}

// 関数名：DeleteStage
// 引　数：なし
// 戻り値：void
// 処理内容：ステージが一定のposまで来たら削除
void AStageBase::DeleteStage()
{
    if (GetActorLocation().X < -5000.0f)
    {
        Cast<AActor>(this)->Destroy();
    }
}

// 関数名：SearchStageMng
// 引　数：なし
// 戻り値：AActor*
// 処理内容：ステージマネージャーを探し、返す
AActor* AStageBase::SearchStageMng()
{
    //AStageManager型のオブジェクトを探し、管理
    TSubclassOf<AStageManager> findClass;
    findClass = AStageManager::StaticClass();

    AActor* stageManager = UGameplayStatics::GetActorOfClass(GetWorld(), findClass);

    //AStageManager型のオブジェクトが１つ以上あれば
    if (IsValid(stageManager))
    {
        return stageManager;
    }
    return nullptr;
}