#pragma once
#include <string>
#include <DxLib.h>
#include "Transform.h"
class SceneManager;

class SpeechBalloon
{

public:

	// �\�����
	enum class TYPE
	{
		TEXT,
		SPEECH
	};

	// �R���X�g���N�^
	SpeechBalloon(TYPE type, const Transform& parent);

	// �f�X�g���N�^
	~SpeechBalloon(void);

	void Draw(void);
	void DrawTextOrg(void);
	void DrawSpeech(void);

	void SetText(const std::string& text);
	void SetTime(double time);

	void SetType(TYPE type);
	void SetVisiblePermanent(void);

	void SetLocalPos(const VECTOR& localPos);

private:

	TYPE type_;

	int image_;

	// true�̏ꍇ�A�i���I�ɕ\������
	bool isVisiblePermanent_;
	double visibleTime_;

	// �����o����t����e�I�u�W�F�N�g
	const Transform& transformParent_;

	// �\�����W
	VECTOR pos_;

	// �\���ʒu�̒����p
	VECTOR localPos_;

	// �\������
	std::string text_;

};
