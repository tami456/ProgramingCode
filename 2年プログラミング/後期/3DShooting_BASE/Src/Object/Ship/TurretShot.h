#pragma once
#include "ShotBase.h"
class Transform;
class TurretShot :
    public ShotBase
{
public:
    TurretShot(const Transform* parent);
    ~TurretShot(void);
};

