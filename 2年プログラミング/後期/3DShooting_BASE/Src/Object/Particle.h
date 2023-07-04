#pragma once
#include <DxLib.h>

class Particle
{
public:
	// コンストラクタ
	Particle(void);

	// デストラクタ
	~Particle(void);

	void Generate(int img,VECTOR pos,float size,
		VECTOR dir,float speed, float lifeTime);
	void Update(void);
	void Draw(void);
	void Release(void);

	//生存判定
	bool isAlive(void) const;

	VECTOR GetPos(void) const;
	void SetPos(const VECTOR& pos);

	float GetZLen(void) const;
	void SetZLen(const float& zlen);

private:

	//画像のハンドルID
	int mImg;

	//座標
	VECTOR mPos;

	//大きさ
	float mSize;

	//移動方向
	VECTOR mDir;

	//移動速度
	float mSpeed;

	//生存時間
	float mLife;

	//Z距離
	float mZLen;
};

