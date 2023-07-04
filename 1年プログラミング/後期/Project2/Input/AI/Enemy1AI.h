#pragma once
#include "../Controller.h"
class Enemy1AI :
    public Controller
{
public:
    Enemy1AI();
    ~Enemy1AI();
private:
    void Update(BaseScene& scene, const Actor* own) override;
};

