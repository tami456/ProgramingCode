#pragma once
#include "Scene.h"
class TitleScene :
    public Scene
{
private:

public:
    TitleScene(SceneManager& scene);
    //virtual&override����Ă闝�R
    //��������ԈႦ��ƕʕ��ɂȂ��Ă��܂�����
    //������₷�����Ă���(���ۈႤ�ƃG���[���o��)
    virtual void Update(Input& input)override;
    virtual void Draw()override;
};

