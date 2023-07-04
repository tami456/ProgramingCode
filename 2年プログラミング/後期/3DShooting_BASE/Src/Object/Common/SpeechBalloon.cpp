#include <DxLib.h>
#include "Transform.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource.h"
#include "../../Manager/SceneManager.h"
#include "../../Utility/AsoUtility.h"
#include "SpeechBalloon.h"

SpeechBalloon::SpeechBalloon(TYPE type, const Transform* parent)
{

	mTransformParent = parent;

	mType = type;

	Resource res = ResourceManager::GetInstance().Load(ResourceManager::SRC::SPEECH_BALLOON);
	mImage = res.mHandleId;

	mIsVisiblePermanent = true;
	mVisibleTime = -1.0f;

	mPos = { 0.0f, 0.0f, 0.0f };
	mRelativePos = { 0.0f, 0.0f, 0.0f };

}

SpeechBalloon::~SpeechBalloon(void)
{
}

void SpeechBalloon::Draw(void)
{

	if (!mIsVisiblePermanent)
	{
		mVisibleTime -= SceneManager::GetInstance().GetDeltaTime();
		if (mVisibleTime < 0.0f)
		{
			return;
		}
	}

	// 対象物の姿勢から見た、上方向に3D座標をずらす
	//親オブジェクトの回転姿勢(クォータニオン)を考慮して、
	//開店した相対座標を取得する
	VECTOR relativePos = mTransformParent->quaRot.PosAxis(mRelativePos);

	//相対座標をワールド座標に変換するために、
	//親オブジェクトの座標に相対座標を加える
	VECTOR pos = VAdd(mTransformParent->pos, relativePos);

	//ワールドオブジェクトから、スクリーン座標に変換する
	//おすすめ
	mPos = ConvWorldPosToScreenPos(pos);

	switch (mType)
	{
	case SpeechBalloon::TYPE::TEXT:
		DrawTextOrg();
		break;
	case SpeechBalloon::TYPE::SPEECH:
		DrawSpeech();
		break;
	}

}

void SpeechBalloon::DrawTextOrg(void)
{

	// カメラのSetCameraNearFarから外れていた場合、表示しない
	if (mPos.z > 0.0f && mPos.z < 1.0f)
	{
		DrawFormatString(
			static_cast<int>(mPos.x), static_cast<int>(mPos.y),
			0xdd0000, "%s", mText.c_str());
	}

}

void SpeechBalloon::DrawSpeech(void)
{

	// カメラのSetCameraNearFarから外れていた場合、表示しない
	if (mPos.z > 0.0f && mPos.z < 1.0f)
	{
		int x = AsoUtility::Round(mPos.x);
		int y = AsoUtility::Round(mPos.y);
		DrawRotaGraph(x, y, 0.5f, 0.0f, mImage, true);
		int len = (int)strlen(mText.c_str());
		int width = GetDrawStringWidth(mText.c_str(), len);
		DrawFormatString(x - (width / 2), y - 15, 0x000000, "%s", mText.c_str());
	}

}

void SpeechBalloon::Release(void)
{
}

void SpeechBalloon::SetText(const std::string& text)
{
	mText = text;
}

void SpeechBalloon::SetTime(double time)
{
	mIsVisiblePermanent = false;
	mVisibleTime = time;
}

void SpeechBalloon::SetType(TYPE type)
{
	mType = type;
}

void SpeechBalloon::SetVisiblePermanent(void)
{
	mIsVisiblePermanent = true;
}

void SpeechBalloon::SetRelativePos(VECTOR relativePos)
{
	mRelativePos = relativePos;
}
