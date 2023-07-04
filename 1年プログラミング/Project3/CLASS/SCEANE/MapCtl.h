#pragma once
#include "../COMMON/Vector2.h"
#include "Common.h"

#define BLOCK_PTN_MAX (7)
#define BLOCK_COUNT_INV (3)
#define BLOCK_MAX_SIZE_CNT ((BLOCK_PTN_MAX - 1) * BLOCK_COUNT_INV)


class MapCtl
{
public:
	MapCtl();
	~MapCtl();
	void Update(void);
	void Draw(void);
	bool Init(void);
	bool Release(void);
	

	//指定した座標にブロックを設置する
	int SetBlock(Vector2 pos, int cnt);
	int SetBlock(Vector2 pos);

	//指定した座標のブロックの状態を取得する
	int GetBlock(Vector2 pos);

	//ブロックのサイズを返す
	int GetBlockSize(void);

	//最大サイズになっているブロックに接触しているかを取得する
	bool isOnBlock(Vector2 pos);
private:
	int blockImage_[BLOCK_PTN_MAX];

	int mapData_[MAP_CHIP_COUNT_Y][MAP_CHIP_COUNT_X];


};

