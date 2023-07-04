#include "TmxObj.h"
#include "../ImageMng.h"
#include "GetAtr.h"
#include "../../_debug/_debug/_DebugDispOut.h"
#include "../../_debug/_debug/_DebugConOut.h"

TmxObj::TmxObj()
{
	
}

TmxObj::~TmxObj()
{

}

//fileNameの中にtmxまでの相対パスも入っている
bool TmxObj::LoadTMX(std::string fileName)
{
	rapidxml::file<> xmlFileData = fileName.c_str();
	tsxDoc_.parse<0>(xmlFileData.data());

	rapidxml::xml_node<>* mapNode = tsxDoc_.first_node("map");
	if (mapNode == nullptr)
	{
		return false;
	}

	auto tilesetNode = mapNode->first_node("tileset");
	if (tilesetNode == nullptr)
	{
		return false;
	}

	auto tsxSource = tilesetNode->first_attribute("source");
	if (tsxSource == nullptr)
	{
		return false;
	}

	//auto getAtr = [&](std::string atrName, auto& value) {
	//	auto atrStr = mapNode->first_attribute(atrName.c_str());
	//	if (atrStr == nullptr)
	//	{
	//		TRACE("アトリビュートの取得エラー : %s",atrName.c_str());
	//		return false;
	//	}
	//	//文字列をintに変換
	//	value = atoi(atrStr->value());
	//	return true;
	//};

	auto getAtr = GetAtr();
	if (!getAtr(mapNode,"nextlayerid", layerSize_))
	{
		return false;
	}
	//nextlayeridの値はレイヤー総数からすると+1されているため減算
	layerSize_--;
	if (!getAtr(mapNode, "width", worldArea_.x))
	{
		return false;
	}
	if (!getAtr(mapNode, "height", worldArea_.y))
	{
		return false;
	}
	if (!getAtr(mapNode, "tilewidth", tileSize_.x))
	{
		return false;
	}
	if (!getAtr(mapNode, "tileheight", tileSize_.y))
	{
		return false;
	}
	//欲しいものは実行物からの相対パス
	//substr = 文字列の指定した場所から場所までの文字列の指定(最初の文字は0文字換算)
	//検索の仕方は一番最初に見つけた文字と一番最後に見つけた文字の二つがある
	//取得したい文字（"./tmx/map.tmx"）の二つ目の/
	//find_last_of = 最後に現れる指定文字列を検索する
	
	auto filePass = fileName.substr(0, fileName.find_last_of("/") + 1);

	//std::string tsxStr = tsxSource->value();
	
	//TMXからTSXへの接続
	LoadTSX(filePass + tsxSource->value());



	/*if (!SetMap(mapNode))
	{
		return false;
	}*/

	//これ以上下に書くものがなければリダイレクトでも良い
	return SetMap(mapNode);
}

bool TmxObj::LoadTSX(std::string fileName)
{
	//これでファイルを開ける(開いただけ)
	rapidxml::file<> xmlFile = fileName.c_str();
	
	//ポインタを取った
	//xmlFile.data();
	
	//ポインタを渡した
	tsxDoc_.parse<0>(xmlFile.data());
	rapidxml::xml_node<>* tilesetNode = tsxDoc_.first_node("tileset");
	if (tilesetNode == nullptr)
	{
		return false;
	}
	//autoでも良し
	//("")の中身がわからない場合は頭の方から読み込んでいく
	rapidxml::xml_attribute<>* version = tilesetNode->first_attribute("version");
	TRACE("TSX Version:%s\n", version->value());

	rapidxml::xml_attribute<>* tiledversion = tilesetNode->first_attribute("tiledversion");
	TRACE("TSX TiledVersion:%s\n", tiledversion->value());

	//構造を見せたいときにラムダ式の方がいいかも
	//auto getAtr = [&](std::string atrName,int& num) {
	//	auto atr = tilesetNode->first_attribute(atrName.c_str());
	//	if (atr == nullptr)
	//	{
	//		return false;
	//	}
	//	num = atoi(atr->value());
	//	return true;
	//};
	
	int tilewidth;
	int tileheight;
	int tilecount;
	int columns;

	//実体を作ってから引数を渡す
	auto getAtr = GetAtr();
	
	//1回使い捨ての場合はif (!GetAtr()("tilewidth", tilewidth))
	if (!getAtr(tilesetNode,"tilewidth", tilewidth))
	{
		return false;
	}
	if (!getAtr(tilesetNode, "tileheight", tileheight))
	{
		return false;
	}
	if (!getAtr(tilesetNode, "tilecount", tilecount))
	{
		return false;
	}
	if (!getAtr(tilesetNode, "columns", columns))
	{
		return false;
	}


	rapidxml::xml_node<>* imageNode = tilesetNode->first_node("image");
	if (imageNode == nullptr)
	{
		return false;
	}
	rapidxml::xml_attribute<>* source = imageNode->first_attribute("source");
	if (source == nullptr)
	{
		return false;
	}
	
	std::string sourcePass = source->value();
	//auto sourceName = "./tmx" + sourcePass.substr(1);
	mapKey_ = "mapChip";
	//mapファイルらへんがおかしいかも！！！！！！！！！！！！！！！！！！！！
	lpImageMng.GetID("./tmx" + sourcePass.substr(1), mapKey_, { tilewidth ,tileheight },{ columns,tilecount/ columns });
	  
	return true;
}

