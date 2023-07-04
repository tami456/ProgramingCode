#pragma once
#include <array>
#include <map>
#include <vector>
#include <list>
//時間系
#include <chrono>
#include "InputID.h"
#include "../Obj/DirLR.h"

enum class Trg
{
	Now,
	Old,
	Max
};

using TrgBool = std::array<bool,static_cast<size_t>(Trg::Max)>;
using CntData = std::map<InputID,TrgBool>;
using ChronoSysClock = std::chrono::system_clock::time_point;

//PlayerCmdの下から見ていく
	//std::vector…キー,時間を格納、std::list…vectorをリストで管理する、std::array…向き(2種類)
	//std::map…技の数分
using CommandMap = std::map<std::string, std::array<std::list<std::vector<int>>, 2>>;
//逆イテレーター … ++したときに後から前に行くときのこと… begin … end - 1のこと
//逆イテレータのbegin … rbeign end … rend
//rbeginに++すると静イテレーターから逆に見える--もしかり
//静イテレーターの時 … beginは最初の位置
//逆を静に入れたりその逆に入れるのも無理
//混在する場合はどっちかで統一してするしかない

class Controller
{
public:

//リングバッファ…コマンドの受付バッファが円状になっているので1周すると上書きされていく
//作り方…お手製の双方向リストを作る
//1要素なのでどんどんnewしていく
	struct RingBuf
	{
		//ボタン情報の保存ができる
		int value = 0;
		//キーを押してからの経過時間
		double time = 0.0;
		//前
		RingBuf* before = nullptr;
		//次
		RingBuf* next = nullptr;

		static RingBuf* Create(int no);
		static void Destroy(RingBuf* buf);
	
	};

	Controller(){};
	virtual ~Controller(){};
	virtual void Update(void) = 0;
	const CntData& GetCntData(void)
	{
		return cntData_;
	}

	std::string CheckCommand(DIR_LR dir,CommandMap& commandMap);	//キャラの向きとコマンドのリストをもらう

private:
	ChronoSysClock startTime_;

protected:
	void WriteCmd(void);
	RingBuf* ringBuf_ = nullptr;
	RingBuf* startBuf_ = nullptr;
	CntData cntData_;

};



//コマンドのマッチング作業…後ろから探した方が速い