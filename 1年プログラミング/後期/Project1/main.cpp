#include <vector>
#include <DxLib.h>
#include "Square.h"
#include "Circle.h"
#include "_debug/_DebugConOut.h"

#define OBJ_X 13	//横に並べるオブジェクトの数
#define OBJ_Y 9		//縦に並べるオブジェクトの数


int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	SetGraphMode(640, 480, 32);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
	{
		return -1;
	}


	std::vector<Square*> shapeVec;
	shapeVec.reserve(OBJ_X * OBJ_Y);
	//ShapeVec[0] = new Square{ 420,200,50,50 };


	//初期化リスト
	//Shape* shape = new Square{ 220,165,200,150 };

	

	SetDrawScreen(DX_SCREEN_BACK);
	bool spKeyFlag = false;
	bool spKeyFlagOld = false;

	bool lKeyFlag = false;
	bool lKeyFlagOld = false;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		spKeyFlagOld = spKeyFlag;
		spKeyFlag = CheckHitKey(KEY_INPUT_SPACE);
		if (spKeyFlag && !spKeyFlagOld)
		{
			if (shapeVec.size() + 1 <= shapeVec.capacity())
			{
				TRACE("スペースキー押した瞬間\n");
				shapeVec.resize(shapeVec.size() + 1);
				shapeVec[shapeVec.size() - 1] = new Square{ static_cast<int>(((shapeVec.size() - 1) % OBJ_X) * 50),
															static_cast<int>(((shapeVec.size() - 1) / OBJ_X) * 50),50,50 };
				//shapeVec.emplace_back(new Square{ static_cast<int>(((shapeVec.size() - 1) % OBJ_X) * 50),
															//static_cast<int>(((shapeVec.size() - 1) / OBJ_X) * 50),50,50 });
				shapeVec[shapeVec.size() - 1]->SetColor(((shapeVec.size() - 1) % 2) ? 0xffffff : 0x00ff00);
				
			}
		}
		
		lKeyFlagOld = lKeyFlag;
		lKeyFlag = CheckHitKey(KEY_INPUT_RETURN);
		if (lKeyFlag && !lKeyFlagOld)
		{
			if (shapeVec.size())
			{
				TRACE("ENMTER押したよね？\n");
				delete (*shapeVec.begin());
				shapeVec.erase(shapeVec.begin());
			}
		}
		
		ClsDrawScreen();

		for (auto& shape:shapeVec)
		{
			shape->Draw();
		}

		//auto num = 0U;
		//
		//for (auto itr = shapeVec.begin();
		//	itr != shapeVec.end(); ++itr)
		//{
		//	(*itr)->Draw();
		//	//(**itr).Draw;
		//}

		//for(std::vector<Shape*>::iterator itr = shapeVec.begin();
		//			itr != shapeVec.end();++itr)
		//{
		//	(*itr)->Draw();
		//	//(**itr).Draw;
		//}
		ScreenFlip();
	}
	return 0;

}