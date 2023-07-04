#pragma once
#include "SCN_ID.h"
#include "../CharaImage.h"
#include "../COMMON/Vector2.h"
#include "Player.h"
class SelectScene
{
public:
	SelectScene();
	~SelectScene();
	bool Init(void);
	bool Release(void);
	SCN_ID Update(char* keyData, char* keyDataOld);
	void Draw(void);
	void SelectDraw(void);
	CharID GetSelPlayer(int no);

private:
	CharImage charImage_[static_cast<int>(CharID::MAX)];
	int selectImage;		//ƒLƒƒƒ‰‚Ì‘I‘ğƒƒS
	int bgImage;			//”wŒi
	Vector2 screenSize_;	//‰æ–Ê‚Ì‰ğ‘œ“x
	Player player_[PLAYER_MAX];
	int gameScreenID_;
	int screenID_;

};

