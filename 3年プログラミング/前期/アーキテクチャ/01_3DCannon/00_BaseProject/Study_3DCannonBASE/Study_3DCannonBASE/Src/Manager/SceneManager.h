#pragma once
#include <chrono>

class SceneBase;
class Fader;
class Camera;

class SceneManager
{

public:

	static constexpr float DEFFAULT_FPS = 60.0f;

	//�d��
	static constexpr float GRAVITY = 9.81;

	// �V�[���Ǘ��p
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME
	};
	
	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static SceneManager& GetInstance(void);

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// ��ԑJ��
	void ChangeScene(SCENE_ID nextId);

	// �V�[��ID�̎擾
	SCENE_ID GetSceneID(void);

	// �f���^�^�C���̎擾
	float GetDeltaTime(void) const;

	Camera* GetCamera(void) const;

private:

	// �ÓI�C���X�^���X
	static SceneManager* instance_;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	// �t�F�[�h
	Fader* fader_;

	// �e��V�[��
	SceneBase* scene_;

	// �V�[���J�ڒ�����
	bool isSceneChanging_;

	// �f���^�^�C��
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	
	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	SceneManager(void);
	// �R�s�[�R���X�g���N�^�����l
	SceneManager(const SceneManager&);
	// �f�X�g���N�^�����l
	~SceneManager(void);

	// �f���^�^�C�������Z�b�g����
	void ResetDeltaTime(void);

	// �V�[���J��
	void DoChangeScene(SCENE_ID sceneId);

	// �t�F�[�h
	void Fade(void);

	//�J����
	Camera* camera_;
};