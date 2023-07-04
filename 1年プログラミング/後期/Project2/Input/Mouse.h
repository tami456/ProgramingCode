#pragma once
#include "Controller.h"
#include "../common/Vector2D.h"
class Mouse :
    public Controller
{
public:
    Mouse();
    ~Mouse();
private:
    void Update(BaseScene& scene, const Actor* own) override;
    std::array<Vector2D, 2> pos_;
};

