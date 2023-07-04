#include <DxLib.h>
#include "ImageMng.h"
#include "../_debug/_debug/_DebugConOut.h"
#include "../_debug/_debug/_DebugConOut.h"

//Loadだけを想定した仕組み

//f_nameは取得済みなのでkeyのみ
const VecInt& ImageMng::GetID(std::string key)
{
    if (ImageMap_.count(key) == 0)
    {
        GetID(key, key);
    }
    return ImageMap_[key];
    //あえて返す必要はない
    //抜けた後どうせreturnするので
}

//LoadGraph想定
//一枚の背景など
const VecInt& ImageMng::GetID(std::string f_name, std::string key)
{
    if (ImageMap_.count(key) == 0)
    {
        ImageMap_[key].resize(1);
        ImageMap_[key][0] = LoadGraph(f_name.c_str());
    }
    return ImageMap_[key];
}

//LoadDivGraphを想定した構造体
const VecInt& ImageMng::GetID(std::string f_name, std::string key, Vector2D divSize, Vector2D divCnt)
{
   
    if (ImageMap_.count(key) == 0)
    {
        //サイズ系のintはほぼunsigned
         //掛け算の前にcastしないと溢れてしまう
        ImageMap_[key].resize(static_cast<UINT>(divCnt.x) * static_cast<UINT>(divCnt.y));
        if (LoadDivGraph(f_name.c_str(),
            divCnt.x * divCnt.y,
            divCnt.x, divCnt.y,
            divSize.x, divSize.y,
            &ImageMap_[key][0], true) == -1)
        {
            TRACE("画像の読み込み失敗:%s\n", f_name.c_str());
            
       }
    }
    return ImageMap_[key];
    // TODO: return ステートメントをここに挿入します
}

ImageMng::ImageMng()
{

}

ImageMng::~ImageMng()
{
}
