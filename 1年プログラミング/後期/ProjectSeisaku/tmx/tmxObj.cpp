#include <dxlib.h>
#include "tmxObj.h"
tmxObj::tmxObj()
{
	virsionTSX_["1.5"] = 1;
	virsionTMX_["1.5"] = 1;		//バージョンが増えてきたら増やす
	//動作確認が取れてなかったら弾く　ホワイトリスト方式
	firstGID_ = 0;
	layerSize_ = 0;
}

tmxObj::~tmxObj()
{
}

bool tmxObj::LoadTSX(std::string fileName)
{
	rapidxml::file<> xmlFileData = fileName.c_str();		//xml形式のバイナリデータになる

	//解析データをドキュメントに格納する
	rapidxml::xml_document<> tsxDoc_;
	tsxDoc_.parse<0>(xmlFileData.data());						//xmlFileDataはvector型なので、.data()をつける

	//解析
	auto tileset = tsxDoc_.first_node("tileset");				//先頭のノード
	auto version = tileset->first_attribute("version");			//tilesetのアトリビュート、versionを格納
	
	//ファイル名:			タグ:image アトリビュート:source="map.png"
	//分割数横・縦:			タグ:image アトリビュート:width="80",height="60"
	//						タグ:tileset アトリビュート:tilewidth="20",tileheight="20"
	//分割サイズ幅・高さ:	タグ:tileset アトリビュート:tilewidth="20",tileheight="20"

	auto imageNode = tileset->first_node("image");				//imageノード
	if (!imageNode)
	{
		return false;
	}
	std::string source = imageNode->first_attribute("source")->value();				//imageノード
	auto tilewidth = atoi(tileset->first_attribute("tilewidth")->value());
	auto tileheight = atoi(tileset->first_attribute("tileheight")->value());
	//map.pngの幅・高さ
	auto divCntX = atoi(imageNode->first_attribute("width")->value()) / tilewidth;
	auto divCntY = atoi(imageNode->first_attribute("height")->value()) / tileheight;

	imageObj_ = ImageLoader().execute(source, Vector2D{ divCntX,divCntY }, Vector2D{tilewidth,tileheight});
	
	if (!virsionTSX_.count(version->value()))
	{
		return false;
	}
	if (virsionTSX_[version->value()] != 1)
	{
		//DrawFormatString(0, 0, 0xff0000, "version:%s\n", version->value());
		//TRACE("version読み込み失敗 現在のversion:%d\n",version);
		return false;
	}
	return true;
}

bool tmxObj::LoadTMX(std::string fileName)
{
	rapidxml::file<> xmlFileData = fileName.c_str();		//xml形式のバイナリデータになる
	rapidxml::xml_document<> tmxDoc_;
	tmxDoc_.parse<0>(xmlFileData.data());					//xmlFileDataはvector型なので、.data()をつける

	//ノードを取得
	//先頭のノードを取得　名前がわからなくても取得することができる
	//while (mapNode)
	//{
	//	if (mapNode->name_size())							//->name_size() : ノード名の文字列長を取得
	//	{
	//		TRACE("%s\n→", mapNode->name());					//->name() : ノード名を取得
	//	}

	auto num = fileName.find_last_of("/");
	std::string pass = fileName.substr(0, num + 1);		//numだけだとdata/tmxになる

	auto mapNode = tmxDoc_.first_node("map");
	if (mapNode == nullptr)
	{
		return false;
	}
	auto tilesetNode = mapNode->first_node("tileset");

	//auto source = tilesetNode->first_attribute("source");
	auto source = mapNode->first_node("tileset")->first_attribute("source");

	if (source->name_size())
	{
		TRACE("%s:->\n", source->name());
		TRACE("%s:\n", source->value());
		TRACE("%s", pass.c_str());
		//TRACE("%s ", source->value());
	}
	LoadTSX(pass + source->value());
//		if (atr->value_size())
	
	auto getAttrInt = [](rapidxml::xml_node<>* node, std::string str) {
		auto attr = node->first_attribute(str.c_str());
		if (attr == nullptr)
		{
			return 0;
		}
		return atoi(attr->value());
	};
	firstGID_ = getAttrInt(tilesetNode,"firstid");

	layerSize_ = getAttrInt(mapNode, "nextlayerid");

	//layerSize_ = GetAtrInt(mapNode->first_node("tileset"),"nextlayerid") - 1;
	worldArea_ = Vector2D{ getAttrInt(mapNode,"width"),getAttrInt(mapNode,"height") };

	tileSize_ = Vector2D{ getAttrInt(mapNode,"tilewidth"),getAttrInt(mapNode,"tileheight") };
	//	mapNode = mapNode->first_node();			//->fist_node() : 自分より下のノードを取得
	//	/apNode = mapNode->next_sibling();		//->next_sibling() : 次の同じ階層のノードを取得
	//}
	
	SetMap(tmxDoc_);
	return false;
}

const Vector2D& tmxObj::GetWorldArea(void)
{
	return worldArea_;
}

const Vector2D& tmxObj::GetTileSize(void)
{
	return tileSize_;
}

void tmxObj::Draw(void)		//テスト用
{
	DrawGraph(0, 0, imageObj_[0], true);
}

