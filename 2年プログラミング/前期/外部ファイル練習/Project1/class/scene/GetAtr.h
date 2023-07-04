#pragma once
#include <string>
#include "../rapidxml/rapidxml.hpp"
#include "../rapidxml/rapidxml_utils.hpp"
class GetAtr 
{
public:
	//最初の()はどのオペレーターをオーバーライドするか
	//2番目の()はオーバーライドした関数呼び出しの()に対しての引数
	bool operator()(rapidxml::xml_node<>* node,std::string atrName, int& num)
	{
		if (node == nullptr)
		{
			return false;
		}
		auto atr = node->first_attribute(atrName.c_str());
		if (atr == nullptr)
		{
			return false;
		}
		num = atoi(atr->value());
		return true;
	}

	bool operator()(rapidxml::xml_node<>* node, std::string atrName, unsigned int& str)
	{
		if (node == nullptr)
		{
			return false;
		}
		auto atr = node->first_attribute(atrName.c_str());
		if (atr == nullptr)
		{
			return false;
		}
		str = atoi(atr->value());
		return true;
	}

	bool operator()(rapidxml::xml_node<>* node, std::string atrName, double& num)
	{
		if (node == nullptr)
		{
			return false;
		}
		auto atr = node->first_attribute(atrName.c_str());
		if (atr == nullptr)
		{
			return false;
		}
		num = atof(atr->value());
		return true;
	}

	bool operator()(rapidxml::xml_node<>* node, std::string atrName, std::string& str)
	{
		if (node == nullptr)
		{
			return false;
		}
		auto atr = node->first_attribute(atrName.c_str());
		if (atr == nullptr)
		{
			return false;
		}
		str = atr->value();
		return true;
	}

};
