#pragma once
#include <string>
#include <map>
#include "../Actor/State.h"
#include "../Actor/ActorType.h"
#include "../Actor/Dir.h"
#include "../tmx/rapidxml.hpp"
#include "../tmx/rapidxml_utils.hpp"
#include "Vector2D.h"
#include "ImageCtl.h"

using AnimeData = std::pair<int, int>;		//first:グラフィックハンドル　second:維持フレーム
using SequenceTable = std::vector<AnimeData>;

struct SequenceData
{
	int loopFlag_;
	SequenceTable table_;
};

class AnimeCtl
{
public:
	//世に込まれたデータを再生するクラス
	AnimeCtl();
	~AnimeCtl();
	void Draw(void);
	void reConnect(Vector2D* pos);
	State SetState(State state);
	State GetState(void);
	Vector2D GetFootPos(void) { return *pos_; }
	bool IsAnimeEnd(void);

	Dir dir_[2];		//0:現在のフレームの方向　1:１フレーム前の方向
private:
	State state_;
	int loopCount_;		//現在のループ回数
	int animePoint_;	//現在のtable_の再生位置
	int animeCount_;	//現在のテーブルの再生位置での経過フレーム
	bool isAnimeEnd;	//アニメーションの終了フラグ
	ImageVec imageVec_;
	std::string source_;
	Vector2D* pos_;
	std::map<State, SequenceData> sequence_;
	friend struct AnimeLoader;		//お前のものは俺のもの アクセス制限を突破してアクセスできる　多用しない
};

struct AnimeLoader
{
	AnimeCtl execute(ActorType type);
};
