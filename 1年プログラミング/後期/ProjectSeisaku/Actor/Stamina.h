#pragma once
class Stamina
{
public:
	Stamina();
	~Stamina();
	bool Init(void);
	void Update(void);
	void Draw(void);
private:
	int gage;
	int gage2;
};

