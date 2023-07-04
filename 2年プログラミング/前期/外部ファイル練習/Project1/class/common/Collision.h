#pragma once
#include <list>
#include "../Vector2D.h"

using Collision = std::pair<Vector2DDouble,Vector2DDouble>;

//何個もあるコリジョンをリストで管理する
using ColList = std::list<Collision>;

//何も書いていない行を最後の行にしておいた方がいい　＝　EOF
