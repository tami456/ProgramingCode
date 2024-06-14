#pragma once
#include <map>
#include<memory>
#include "Common/Transform.h"
#include "ActorBase.h"
class Player;

class WarpStar : public ActorBase
{

public:

	// ��������
	static constexpr float TIME_WARP_RESERVE = 2.0f;

	// ���a
	static constexpr float RADIUS = 80.0f;

	// �G�t�F�N�g�̈ʒu
	static constexpr VECTOR EFFECT_RELATIVE_POS = { 0.0f, 70.0f, 0.0f };
	
	// ���[�v�O�̏����ړ����΍��W
	static constexpr VECTOR WARP_RELATIVE_POS = { 0.0f, -80.0f, 10.0f };

	// ��]���x
	static constexpr float SPEED_ROT_IDLE = 3.0f;
	static constexpr float SPEED_ROT_RESERVE = 15.0f;

	// �G�t�F�N�g�����Ԋu
	static constexpr float TERM_EFFECT = 0.08f;

	// ���
	enum class STATE
	{
		NONE,
		IDLE,
		RESERVE,
		MOVE
	};

	// �R���X�g���N�^
	WarpStar(
		Player& player, const Transform& transform);

	// �f�X�g���N�^
	~WarpStar(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	// �v���C���[
	Player& player_;

	// ��ԊǗ�
	STATE state_;

	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStateIdle(void);
	void ChangeStateReserve(void);
	void ChangeStateMove(void);

	// �X�V�X�e�b�v
	void UpdateNone(void);
	void UpdateIdle(void);
	void UpdateReserve(void);
	void UpdateMove(void);

};