bool tmxObj::SetMap(rapidxml::xml_document<>& tmxDoc)
{
	auto mapNode = tmxDoc.first_node("map");
	if (mapNode == nullptr)
	{
		TRACE("mapNode=NULL\n");
		return false;
	}

	for (auto layerNode = mapNode->first_node("layer");		//layerNode階層の一番最初
		layerNode != nullptr;								//layerNodeの次がなくなるまで
		layerNode = layerNode->next_sibling()				//同じ階層の次のノードを取得
		)
	{

		if (layerNode == nullptr)
		{
			TRACE("layerNode=NULL\n");
			return false;
		}

		auto layerName = layerNode->first_attribute("name");
		if (layerName == nullptr)
		{
			TRACE("layerName=NULL\n");
			return false;
		}

		//中身を見ていく
		//結果を入れる
		auto result = mapData_.try_emplace(layerName->value());//場所だけ用意

		//参照型のauto　推論してくれないので、明示的に書く
		auto& mapPair = *result.first;
		mapPair.second.resize(static_cast<size_t>(worldArea_.x)* static_cast<size_t>(worldArea_.y));		//std::vectorのintが取り出せる

		auto dataNode = layerNode->first_node("data");
		if (dataNode == nullptr)
		{
			TRACE("dataNode=NULL\n");
			return false;
		}

		auto csvNode = dataNode->first_node();//タグがないので、ノード名は書かない
		TRACE("%s", csvNode->value());

		std::stringstream dataStream;
		dataStream << csvNode->value();
		//直接入れたいので、参照のauto
		for (auto& data : mapPair.second)
		{
			std::string numStr;
			//使用する文字列,格納先,分割する文字(カンマで区切って分割、格納先に格納する)
			std::getline(dataStream, numStr, ',');

			data = atoi(numStr.c_str())-1;	//データを格納しない段階でマイナス１する
		}
	}


	return false;
}

void tmxObj::DrawMapData(std::string layer, int x, int y, const Vector2D& camOffset)
{
	//mapData_には１番以降にデータが入っている
	int point = x + y * worldArea_.x;

	if (!mapData_.count(layer))
	{
		TRACE("指定したlayerが存在しない\n");
		return;
	}
	if (x < 0 || x > worldArea_.x ||
		y < 0 || y > worldArea_.y)
	{
		TRACE("xまたはyが範囲外\n");
		return;
	}
	if (point < 0 || point >= mapData_[layer].size())
	{
		TRACE("pointが０以下\n");
		return;
	}
	if (mapData_[layer][point] < 0)
	{
		return;
	}
	DrawGraph(x * tileSize_.x + camOffset.x, y * tileSize_.y + camOffset.y, imageObj_[mapData_[layer][point]], true);
}

const MapData& tmxObj::GetMapData(void)
{
	return mapData_;
}

const int tmxObj::GetMapData(std::string layer, const Vector2D& pos)
{
	Vector2D grid = pos / tileSize_;

	return GetMapData(layer,grid.x,grid.y);
}

const int tmxObj::GetMapData(std::string layer, int x, int y)
{
	int point = x + y * worldArea_.x;
	if (!mapData_.count(layer))
	{
		TRACE("SetMapData:指定したlayerが存在しない\n");
		return -1;
	}
	if (x < 0 || x > worldArea_.x ||
		y < 0 || y > worldArea_.y)
	{
		TRACE("SetMapData:xまたはyが範囲外\n");
		return -1;
	}
	if (point < 0 || point >= mapData_[layer].size())
	{
		TRACE("SetMapData:pointが０以下\n");
		return -1;
	}
	return mapData_[layer][point];
}

bool tmxObj::SetMapData(int id, std::string layer, Vector2D pos)
{
	Vector2D grid = pos / tileSize_;
	
	return SetMapData(id, layer, grid.x,grid.y);
}

bool tmxObj::SetMapData(int id, std::string layer, int x, int y)
{
	int point = x + y * worldArea_.x;
	if (!mapData_.count(layer))
	{
		TRACE("SetMapData:指定したlayerが存在しない\n");
		return false;
	}
	if (x < 0 || x > worldArea_.x ||
		y < 0 || y > worldArea_.y)
	{
		TRACE("SetMapData:xまたはyが範囲外\n");
		return false;
	}
	if (point < 0 || point >= mapData_[layer].size())
	{
		TRACE("SetMapData:pointが０以下\n");
		return false;
	}
	if (mapData_[layer][point] < 0)
	{
		return false;
	}

	mapData_[layer][point] = id;
	return true;
}

Vec2List tmxObj::SearchID(MapID mapID, std::string layer)
{
	Vec2List vec2List;
	if (!mapData_.count(layer))
	{
		return std::move(vec2List);
	}
	int id = static_cast<int>(mapID);
	for (int point = 0; point < mapData_[layer].size(); point++)
	{
		if (mapData_[layer][point] == id)
		{
			vec2List.emplace_back(point % worldArea_.x, point / worldArea_.x);
		}
	}
	return std::move(vec2List);
}

//int tmxObj::GetAtrInt(rapidxml::xml_node<>* node, std::string str)
//{
//
//	//return (attr->next_attribute());
//	return 0;
//}
