#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <memory>
#include <string>
#include <DxLib.h>
#include "Application.h"

#ifdef _DEBUG
#define	new	new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

std::shared_ptr<int> testSha_;
void TestShared(std::shared_ptr<int> a)
{
	int cnt2 = testSha_.use_count();
	std::shared_ptr<int> b = a;

	// �V�F�A�[�h�|�C���^�̎Q�ƃJ�E���g
	int cnt3 = testSha_.use_count();
}

// WinMain�֐�
//---------------------------------
int WINAPI WinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	// ���������[�N���o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// �@���|�C���^
	//int* test = new int();
	//delete test;

	// �A���j�[�N�|�C���^
	//std::unique_ptr<int> testI = std::make_unique<int>();
	//std::unique_ptr<std::string> testS = std::make_unique<std::string>();

	//// �B�V�F�A�[�h�|�C���^
	//testSha_ = std::make_shared<int>(5);
	//// �V�F�A�[�h�|�C���^�̎Q�ƃJ�E���g
	//int cnt1 = testSha_.use_count();
	//// �ŏ��͎������Ȃ�
	//TestShared(testSha_);
	//int cnt4 = testSha_.use_count();

	// �C���X�^���X�̐���
	Application::CreateInstance();

	// �C���X�^���X�̎擾
	Application& instance = Application::GetInstance();

	if (instance.IsInitFail())
	{
		// ���������s
		return -1;
	}

	// ���s
	instance.Run();

	// ���
	instance.Destroy();

	return 0;

}
