#pragma once
#include <memory>
#include "BaseScene.h"

//lp = longptr　ロングポインター 
//遠い場所にあるもの
#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
	//シングルトン
public:
	//static = プログラムを起動した瞬間にアドレスを固定する
	//staticはstaticでないとダメ
	//絶対単一で存在してほしいもの＝静的シングルトン
	//消したり作れたりしてほしいもの＝動的シングルトン
	static SceneMng& GetInstance(void)
	{
		static SceneMng s_Instance;

		return s_Instance;
	}
	void Run(void);
	void Update(void);
	void Draw(void);
	//static SceneMng* Create(void)
	//{
	//	
	//	if (s_Instance == nullptr)
	//	{
	//		s_Instance = new SceneMng();
	//	}
	//	return s_Instance;
	//}
	//static void Destroy(void)
	//{

	//	delete s_Instance;

	//	//newを消してもアドレスが残るからnullを入れる
	//	//そうしないとバグる
	//	s_Instance = nullptr;
	//}

private:
	//*がないものを実態と言う
	bool SysInit(void);
	SceneMng();
	~SceneMng();
	//<>=template
	uniquScene scene_;
	

	//ptrの種類 unique_ptr shared_ptr weak_ptr
	//A=B=C=A　循環参照　メモリ上に残ってしまう（バグ）
	//unique_ptr　所有権を自分以外持っていない
	//shared_ptr 所有権を共有することができる
	// shared_ptrだけを使っていると循環参照が起きる
	// shared_ptrを恒久的に使う必要がないならweak_ptrを使ったほうがいい
	//weak_ptr 一時的に所有権を所持できる
};

