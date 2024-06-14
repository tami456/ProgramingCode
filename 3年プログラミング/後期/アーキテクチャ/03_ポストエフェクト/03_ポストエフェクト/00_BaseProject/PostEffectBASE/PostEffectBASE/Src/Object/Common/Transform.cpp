#include <DxLib.h>
#include "../../Utility/AsoUtility.h"
#include "Transform.h"

Transform::Transform(void)
{
	modelId = -1;

	scl = AsoUtility::VECTOR_ONE;
	rot = AsoUtility::VECTOR_ZERO;
	pos = AsoUtility::VECTOR_ZERO;
	localPos = AsoUtility::VECTOR_ZERO;

	matScl = MGetIdent();
	matRot = MGetIdent();
	matPos = MGetIdent();
	quaRot = Quaternion();
	quaRotLocal = Quaternion();

	collider = nullptr;
}

Transform::Transform(int model)
{
	modelId = model;

	scl = AsoUtility::VECTOR_ONE;
	rot = AsoUtility::VECTOR_ZERO;
	pos = AsoUtility::VECTOR_ZERO;
	localPos = AsoUtility::VECTOR_ZERO;

	matScl = MGetIdent();
	matRot = MGetIdent();
	matPos = MGetIdent();
	quaRot = Quaternion();
	quaRotLocal = Quaternion();

	collider = nullptr;
}

Transform::~Transform(void)
{
}

void Transform::Update(void)
{

	// �傫��
	matScl = MGetScale(scl);

	// ��]
	rot = quaRot.ToEuler();
	matRot = quaRot.ToMatrix();

	// �ʒu
	matPos = MGetTranslate(pos);

	// �s��̍���
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl);
	Quaternion q = quaRot.Mult(quaRotLocal);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos);

	// �s������f���ɔ���
	if (modelId != -1)
	{
		MV1SetMatrix(modelId, mat);
	}

	// �Փ˔���̍X�V
	if (collider != nullptr)
	{
		MV1RefreshCollInfo(modelId);
	}

}

void Transform::SetModel(int model)
{
	modelId = model;
}

void Transform::MakeCollider(Collider::TYPE type)
{

	if (modelId == -1)
	{
		return;
	}

	collider = std::make_shared<Collider>(type, modelId);
	int ret = MV1SetupCollInfo(modelId, -1, 1, 1, 1);

}

VECTOR Transform::GetForward(void) const
{
	return GetDir(AsoUtility::DIR_F);
}

VECTOR Transform::GetBack(void) const
{
	return GetDir(AsoUtility::DIR_B);
}

VECTOR Transform::GetRight(void) const
{
	return GetDir(AsoUtility::DIR_R);
}

VECTOR Transform::GetLeft(void) const
{
	return GetDir(AsoUtility::DIR_L);
}

VECTOR Transform::GetUp(void) const
{
	return GetDir(AsoUtility::DIR_U);
}

VECTOR Transform::GetDown(void) const
{
	return GetDir(AsoUtility::DIR_D);
}

VECTOR Transform::GetDir(const VECTOR& vec) const
{
	return quaRot.PosAxis(vec);
}
