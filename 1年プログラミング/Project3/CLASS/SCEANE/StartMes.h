#pragma once
#include "../COMMON/Vector2.h"
class StartMes
{
public:
	StartMes();
	~StartMes();
	bool Init(Vector2 screenSize);
	bool Release(void);
	bool Update(void);
	void Draw(void);

private:
	int Image_[2];
	int count_;
	Vector2 screenSize_;
	Vector2 imageSize_;
};

