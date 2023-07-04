#pragma once

#include <vector>
#include <map>
#include "../Common/IntVector3.h"

class Transform;
class Rock;

class RockManager
{
public:
	//1マップ当たりのサイズ
	static constexpr int MAP_SIZE = 2000;

	//1マップあたりに生成する岩の数
	static constexpr int NUM_CREATE = 30;



	// コンストラクタ
	RockManager(const Transform* target);

	// デストラクタ
	~RockManager(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	//岩生成の中心
	const Transform* mTarget;

	//マップ管理の動的配列
	std::map<IntVector3, std::vector<Rock*>> mMapRocks;

	//岩をランダム生成
	Rock* CreateRandom(IntVector3 mapPos);
};