bool TmxObj::SetMap(rapidxml::xml_node<>* node)
{
	if (node == nullptr)
	{
		return false;
	}
	
	auto tileset = node->first_node("tileset");
	if (tileset == nullptr)
	{
		return false;
	}

	auto getAttr = GetAtr();
	int firstgid;
	getAttr(tileset,"firstgid", firstgid);

	//layerループ , nullじゃなかったら抜けてnextに行く
	for (auto layerNode = node->first_node("layer");
		layerNode != nullptr;
		layerNode = layerNode->next_sibling())
	{
		
		if (layerNode == nullptr)
		{
			return false;
		}
		

		std::string layerName;
		getAttr(layerNode, "name", layerName);
		if (layerName == "Col")
		{
			//Colだったら処理を中断して上に行ってnextする
			continue;
		}

		Vector2D layerSize;
		getAttr(layerNode, "width", layerSize.x);
		getAttr(layerNode, "height", layerSize.y);
		

		//比べて違かったら
		if (worldArea_ != layerSize)
		{
			return false;
		}

		
		auto layerData = mapData_.try_emplace(layerName);

		//firstがキー secondがデータ
		if (layerData.second)
		{
			//要素を入れるための箱
			layerData.first->second.resize(worldArea_.x * worldArea_.y);
		}
		auto data = layerNode->first_node("data");
		if (data == nullptr)
		{
			return false;
		}
		auto csvData = data->first_node();

		std::stringstream dataStream;

		//=と同じ
		dataStream << csvData->value();


		//std::vectorの1要素ずつ取り出すfor分
		for (auto& vecData : layerData.first->second)
		{
			std::string numStr;

			//読み込む文字,読み込んだ文字を入れる場所,文字の区切り
			getline(dataStream, numStr, ',');

			//これで1文字取り込んで保存を繰り返せる
			//このままだとずれてしまうのでfirstgid分引く
			vecData = atoi(numStr.c_str()) - firstgid;
		}
	}
	for (auto colNode = node->first_node("objectgroup");
		colNode != nullptr;
		colNode = colNode->next_sibling())
	{
		std::string grpName;
		getAttr(colNode, "name", grpName);
		if (grpName != "Col")
		{
			continue;
		}

		for (auto objNode = colNode->first_node("object");
			objNode != nullptr;
			objNode = objNode->next_sibling())
		{
			double x, y, w, h;
			getAttr(objNode, "x", x);
			getAttr(objNode, "y", y);
			getAttr(objNode, "width", w);
			getAttr(objNode, "height", h);
			colList_.push_back(
				Collision{
					//firstにx,y
					Vector2DDouble{x,y},
					//secondにw,hを入れる
					Vector2DDouble{w,h},
				}
			);

			//GetAtrのdoubleのatoiをatofに変更
			//浮動小数点に変換してVector2Ddoubleに格納してコリジョンの状態してpushbackでコルリストに入れる
		}

	}
	return true;
}


const ColList& TmxObj::GetColList(void)
{
	return colList_;
}

const MapData& TmxObj::GetMapData(void)
{
	return mapData_;
}

const Vector2D& TmxObj::GetWorldArea(void)
{
	return worldArea_;
}

const Vector2D& TmxObj::GetTileSize(void)
{
	return tileSize_;
}

const std::string& TmxObj::GetMapKey(void)
{
	return mapKey_;
}

//UTF-8という文字コードを読み込んでいるという意味
//encoding = UTF-8