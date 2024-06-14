#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "ShotBase.h"

ShotBase::ShotBase(int baseModelId, int* blastImgs, int blastAnimNum)
{
    baseModelId_ = baseModelId;
    blastImgs_ = blastImgs;
    blastAnimNum_ = blastAnimNum;
}

ShotBase::~ShotBase(void)
{
}

void ShotBase::CreateShot(VECTOR pos, VECTOR dir)
{
    //使用メモリ容量と読み込み時間削減のため
    //モデルデータをいくつもメモリ上に存在させない
    modelId_ = MV1DuplicateModel(baseModelId_);

    //弾の大きさを設定
    scl_ = { 0.8f,0.8f,0.8f };

    //弾の角度を設定
    rot_ = { 0.0f,0.0f,0.0f };

    //弾の発射位置を設定
    pos_ = pos;

    //弾の発射方向の設定
    dir_ = dir;

    //弾の速度
    speed_ = 8.0f;

    //弾の生存判定
    isAlive_ = true;

    //重力
    gravityPow_ = 0.0f;

    //爆発のアニメーション用カウンタ
    blastCntAnim_ = 0;

    //爆発のアニメーション速度
    blastSpeedAnim_ = 0.3f;

    //状態遷移
    ChangeState(STATE::SHOT);
}

void ShotBase::Update(void)
{
    //大きさの設定
    MV1SetScale(modelId_, scl_);

    //角度の設定
    MV1SetRotationXYZ(modelId_, rot_);

    //位置の設定
    MV1SetPosition(modelId_, pos_);

    switch (state_)
    {
    case ShotBase::STATE::SHOT:
        UpdateShot();
        break;
    case ShotBase::STATE::BLAST:
        UpdateBlast();
        break;
    case ShotBase::STATE::END:
        UpdateEND();
        break;
    }

}

void ShotBase::UpdateShot(void)
{
    if (!IsAlive())
    {
        //生存していなければ処理中断
        return;
    }

    //弾を移動させる

    //移動量の計算(方向×スピード)
    VECTOR movePow;
    movePow = VScale(dir_, speed_);

    //移動処理(座標＋移動量)
    pos_ = VAdd(pos_, movePow);

    //さらに加速度的に重力を加える
    gravityPow_ += SceneManager::GRAVITY / SceneManager::DEFFAULT_FPS;
    pos_.y -= gravityPow_;
}

void ShotBase::UpdateBlast(void)
{
    blastIdxAnim_++;
    //爆発アニメーションの終了判定
    if (blastIdxAnim_ + 1 >= blastAnimNum_)
    {
        blastIdxAnim_ = 0;
        ChangeState(STATE::END);
    }
}

void ShotBase::UpdateEND(void)
{
    //何もしない
}

void ShotBase::Draw()
{
    switch (state_)
    {
    case ShotBase::STATE::SHOT:
        DrawShot();
        break;
    case ShotBase::STATE::BLAST:
        DrawBlast();
        break;
    case ShotBase::STATE::END:
        DrawEND();
        break;
    }

}

void ShotBase::DrawShot(void)
{
    if (!IsAlive())
    {
        //生存していなければ処理中断
        return;
    }
    MV1DrawModel(modelId_);

}

void ShotBase::DrawBlast(void)
{
    
    DrawBillboard3D(
        pos_, 0.5f, 0.5f, 80.0f, 0.0f, blastImgs_[blastIdxAnim_], true);
}

void ShotBase::DrawEND(void)
{
}

void ShotBase::Release(void)
{
    MV1DeleteModel(modelId_);
}

bool ShotBase::IsShot(void)
{

    if (state_ == STATE::SHOT)
    {
        return true;
    }

    return false;
}

void ShotBase::Blast(void)
{
    state_ = STATE::BLAST;
}

bool ShotBase::IsAlive(void)
{
    return state_ != STATE::END;
}

VECTOR ShotBase::GetPos(void) const
{
    return pos_;
}

void ShotBase::ChangeState(STATE state)
{
    state_ = state;

    switch (state_)
    {
    case ShotBase::STATE::NONE:
        break;
    case ShotBase::STATE::SHOT:
        break;
    case ShotBase::STATE::BLAST:
        break;
    case ShotBase::STATE::END:
        break;
    }
}
