#pragma once

class Input;
class SceneManager;

/// <summary>
/// シーンの基底クラス(抽象1クラス)
/// </summary>
class Scene
{
protected:
	SceneManager& sceneManager_;
public:
	Scene(SceneManager& manager);
	//これ実は大事virtualつけないと親のほうが実行される
	//何も作らなくても勝手に作られるがvirtualを付けて
	//作っておかないと何もついてないデストラクタの方が呼ばれるのでゴミ
	virtual ~Scene(){};
	virtual void Update(Input& input) = 0;
	virtual void Draw() = 0;
};

