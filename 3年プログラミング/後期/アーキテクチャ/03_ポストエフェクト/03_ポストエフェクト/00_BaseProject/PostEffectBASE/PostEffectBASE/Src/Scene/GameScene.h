#pragma once
#include <memory>
#include "SceneBase.h"

class Stage;
class SkyDome;
class Player;
class PixelMaterial;
class PixelRenderer;

class GameScene : public SceneBase
{
	
public:
	
	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	// �s�N�Z���V�F�[�_�p�I���W�i���萔�o�b�t�@�̎g�p�J�n�X���b�g
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	// ���_��
	static constexpr int NUM_VERTEX = 4;

	// ���_�C���f�b�N�X��
	static constexpr int NUM_VERTEX_IDX = 6;

	// �|���S����
	static constexpr int NUM_POLYGON = 2;


	// �萔�o�b�t�@�n���h��
	int monoShaderConstBuf_;

	int slShaderConstBuf_;

	int distorShaderConstBuf_;

	int vignettShaderConstBuf_;

	int afterImageShaderConstBuf_;

	int blurShaderConstBuf_;

	// ���_
	VERTEX2DSHADER vertexs_[NUM_VERTEX];

	// ���_�C���f�b�N�X
	WORD indexes_[NUM_VERTEX_IDX];

	std::unique_ptr<PixelMaterial> matMono_;

	std::unique_ptr<PixelMaterial> matScanL_;

	std::unique_ptr<PixelMaterial> matDistortion_;

	std::unique_ptr<PixelMaterial> matVignett_;

	std::unique_ptr<PixelMaterial> matAfterImage_;

	std::unique_ptr<PixelMaterial> matBlur_;


	std::vector<std::unique_ptr<PixelRenderer>> renderer_;

	// �X�e�[�W
	std::unique_ptr<Stage> stage_;

	// �X�J�C�h�[��
	std::unique_ptr<SkyDome> skyDome_;

	// �v���C���[
	std::unique_ptr<Player> player_;

};
