#pragma once
#include "../Common/Quaternion.h"
#include "ActorBase.h"
#include "Stage.h"

class Planet : public ActorBase
{

public:

	// �d�͂̋���
	static constexpr float DEFAULT_GRAVITY_POW = 25.0f;

	// �d�͂�������͈�
	static constexpr float DEFAULT_GRAVITY_RADIUS = 5000.0f;

	// �Q�[���I�[�o�[�͈�
	static constexpr float DEFAULT_DEAD_LENGTH = 1000.0f;

	// �d�͎��
	enum class TYPE
	{
		GROUND,
		SPHERE,
		TRANS_ROT,
		TRANS_CAMERA_FIXED,
		ROAD,
	};

	// �R���X�g���N�^
	Planet(const Stage::NAME& name, const TYPE& type, const Transform& transform);

	// �f�X�g���N�^
	~Planet(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	
	// �f�����
	const TYPE& GetType(void) const;

	// �f����
	const Stage::NAME& GetName(void) const;

	// �ʒu�̐ݒ�
	void SetPosition(const VECTOR& pos);

	// ��]�̐ݒ�
	void SetRotation(const Quaternion& rot);

	// �d�͂̋����̎擾�E�ݒ�
	float GetGravityPow(void) const;
	void SetGravityPow(float pow);
	
	// �d�͔͈͂̎擾�E�ݒ�
	float GetGravityRadius(void) const;
	void SetGravityRadius(float radius);

	// �d�͔͈͓�
	bool InRangeGravity(const VECTOR& pos) const;

	// �Q�[���I�[�o�[�͈͓�
	bool InRangeDead(const VECTOR& pos) const;

	// �Q�[���I�[�o�[�͈�
	void SetDeadLength(float len);

private:

	// �f�����
	TYPE type_;

	// �f����
	Stage::NAME name_;

	// �d�͂̋���
	float gravityPow_;

	// �d�͔͈�
	float gravityRadius_;

	// �Q�[���[�I�[�o�[����
	float deadLength_;

};
