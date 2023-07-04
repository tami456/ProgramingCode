#include "AnimeCtl.h"
#include <sstream>
#include "../tmx/rapidxml.hpp"
#include "../tmx/rapidxml_utils.hpp"
#include "../_debug/_DebugConOut.h"
#include "Frame.h"

AnimeCtl AnimeLoader::execute(ActorType type)
{
	auto getFileName = [&]()
	{
		switch (type)
		{
		case ActorType::Player:
			return std::string("Player.anm");
			//break;
		case ActorType::Enemy1:
			return std::string("Enemy1.anm");
			//break;
		case ActorType::Enemy2:
			return std::string("Enemy2.anm");
			//break;
		case ActorType::Bomb:
			return std::string("Bomb.anm");
		default:
			TRACE("存在しないタイプが指定されています");
			return std::string();
		}
	};
	rapidxml::file<> xmlFileData = ("./data/anime/" + getFileName()).c_str();		//xml形式のバイナリデータになる

//解析データをドキュメントに格納する
	rapidxml::xml_document<> xmlDoc;
	xmlDoc.parse<0>(xmlFileData.data());						//xmlFileDataはvector型なので、.data()をつける

	//解析
	auto animationNode = xmlDoc.first_node("Animation");				//先頭のノード
	if (animationNode == nullptr)
	{
		TRACE("animationNode=NULL\n");
	}
	auto version = animationNode->first_attribute("version");			//tilesetのアトリビュート、versionを格納
	TRACE("アニメーションファイル:%s\n", version->value());
	AnimeCtl animeCtl;
	auto imageNode = animationNode->first_node("image");
	if (imageNode == nullptr)
	{
		TRACE("imageNode=NULL\n");
		return animeCtl;
	}

	auto sourceAttr		 = imageNode->first_attribute("source");
	auto tilewidthAttr	 = imageNode->first_attribute("tilewidth");
	auto tileheightAttr	 = imageNode->first_attribute("tileheight");
	auto tilecountAttr	 = imageNode->first_attribute("tilecount");
	auto columnsAttr	 = imageNode->first_attribute("columns");
	auto offsetXAttr	 = imageNode->first_attribute("offsetX");
	auto offsetYAttr	 = imageNode->first_attribute("offsetY");
	auto footPosXAttr = imageNode->first_attribute("footPosX");
	auto footPosYAttr = imageNode->first_attribute("footPosY");

	if (!sourceAttr ||
		!tilewidthAttr || !tileheightAttr ||
		!tilecountAttr || !columnsAttr ||
		!offsetXAttr || !offsetYAttr ||
		!footPosXAttr || !footPosYAttr
		)
	{
		TRACE("imageノードのアトリビュート取得エラー\n");
		return animeCtl;
	}


	animeCtl.source_ = imageNode->first_attribute("source")->value();
	animeCtl.drawOffset_ = { atoi(offsetXAttr->value()), atoi(offsetYAttr->value()) };
	animeCtl.footOffset_ = { atoi(footPosXAttr->value()), atoi(footPosYAttr->value()) };

	animeCtl.imageVec_ = ImageLoader().execute
	(sourceAttr->value(),
		{ atoi(tilecountAttr->value()),atoi(columnsAttr->value()) },
		{ atoi(tilewidthAttr->value()),atoi(tileheightAttr->value()) }
	);
	
	auto seqListNode = animationNode->first_node("SequenceList");
	if (seqListNode == nullptr)
	{
		TRACE("seqListNode=NULL\n");
		return animeCtl;
	}

	auto convState = [&](std::string str) 
	{
		if (str == "Idle")
		{
			return State::Idle;
		}
		if (str == "Run")
		{
			return State::Run;
		}
		if (str == "Dead")
		{
			return State::Dead;
		}
		return State::Max;
	};

	for (auto sequenceNode = seqListNode->first_node();
		sequenceNode != nullptr; sequenceNode = sequenceNode->next_sibling())
	{
		//TRACE("%s\n", sequenceNode->first_attribute("name")->value());
		auto name = sequenceNode->first_attribute("name");
		auto loop = sequenceNode->first_attribute("loop");
		if (!name || !loop)
		{
			continue;		//ループ処理は終わるが、for文に戻る
		}
		auto state = convState(name->value());
		animeCtl.sequence_.try_emplace(convState(name->value()));
		animeCtl.sequence_[state].loopFlag_ = atoi(loop->value());

		auto dataNode = sequenceNode->first_node("data");
		if (dataNode == nullptr)
		{
			TRACE("dataNode=NULL\n");
			continue;
		}
		auto csvNode = dataNode->first_node();

		std::stringstream dataStream;
		dataStream << csvNode->value();
		std::string grID;
		std::string flame;
		while (dataStream.tellg() != -1)
		{
			std::getline(dataStream, grID, ',');
			std::getline(dataStream, flame, ',');
			animeCtl.sequence_[state].table_.emplace_back(std::pair{ atoi(grID.c_str()) ,atoi(flame.c_str()) });
		}
	}

	animeCtl.dir_[NowFlame] = Dir::Down;
	animeCtl.dir_[OldFlame] = Dir::Down;
	animeCtl.state_ = State::Idle;
	
	animeCtl.loopCount_ = 0;
	animeCtl.animePoint_ = 0;
	animeCtl.animeCount_ = 0;
	animeCtl.isAnimeEnd = false;

	return animeCtl;
}

AnimeCtl::AnimeCtl()
{
}

AnimeCtl::~AnimeCtl()
{
}

void AnimeCtl::Draw(const Vector2D& camOffset)
{
	if (!sequence_.count(state_))
	{
		return;
	}
	if (!sequence_[state_].table_.size())
	{
		return;
	}
	auto animeData = sequence_[state_].table_[animePoint_];
	int animeID = animeData.first + static_cast<int>(dir_[NowFlame]);	//表示する画像の要素番号をImageVecの配列用に直したもの

	//animeCount_++;
	//足してからif文が実行される
	if (++animeCount_ > animeData.second)
	{
		animeCount_ = 0;
		if (++animePoint_ >= sequence_[state_].table_.size())
		{
			loopCount_++;
			if (sequence_[state_].loopFlag_ != -1 && loopCount_ >= sequence_[state_].loopFlag_)
			{
				isAnimeEnd = true;
				animePoint_ = static_cast<int>(sequence_[state_].table_.size() - 1);
			}
			else
			{
				animePoint_ = 0;
			}
		}
	}

	if (animeID < imageVec_.size())
	{
		DrawGraph(pos_->x + drawOffset_.x + camOffset.x, pos_->y + drawOffset_.y + camOffset.y, imageVec_[animeID], true);
	}

}

void AnimeCtl::reConnect(Vector2D* pos)
{
	pos_ = pos;
}

State AnimeCtl::SetState(State state)
{
	if (static_cast<int>(state) < static_cast<int>(State::Idle) ||
		static_cast<int>(state) >= static_cast<int>(State::Max))
	{
		return state_;
	}
	if (state_ != state)
	{
		//ステータスが変化した場合、カウンター関連をリセットする
		if (state == State::Dead)
		{
			dir_[NowFlame] = Dir::Down;
		}
		isAnimeEnd = false;
		animeCount_ = 0;
		animePoint_ = 0;
		animeCount_ = 0;
	}
	state_ = state;
	return state_;
}

State AnimeCtl::GetState(void)
{
	return state_;
}

bool AnimeCtl::IsAnimeEnd(void)
{
	return isAnimeEnd;
}
