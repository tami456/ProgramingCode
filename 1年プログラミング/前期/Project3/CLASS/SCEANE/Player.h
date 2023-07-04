#pragma once
#include "../CharaImage.h"
#include "../COMMON/Vector2.h"
#include "MapCtl.h"


#define PLAYER_MAX 2

class Player
{
public:
	enum class State 
	{
		NON,	//キャラ未選択
		ALIVE,	//生きてる
		DEAD,	//死んでる
		MAX
	};
	Player();
	~Player();
	bool Init(CharID charID, bool loadFlag,Vector2 chipSize);
	bool Release(void);
	State Update(MapCtl*mapCtl);
	void SetDir(void);
	void Draw(CharImage* charImage);
	void Draw(void);
	
	void GetKeyState(char* keyData);
	bool SetState(State state);
	State GetState(void);
	CharID charID_;
	Vector2 pos_;
	static int playerCount;
	int animCount;
	int keyList_[static_cast<int>(Dir::MAX)]; //方向ごとのキーリスト
	int keyNow_[static_cast<int>(Dir::MAX)];
	int keyOld_[static_cast<int>(Dir::MAX)];

private:
	Vector2 chipSize_;
	State state_;
	CharImage charImage_;
	Dir dir_;
	//CharImage charImage_;


};

