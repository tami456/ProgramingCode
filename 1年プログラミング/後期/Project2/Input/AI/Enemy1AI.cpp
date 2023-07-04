#include "Enemy1AI.h"
#include "../../common/Frame.h"
#include "../../Scene/GameScene.h"

Enemy1AI::Enemy1AI()
{
}

Enemy1AI::~Enemy1AI()
{
}



void Enemy1AI::Update(BaseScene& scene, const Actor* own)
{
	memcpy(inputData_[OldFlame].data(), inputData_[NowFlame].data(), sizeof(inputData_[OldFlame][NowFlame]) * inputData_[OldFlame].size());
	memset(inputData_[NowFlame].data(), 0, inputData_[NowFlame].size() * sizeof(inputData_[NowFlame][0]));

	auto actorList = dynamic_cast<GameScene&>(scene).GetActorList();

	sharedActor player;
	for (auto actor : actorList)
	{
		if (actor->GetActorType() == ActorType::Player)
		{
			player = actor;
			break;
		}
	}
	if (!player)
	{
		return;
	}

	if (own->GetPos().x < player->GetPos().x)
	{
		inputData_[NowFlame][static_cast<int>(InputID::Right)] = 1;
	}
	if (own->GetPos().x > player->GetPos().x)
	{
		inputData_[NowFlame][static_cast<int>(InputID::Left)] = 1;
	}
	if (own->GetPos().y > player->GetPos().y)
	{
		inputData_[NowFlame][static_cast<int>(InputID::Up)] = 1;
	}
	if (own->GetPos().y < player->GetPos().y)
	{
		inputData_[NowFlame][static_cast<int>(InputID::Down)] = 1;
	}


	//for (auto id : InputID())
	//{
	//	inputData_[NowFlame][static_cast<int>(id)] = (padData & keyMap_[id]);		//>Ç≈èoÇΩåãâ ÇÕ0Ç©1Ç™ì¸ÇÈ
	//}
}
