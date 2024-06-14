#pragma once
#include "Scene.h"
class TitleScene :
    public Scene
{
private:

public:
    TitleScene(SceneManager& scene);
    //virtual&override入れてる理由
    //引数入れ間違えると別物になってしまうため
    //分かりやすくしている(実際違うとエラーが出る)
    virtual void Update(Input& input)override;
    virtual void Draw()override;
};

