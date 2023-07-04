#pragma once



#define BG_SIZE_Y 430;
class Bg
{
public:
	Bg();
	~Bg();
	void Init();
	void Update();
	void Draw();
private:
	int bgImage;
	int bgPosY;
	int bgPosY1;
	int speed;
	int gameScreen;
};

