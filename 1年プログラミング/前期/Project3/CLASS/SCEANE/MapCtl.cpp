#include <DxLib.h>
#include "MapCtl.h"
#include "../../_debug/_DebugConOut.h"

#define BLOCK_SIZE 32

MapCtl::MapCtl()
{
}

MapCtl::~MapCtl()
{
}

void MapCtl::Update(void)
{
	for (int y = 0; y < MAP_CHIP_COUNT_Y; y++)
	{
		for (int x = 0; x < MAP_CHIP_COUNT_X; x++)
		{
			if ((mapData_[y][x] > 0) && (mapData_[y][x] < BLOCK_MAX_SIZE_CNT))
			{
				mapData_[y][x]++;
			}
		}
	}
}

void MapCtl::Draw(void)
{
	for (int y = 0; y < MAP_CHIP_COUNT_Y; y++)
	{
		for (int x = 0; x < MAP_CHIP_COUNT_X; x++)
		{
			int mapID = mapData_[y][x] / BLOCK_COUNT_INV;
			mapID = (mapID < BLOCK_PTN_MAX ? mapID : BLOCK_PTN_MAX - 1);
			DrawGraph(x * BLOCK_SIZE, y * BLOCK_SIZE, blockImage_[mapID], true);
		}
	}
	
}

bool MapCtl::Init(void)
{
	bool rtnFlag = true;
	if (LoadDivGraph(
		"Resource/image/block.png",
		BLOCK_PTN_MAX,
		BLOCK_PTN_MAX,
		1,
		BLOCK_SIZE,
		BLOCK_SIZE,
		blockImage_
	) == -1)
	{
		TRACE("ƒuƒƒbƒN‚Ì“Ç‚Ýž‚ÝŽ¸”s\n");
		rtnFlag = false;
	}
	memset(mapData_, 0, sizeof(mapData_));
	/*for (int y = 0; y < MAP_CHIP_COUNT_Y; y++)
	{
		for (int x = 0; x < MAP_CHIP_COUNT_X; x++)
		{
			mapData_[y][x];
		}
	}*/

	return rtnFlag;
}

bool MapCtl::Release(void)
{
	for (int no = 0; no < BLOCK_PTN_MAX; no++)
	{
		DeleteGraph(blockImage_[no]);
	}
	return true;
}

int MapCtl::SetBlock(Vector2 pos, int cnt)
{
	Vector2 chipPos{pos.x_ / BLOCK_SIZE,pos.y_ / BLOCK_SIZE};
	if (chipPos.x_ < 0 ||
		chipPos.x_ >= MAP_CHIP_COUNT_X ||
		chipPos.y_ < 0 ||
		chipPos.y_ >= MAP_CHIP_COUNT_Y)
	{
		return 0;
	}
	mapData_[chipPos.y_][chipPos.x_] = cnt;
	
	return mapData_[chipPos.y_][chipPos.x_];
}

int MapCtl::SetBlock(Vector2 pos)
{
	int cnt = GetBlock(pos);
	if (cnt == 0)
	{
		cnt = SetBlock(pos, 1);
	}
	return cnt;
}

int MapCtl::GetBlock(Vector2 pos)
{
	Vector2 chipPos{ pos.x_ / BLOCK_SIZE,pos.y_ / BLOCK_SIZE };
	if (chipPos.x_ < 0 ||
		chipPos.x_ >= MAP_CHIP_COUNT_X ||
		chipPos.y_ < 0 ||
		chipPos.y_ >= MAP_CHIP_COUNT_Y)
	{
		return 0;
	}
	return mapData_[chipPos.y_][chipPos.x_];
}

int MapCtl::GetBlockSize(void)
{
	return BLOCK_SIZE;
}

bool MapCtl::isOnBlock(Vector2 pos)
{
	return (GetBlock(pos) >= BLOCK_MAX_SIZE_CNT);
}
