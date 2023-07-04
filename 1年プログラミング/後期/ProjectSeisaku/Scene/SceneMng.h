#pragma once
#include "../common/Vector2D.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:
	void Run(void);
	static SceneMng& GetInstance(void)
	{
		static SceneMng s_Instance;
		//Create();
		return s_Instance;
	}
	Vector2D GetScreenSize(void);
	//static void Create(void)			//静的な関数
	//{
	//	if (!s_Instance)				//実体がなかったら、インスタンスする
	//	{
	//	s_Instance = new SceneMng();
	//	}
	//}
	//static void Destroy(void)					//いらなくなったらnullする。いるときはcreateをよぶ
	//{
	//	if (s_Instance)
	//	{
	//		delete s_Instance;
	//		s_Instance = nullptr;
	//	}
	//}
private:
	//static SceneMng* s_Instance;
	SceneMng();
	~SceneMng();
	bool SysInit(void);		//初期化関係の関数
	bool initFlag_;
};

