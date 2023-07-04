#include <DxLib.h>
#include "Particle.h"
#include "../Manager/SceneManager.h"

Particle::Particle(void)
{
}

Particle::~Particle(void)
{
}

void Particle::Generate(int img, VECTOR pos, float size, VECTOR dir, float speed, float lifeTime)
{
    mImg = img;
    //出現場所
    mPos = pos;
    mSize = size;
    mDir = dir;
    mSpeed = speed;
    mLife = lifeTime;
}

void Particle::Update(void)
{
    float deltaTime = SceneManager::GetInstance().GetDeltaTime();
    if (mLife > 0.0f)
    {
        mLife -= deltaTime;
    }
    else
    {
        return;
    }


    //移動(座標 += 移動方向*スピード*デルタタイム)

    mPos = VAdd(mPos, VScale(mDir, mSpeed * deltaTime));
}

void Particle::Draw(void)
{
    if (!isAlive())
    {
        return;
    }
    DrawBillboard3D(mPos,0.5f,0.5f,mSize,0.0f,mImg,true);

}

void Particle::Release(void)
{
}

bool Particle::isAlive(void) const
{
    return mLife > 0.0f;
}

VECTOR Particle::GetPos(void) const
{
    return mPos;
}

void Particle::SetPos(const VECTOR& pos)
{
    mPos = pos;
}

float Particle::GetZLen(void) const
{
    return mZLen;
}

void Particle::SetZLen(const float& zlen)
{
    mZLen = zlen;
}
