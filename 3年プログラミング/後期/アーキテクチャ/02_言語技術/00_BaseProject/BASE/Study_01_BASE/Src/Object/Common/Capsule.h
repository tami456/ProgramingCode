#pragma once
#include <DxLib.h>
class Transform;

class Capsule
{

public :

	// �f�o�b�O���̊ȈՃJ�v�Z���\���̐F
	static constexpr int COLOR = 0xffffff;

	// �R���X�g���N�^
	// �J�v�Z��������I�u�W�F�N�g���w�肷��
	Capsule(const Transform& parent);
	Capsule(const Capsule& base, const Transform& parent);

	// �f�X�g���N�^
	~Capsule(void);

	// �`��
	void Draw(void);

	// �eTransform����̑��Έʒu���擾
	VECTOR GetLocalPosTop(void) const;
	VECTOR GetLocalPosDown(void) const;

	// �eTransform����̑��Έʒu���Z�b�g
	void SetLocalPosTop(const VECTOR& pos);
	void SetLocalPosDown(const VECTOR& pos);

	// ���[���h���W���擾
	VECTOR GetPosTop(void) const;
	VECTOR GetPosDown(void) const;

	// ���΍��W����]�����ă��[���h���W�Ŏ擾����
	VECTOR GetRotPos(const VECTOR& localPos) const;

	// ���a
	float GetRadius(void) const;
	void SetRadius(float radius);

	// ����
	float GetHeight(void) const;

	// �J�v�Z���̒��S���W
	VECTOR GetCenter(void) const;

private :

	// �J�v�Z�����������鑊��
	const Transform& transformParent_;

	// �eTransform����̑��Έʒu(�㑤)
	VECTOR localPosTop_;

	// �eTransform����̑��Έʒu(����)
	VECTOR localPosDown_;

	// ���a
	float radius_;

};
