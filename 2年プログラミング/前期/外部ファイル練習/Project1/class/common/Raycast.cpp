#include <DxLib.h>
#include "../../_debug/_debug/_DebugConOut.h"
#include "../../_debug/_debug/_DebugDispOut.h"
#include "Raycast.h"

//判定をする
//Lineを作ってCheckRayに渡す
bool Raycast::CheckCollision(Ray ray, Collision collision)
{
    Raycast::Line lines[4] = {
        //上の辺
        {
            collision.first,
            collision.first + Vector2DDouble{collision.second.x,0}
        },
        //右の辺
        {
            collision.first + Vector2DDouble{collision.second.x,0},
            collision.first + collision.second
         },
        //下の辺
         {
             collision.first + collision.second,
             collision.first + Vector2DDouble{0,collision.second.y}
         },
            //左の辺
         {
             collision.first + Vector2DDouble{0,collision.second.y},
             collision.first
         }
    };

    bool result = false;
    for (auto line:lines)
    {
        result |= CheckRay(ray, line);
    }
    return result;
}

//CheckCollision側から内部的に呼び出す
bool Raycast::CheckRay(Ray ray, Line line)
{
    _dbgDrawLine(
        static_cast<int>(line.p.x),
        static_cast<int>(line.p.y),
        static_cast<int>(line.end.x),
        static_cast<int>(line.end.y),0xff0000);

    auto checkCross = [](Vector2DDouble& vec1, Vector2DDouble& vec2)
    {
        //外積を取っていく
        //外積の結果をリターン
        return (vec1.x * vec2.y) - (vec2.x * vec1.y);
    };
    //平行を除外していく
    auto vecLine = line.end - line.p;
    auto crossRayLine = checkCross(ray.vec, vecLine);
    if (crossRayLine == 0.0)
    {
        //平行なのでfalseを返す
        //当たらないので計算の無駄
        //外積の結果が0の場合は平行
        //平行時は絶対に交差しないので弾く
        return false;
    }
    Vector2DDouble v = line.p - ray.p;
      
    auto cross_vRay = checkCross(v,ray.vec);
    auto cross_vLine = checkCross(v, vecLine);

    auto t1 = cross_vRay / crossRayLine;
    auto t2 = cross_vLine / crossRayLine;

    if (t1 > 0.0 && t1 <= 1.0 && t2 > 0.0 && t2 <= 1.0)
    {
        //交差していたらtrue
        return true;
    }

    return false;
}
