#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	// �������܂��񂪁A�ǂ����Ă��g����������
	using RES = Resource;
	using RES_T = RES::TYPE;
	static std::string PATH_IMG = Application::PATH_IMAGE;
	static std::string PATH_MDL = Application::PATH_MODEL;
	static std::string PATH_EFF = Application::PATH_EFFECT;

	std::unique_ptr<Resource> res;

	// �^�C�g���摜
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "Title.png");
	resourcesMap_.emplace(SRC::TITLE, std::move(res));

	// PushSpace
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "PushSpace.png");
	resourcesMap_.emplace(SRC::PUSH_SPACE, std::move(res));

	// �����o��
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "SpeechBalloon.png");
	resourcesMap_.emplace(SRC::SPEECH_BALLOON, std::move(res));
	
	// �v���C���[
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Player/Player.mv1");
	resourcesMap_.emplace(SRC::PLAYER, std::move(res));

	// �v���C���[�e
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "Shadow.png");
	resourcesMap_.emplace(SRC::PLAYER_SHADOW, std::move(res));

	// �X�J�C�h�[��
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "SkyDome/SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, std::move(res));

	// �ŏ��̘f��
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Planet/MainPlanet.mv1");
	resourcesMap_.emplace(SRC::MAIN_PLANET, std::move(res));

	// ���Ƃ����̘f��
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Planet/FallPlanet.mv1");
	resourcesMap_.emplace(SRC::FALL_PLANET, std::move(res));

	// ���R�Șf��01
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Planet/FlatPlanet01.mv1");
	resourcesMap_.emplace(SRC::FLAT_PLANET_01, std::move(res));

	// ���R�Șf��02
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Planet/FlatPlanet02.mv1");
	resourcesMap_.emplace(SRC::FLAT_PLANET_02, std::move(res));

	// �Ō�̘f��
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Planet/LastPlanet.mv1");
	resourcesMap_.emplace(SRC::LAST_PLANET, std::move(res));

	// ���ʂȘf��
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Planet/RoadPlanet.mv1");
	resourcesMap_.emplace(SRC::SPECIAL_PLANET, std::move(res));

	// ����
	res = std::make_unique<RES>(RES_T::EFFEKSEER, PATH_EFF + "Smoke/Smoke.efkefc");
	resourcesMap_.emplace(SRC::FOOT_SMOKE, std::move(res));

	// ���[�v�X�^�[���f��
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Star/star.mv1");
	resourcesMap_.emplace(SRC::WARP_STAR, std::move(res));

	// ���[�v�X�^�[�p��]�G�t�F�N�g
	res = std::make_unique<RES>(RES_T::EFFEKSEER, PATH_EFF + "StarDust/StarDust.efkefc");
	resourcesMap_.emplace(SRC::WARP_STAR_ROT_EFF, std::move(res));

	// ���[�v�̋O�Ր�
	res = std::make_unique<RES>(RES_T::EFFEKSEER, PATH_EFF + "Warp/WarpOrbit.efkefc");
	resourcesMap_.emplace(SRC::WARP_ORBIT, std::move(res));

	// �u���b�N�z�[��
	res = std::make_unique<RES>(RES_T::EFFEKSEER, PATH_EFF + "BlackHole/BlackHole.efkefc");
	resourcesMap_.emplace(SRC::BLACK_HOLE, std::move(res));

	// �S�[��
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "GoalStar/GoalStar.mv1");
	resourcesMap_.emplace(SRC::GOAL_STAR, std::move(res));

	// Clear
	res = std::make_unique<RES>(RES_T::IMG, PATH_IMG + "Congratulations.png");
	resourcesMap_.emplace(SRC::CLEAR, std::move(res));

	// �^���N
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Tank/Body.mv1");
	resourcesMap_.emplace(SRC::TANK_BODY, std::move(res));
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Tank/Wheel.mv1");
	resourcesMap_.emplace(SRC::TANK_WHEEL, std::move(res));
	res = std::make_unique<RES>(RES_T::MODEL, PATH_MDL + "Tank/Barrel.mv1");
	resourcesMap_.emplace(SRC::TANK_BARREL, std::move(res));

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

const Resource& ResourceManager::Load(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource& ResourceManager::_Load(SRC src)
{

	// ���[�h�ς݃`�F�b�N
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	// ���\�[�X�o�^�`�F�b�N
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// �o�^����Ă��Ȃ�
		return dummy_;
	}

	// ���[�h����
	rPair->second->Load();

	// �O�̂��߃R�s�[�R���X�g���N�^
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
