#pragma once
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include "../rapidxml/rapidxml.hpp"
#include "../rapidxml/rapidxml_utils.hpp"
#include "../ImageMng.h"
#include "../Vector2D.h"
#include "../common/Collision.h"
#include "../common/Raycast.h"

using MapData = std::map<std::string, std::vector<int>>;

class TmxObj
{
public:
	//Tmxの役割rapidxmlを通じてxmlを読み込む
	//開くのはrapidxml…パーサー(rapidxml)
	//Load部分はTmxObj…ローダー(TmxObj)
	TmxObj();
	~TmxObj();

	bool LoadTMX(std::string fileName);
	bool LoadTSX(std::string fileName);
	bool SetMap(rapidxml::xml_node<>* node);

	const ColList& GetColList(void);
	const MapData& GetMapData(void);
	const Vector2D& GetWorldArea(void);
	const Vector2D& GetTileSize(void);
	const std::string& GetMapKey(void);
	Raycast raycast_;

private:
	//rapidxmlの構成　ドキュメント(ファイル(TMX))->ノード(タグ)->アトリビュート

	//TSX
	//テンプレート
	//定義の方でchar型として定義されているので<>でもエラーは出ない(暗黙的)
	rapidxml::xml_document<> tsxDoc_;
	

	//TMX
	//-値がない
	unsigned int layerSize_;
	Vector2D worldArea_;
	Vector2D tileSize_;
	ColList colList_;

	//stringはクラスなので起動が遅いので避けた方がいい
	MapData mapData_;
	std::string mapKey_;
};

