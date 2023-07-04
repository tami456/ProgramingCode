#include "Rock.h"
#include "../Manager/ResourceManager.h"
#include "../Utility/AsoUtility.h"
#include "RockManager.h"

RockManager::RockManager(const Transform* target)
{
    mTarget = target;
}

RockManager::~RockManager(void)
{
}

void RockManager::Init(void)
{
    //暗転中にUpdateを呼ぶことで最初のラグがなくなる
    Update();
}

void RockManager::Update(void)
{
    //mTarget = 自機
    //自機のマップを計算
    VECTOR mapPos = mTarget->pos;
    mapPos.x /= MAP_SIZE;
    mapPos.y /= MAP_SIZE;
    mapPos.z /= MAP_SIZE;



    //周囲９マップのデブリ生成状況を確認
    //std::map<IntVector3, std::vector<Rock*>> mMapRocks;
    //IntVector3          ->マップ座標(例{0,0,0})
    //std::vector<Rock*>  ->生成したRockクラス30個

    IntVector3 sPos = IntVector3(mapPos);
    sPos.Sub(1);
    IntVector3 checkMapPos;

    int loop = 3;
    for (int x = 0; x < loop; x++)
    {
        for (int y = 0; y < loop; y++)
        {
            for (int z = 0; z < loop; z++)
            {

                checkMapPos.x = sPos.x + x;
                checkMapPos.y = sPos.y + y;
                checkMapPos.z = sPos.z + z;

                /*checkMapPos.x = sPos.x + x;
                checkMapPos.y = sPos.y + y;
                checkMapPos.z = sPos.z + z;*/

                if (mMapRocks.count(checkMapPos) == 0)
                {
                    //③デブリを生成していないマップがあったら作る
                    //CreateRandom関数を使用して30個の岩を生成する
                    std::vector<Rock*> vRocks;

                    //新しいマップで岩をランダム作成
                    for (int i = 0; i < NUM_CREATE; i++)
                    {
                        //push_backの方はここと中でインスタンス(ダブルインスタンス)するから処理が重たくなる
                        //emplace_backの方はインスタンスのダブル生成が出ない
                        vRocks.emplace_back(CreateRandom(checkMapPos));
                    }
                    //mapの方はemplaceの方でいい
                    mMapRocks.emplace(checkMapPos, vRocks);

                }
            }
        }
    }

    //ある程度自機が離れたら岩を削除
    std::vector<IntVector3> deleteMap;

    IntVector3 shipMapPos = IntVector3(mapPos);

    IntVector3 rockMapPos;
    int len;
    for (const auto& p : mMapRocks)
    {
        rockMapPos = p.first;
        len = abs(rockMapPos.x - shipMapPos.x)
            + abs(rockMapPos.y - shipMapPos.y)
            + abs(rockMapPos.z - shipMapPos.z);
        if (len > 4)
        {
            for (auto rock:p.second)
            {
                rock->Release();
                delete rock;
            }
            deleteMap.emplace_back(p.first);
        }
    }

    for (const auto& key : deleteMap)
    {
        mMapRocks.erase(key);
    }
}

void RockManager::Draw(void)
{

    for (const auto& p : mMapRocks)
    {
        for (const auto& rock : p.second)
        {
            rock->Draw();
        }
    }

}

void RockManager::Release(void)
{
    for (auto& p : mMapRocks)
    {
        //for (const auto& rock : p.second)
        for (const auto& rock : p.second)
        {
            rock->Release();
            delete rock;
        }
        p.second.clear();
    }
    mMapRocks.clear();
}

Rock* RockManager::CreateRandom(IntVector3 mapPos)
{
    //岩のモデルをランダム
    int r = GetRand(1);
    int rockModelId = -1;
    switch (r)
    {
    case 0:
        rockModelId = ResourceManager::GetInstance().
            LoadModelDuplicate(ResourceManager::SRC::ROCK01);
        break;
    case 1:
        rockModelId = ResourceManager::GetInstance().
            LoadModelDuplicate(ResourceManager::SRC::ROCK02);
        break;
    }

    const int MIN_SCL = 2;
    const int MAX_SCL = 8;

    //大きさ
    VECTOR scl;
    int s = MIN_SCL + GetRand(MAX_SCL - MIN_SCL);
    scl.x = static_cast<float>(s);
    scl.y = static_cast<float>(s);
    scl.z = static_cast<float>(s);


    //位置
    VECTOR pos;
    pos.x = static_cast<float>(mapPos.x * MAP_SIZE);
    pos.y = static_cast<float>(mapPos.y * MAP_SIZE);
    pos.z = static_cast<float>(mapPos.z * MAP_SIZE);
    
    //-1000～1000
    int hMapSize = MAP_SIZE / 2;
    int rX = GetRand(MAP_SIZE) - hMapSize;
    int rY = GetRand(MAP_SIZE) - hMapSize;
    int rZ = GetRand(MAP_SIZE) - hMapSize;

    pos.x += rX;
    pos.y += rY;
    pos.z += rZ;

    //角度
    VECTOR rot;
    rot.x = AsoUtility::Deg2RadF(static_cast<float>(GetRand(360)));
    rot.y = AsoUtility::Deg2RadF(static_cast<float>(GetRand(360)));
    rot.z = 0.0f;

    Rock* ret = new Rock();
    ret->Init(rockModelId, pos, rot, scl);

    return ret;
}


