#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"
class Transform;

class Camera
{

public:

	// �J�����X�s�[�h(�x)
	static constexpr float SPEED = 1.0f;

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_NEAR = 10.0f;

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_FAR = 30000.0f;

	// �J�����̏������W
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// �Ǐ]�ʒu����J�����ʒu�܂ł̑��΍��W
	static constexpr VECTOR LOCAL_F2C_POS = { 0.0f, 50.0f, -400.0f };

	// �Ǐ]�ʒu���璍���_�܂ł̑��΍��W
	static constexpr VECTOR LOCAL_F2T_POS = { 0.0f, 0.0f, 500.0f };

	// �J������X��]����x�p
	static constexpr float LIMIT_X_UP_RAD = 40.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = 15.0f * (DX_PI_F / 180.0f);
	
	// �J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT,
		FOLLOW,
		SELF_SHOT
	};

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void Draw(void);

	// �J�����ʒu
	VECTOR GetPos(void) const;
	// �J�����̑���p�x
	VECTOR GetAngles(void) const;
	// �J�����̒����_
	VECTOR GetTargetPos(void) const;

	// �J�����p�x
	Quaternion GetQuaRot(void) const;
	// X��]�𔲂����J�����p�x
	Quaternion GetQuaRotOutX(void) const;
	// �J�����̑O������
	VECTOR GetForward(void) const;

	// �J�������[�h�̕ύX
	void ChangeMode(MODE mode);

	// �Ǐ]�Ώۂ̐ݒ�
	void SetFollow(const Transform* follow);

private:

	// �J�������Ǐ]�ΏۂƂ���Transform
	const Transform* followTransform_;

	// �J�������[�h
	MODE mode_;

	// �J�����̈ʒu
	VECTOR pos_;

	// �J�����p�x(rad)
	VECTOR angles_;

	// X����]�������p�x
	Quaternion rotOutX_;

	// �J�����p�x
	Quaternion rot_;

	// �����_
	VECTOR targetPos_;

	// �J�����̏����
	VECTOR cameraUp_;

	// �J�����������ʒu�ɖ߂�
	void SetDefault(void);

	// �Ǐ]�ΏۂƂ̈ʒu���������
	void SyncFollow(void);

	// �J��������
	void ProcessRot(void);

	// ���[�h�ʍX�V�X�e�b�v
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawSelfShot(void);

};

