#pragma once
#include<DxLib.h>
#include <string>
#include <vector>
#include <memory>

//DxLib�̊֐��ł����ݒ肷��֐�������
enum class TEXADDRESS
{
	NONE = 0,
	WRAP,
	MIRROR,
	CLAMP,
	BORDER,
	MAX
};

class PixelMaterial
{
public:
	
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="shaderFileName"></param>
	/// <param name="constBufFloat4Size"></param>
	PixelMaterial(std::string shaderFileName, int constBufFloat4Size);

	~PixelMaterial();
	void Init();
	void Init(const std::vector<int>& texs);

	void SetConstBuf(const std::vector<FLOAT4>& constBufs)
	{
		constBufs_ = constBufs;
	}

	const int& GetMShader(void)const { return shader_; }
	const int& GetMConstBufSize(void)const { return constBufFloat4Size_; }
	const int& GetMConstBuf(void)const { return constBuf_; }
	const std::vector<FLOAT4>& GetMConstBufs(void)const { return constBufs_; }
	const int& GetPostEScreen(void)const { return postEScreen_; }
	const std::vector<int>& GetTextures(void)const { return textures_; }

private:

	// �V�F�[�_�n���h��
	int shader_;

	// �萔�o�b�t�@�̊m�ۃT�C�Y(FLOAT4��������邩)
	int constBufFloat4Size_;

	// �萔�o�b�t�@�n���h��
	int constBuf_;

	int postEScreen_;

	// �e�N�X�`���A�h���X
	TEXADDRESS texAddress_;

	// �萔�o�b�t�@
	std::vector<FLOAT4> constBufs_;

	// �摜
	std::vector<int> textures_;

	std::string shaderFName;
};

