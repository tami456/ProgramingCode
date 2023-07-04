#pragma once
#include <memory>
#include <list>
#include "../common/ImageCtl.h"
#include "../common/Vector2D.h"
#include "ActorType.h"
#include "../common/AnimeCtl.h"




class tmxObj;
class FireMap;
class BaseScene;
class Actor;

using BombPair = std::pair<bool, Vector2D>;
using sharedActor = std::shared_ptr<Actor>;
using ActorList = std::list<sharedActor>;

class Actor
{
public:
	Actor();
	Actor(ActorType type ,Vector2D pos);
	virtual ~Actor();
	virtual void Update(BaseScene& scene) = 0;	//èäóLå†ÇéùÇΩÇ»Ç¢
	void Draw(const Vector2D& camOffset);
	void LoadAnime(void);
	const Vector2D& GetPos(void)const { return pos_; }
	const Vector2D GetFootPos(void) { return animeCtl_.GetFootPos(); }
	bool IsAlive(void) { return alive_; }
	BombPair IsBomb(void) { return bombPair_; }
	ActorType GetActorType(void) { return type_; }
private:
protected:
	bool CheckErase(void);
	AnimeCtl animeCtl_;
	Vector2D pos_;
	int speed_;		//à⁄ìÆó 
	ActorType type_;
	bool alive_;
	BombPair bombPair_;
	friend class GameScene;
};

