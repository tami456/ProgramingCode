#pragma once
#include<string>
#include<unordered_map>
#include<vector>

enum class InputType
{
	keyboard,
	gamepad,
	mouse
};

struct InputInfo {
	InputType type;
	int inputID;
};

//struct InputState {
//	char keycode[256];
//	bool isPressed = false;
//};

//�Q�[�����̈Ӗ��Ɠ��͂��ꂽ���ǂ������y�A�ɂ�������
//�Ƃɂ����Ώۂ́u�Ӗ��v�������ꂽ��l��true�ɂȂ�^
using InputState_t = std::unordered_map<std::string, bool>;

//�Q�[������"�Ӗ�"�Ǝ��ۂ̃L�[�R�[�h�̃y�A
//�L�[(first)�͖��߂̈Ӗ��A
//�Q�[������"�Ӗ�"�Ǝ��ۂ̓���(�n�[�h����)�̑Ή��\��\���u�^�v
using InputTable_t = std::unordered_map<std::string, std::vector<InputInfo>>;

class KeyconfigScene;

class Input
{
	friend KeyconfigScene;
private:

	std::vector<std::string> excludeOnConfigList_;//�L�[�R���t�B�O���O���X�g

	//�Ӗ��ƃn�[�h���͂̑Ή��\
	InputTable_t inputTable_;
	InputTable_t tempTable_;
	

	//�������L�^
	InputState_t currentState_ = {};
	InputState_t lastState_ = {};


	int currentpadState_;
	int lastpadState_;

	//�l�̑���ƃR�s�[�̋֎~
	//�V���O���g���ł͂Ȃ��������o�Ă���Ƃ�₱�����Ȃ邽��
	Input(const Input&) = delete;
	void operator=(const Input&) = delete;

	//���݂��̃L�[��������Ă��邩
	bool GetCurrentInput(const std::string& input)const;
	//���O�ɂ��̃L�[��������Ă��邩
	bool GetLastInput(const std::string& input)const;
	
public:
	
	Input();
	/// <summary>
	/// ���͏����X�V����(���t���[���Ă΂��)
	/// </summary>
	void Update();

	/// <summary>
	/// ���������u�Ԃ��ǂ����Ԃ�(�������ςȂ���false)
	/// </summary>
	/// <param name="input">���͎��ʕ�����<"next"��"pause"�Ȃ�/param>
	/// <returns>�������u�ԂȂ�true</returns>
	bool IsTriggered(const std::string& input)const;

	/// <summary>
	/// �������Ă��邩�ǂ�����Ԃ�
	/// </summary>
	/// <param name="input">���͎��ʕ�����<"next"��"pause"�Ȃ�/param>
	/// <returns>�������u�ԂȂ�true</returns>
	bool IsPressed(const std::string& input)const;

	/// <summary>
	/// ���̓e�[�u���̏�������
	/// </summary>
	/// <param name="key">���ߕ�����</param>
	/// <param name="inputInfo">�@�킩��̓��͏��(�n�[�h)</param>
	void RewriteInput(const std::string& key,
						const InputInfo& inputInfo);

	void CommitInputConfig();

	/// <summary>
	/// ���̓e�[�u�����l����
	/// </summary>
	/// <returns></returns>
	const InputTable_t GetInputTabel()const;

	void SaveInputTable();

	void LoadInputTable();

	/// <summary>
	/// ���݂̓��͂����O���͂ɓ����邩�ǂ������`�F�b�N����
	/// </summary>
	/// <returns>���O���͂ɂ�����Afalse:���O����K�v�͂Ȃ�</returns>
	bool CheckExclusiveInput();
};

