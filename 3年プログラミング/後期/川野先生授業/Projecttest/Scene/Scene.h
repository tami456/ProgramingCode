#pragma once

class Input;
class SceneManager;

/// <summary>
/// �V�[���̊��N���X(����1�N���X)
/// </summary>
class Scene
{
protected:
	SceneManager& sceneManager_;
public:
	Scene(SceneManager& manager);
	//������͑厖virtual���Ȃ��Ɛe�̂ق������s�����
	//�������Ȃ��Ă�����ɍ���邪virtual��t����
	//����Ă����Ȃ��Ɖ������ĂȂ��f�X�g���N�^�̕����Ă΂��̂ŃS�~
	virtual ~Scene(){};
	virtual void Update(Input& input) = 0;
	virtual void Draw() = 0;
};

