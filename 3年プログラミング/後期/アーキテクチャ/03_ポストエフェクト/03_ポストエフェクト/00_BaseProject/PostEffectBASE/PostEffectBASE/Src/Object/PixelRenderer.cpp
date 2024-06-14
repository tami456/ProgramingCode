#include "PixelMaterial.h"
#include "PixelRenderer.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"

PixelRenderer::PixelRenderer(PixelMaterial& pixelMaterial):pixelMaterial_(pixelMaterial)
{
	MakeSquereVertex();
	mainScreen_ = SceneManager::GetInstance().GetMainScreen();
}

PixelRenderer::~PixelRenderer()
{
}

void PixelRenderer::MakeSquereVertex(Vector2 pos, Vector2 size)
{

}

//GameScene��Init�ł���Ă��|���S���̐���
void PixelRenderer::MakeSquereVertex(void)
{

	//��ʂ����ς���2���̃|���S��
	float sX = static_cast<float>(0.0f);
	float sY = static_cast<float>(0.0f);
	float eX = static_cast<float>(Application::SCREEN_SIZE_X);
	float eY = static_cast<float>(Application::SCREEN_SIZE_Y);

	// �S���_�̏�����
	for (int i = 0; i < 4; i++)
	{
		vertexs_[i].rhw = 1.0f;
		vertexs_[i].dif = GetColorU8(255, 255, 255, 255);
		vertexs_[i].spc = GetColorU8(255, 255, 255, 255);
		vertexs_[i].su = 0.0f;
		vertexs_[i].sv = 0.0f;
	}

	// ����
	vertexs_[0].pos = VGet(sX, sY, 0.0f);
	vertexs_[0].u = 0.0f;
	vertexs_[0].v = 0.0f;

	// �E��
	vertexs_[1].pos = VGet(eX, sY, 0.0f);
	vertexs_[1].u = 1.0f;
	vertexs_[1].v = 0.0f;

	// �E��
	vertexs_[2].pos = VGet(eX, eY, 0.0f);
	vertexs_[2].u = 1.0f;
	vertexs_[2].v = 1.0f;

	// ����
	vertexs_[3].pos = VGet(sX, eY, 0.0f);
	vertexs_[3].u = 0.0f;
	vertexs_[3].v = 1.0f;

	indexes_[0] = 0;
	indexes_[1] = 1;
	indexes_[2] = 3;

	indexes_[3] = 1;
	indexes_[4] = 2;
	indexes_[5] = 3;
}

//GameScene��Draw�ł���Ă����
void PixelRenderer::Draw()
{
	auto postEffectScreen = pixelMaterial_.GetPostEScreen();
	auto shader = pixelMaterial_.GetMShader();
	auto shaderConstBuf = pixelMaterial_.GetMConstBuf();
	
	// �|�X�g�G�t�F�N�g
	//-----------------------------------------
	SetDrawScreen(postEffectScreen);

	// ��ʂ�������
	ClearDrawScreen();

	// �V�F�[�_�[�ݒ�
	SetUsePixelShader(shader);

	SetUseTextureToShader(0, mainScreen_);

	int index = 1;
	for (const auto& mScreen : pixelMaterial_.GetTextures())
	{
		// �e�N�X�`���̐ݒ�
		SetUseTextureToShader(index, mScreen);
		index++;
	}

	// �萔�o�b�t�@
	FLOAT4* constBufsPtr =
		(FLOAT4*)GetBufferShaderConstantBuffer(shaderConstBuf);

	for (const auto& constBufs : pixelMaterial_.GetMConstBufs())
	{
		// ��Z�F
		constBufsPtr->x = constBufs.x;
		constBufsPtr->y = constBufs.y;
		constBufsPtr->z = constBufs.z;
		constBufsPtr->w = constBufs.w;
		constBufsPtr++;
	}
	

	// �萔�o�b�t�@���X�V���ď������񂾓��e�𔽉f����
	UpdateShaderConstantBuffer(shaderConstBuf);

	// �萔�o�b�t�@���s�N�Z���V�F�[�_�[�p�萔�o�b�t�@���W�X�^�ɃZ�b�g
	SetShaderConstantBuffer(
		shaderConstBuf, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	// �`��
	DrawPolygonIndexed2DToShader(vertexs_, NUM_VERTEX, indexes_, NUM_POLYGON);

	// ���C���ɖ߂�
	SetDrawScreen(mainScreen_);
	DrawGraph(0, 0, postEffectScreen, false);
	//-----------------------------------------
}

void PixelRenderer::Draw(int x, int y)
{
}
