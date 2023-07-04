#pragma once
#include "Controller.h"
//PadInput
class Gamepad :
    public Controller
{
public:
    Gamepad();
    ~Gamepad();
private:
    void Update(BaseScene& scene, const Actor* own)override;
};

