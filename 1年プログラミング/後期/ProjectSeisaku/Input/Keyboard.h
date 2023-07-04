#pragma once
#include "Controller.h"
class Keyboard :
    public Controller
{
public:
    Keyboard();
    ~Keyboard();
private:
    void Update(BaseScene& scene, const Actor* own) override;
    char keyData_[256];

};

