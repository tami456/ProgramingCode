#pragma once
#include <array>
#include <vector>
#include <memory>
#include "../Actor/Bomb.h"
#include "../common/Vector2D.h"
#include "../Actor/Dir.h"

struct FireStruct
{
	int count;	//方向の炎の表示残り時間
	int length;	//方向の炎の残り伸びマス
};

using FireData = std::array<FireStruct, static_cast<int>(Dir::MAX)>;

class tmxObj;

class FireMap
{
public:
	FireMap(const Vector2D& worldArea,const Vector2D& tileSize);
	~FireMap();
	bool SetData(const Vector2D& pos,int power);			//posは座標
	bool SetData(int x,int y, int power);					//x,yはマス目版
	bool SetData(int x, int y, int power,Dir dir);			//x,yはマス目版
	FireData& GetData(const Vector2D& pos);
	bool CheckFire(const Vector2D& pos);
	void Update(std::weak_ptr<tmxObj> tmxObj);
	void Draw(std::weak_ptr<tmxObj> tmxObj, int x,int y, const Vector2D& camOffset,const Vector2D& drawOffset);
	bool Init(void);
private:
	std::vector<FireData> fireData_;
	std::array<Vector2D, static_cast<int>(Dir::MAX)> dirOffset_;
	std::array<double, static_cast<int>(Dir::MAX)> dirRad_;
	int fireHdl_[4][3];
	Vector2D worldArea_;
	Vector2D tileSize_;
};

