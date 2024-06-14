#include <DxLib.h>
#include "PixelMaterial.h"
#include "../Application.h"

//GameSceneÇÃInitÇ≈Ç‚Ç¡ÇƒÇΩÇ‚Ç¬
PixelMaterial::PixelMaterial(std::string shaderFileName,int constBufFloat4Size)
{
	shaderFName = shaderFileName;
	constBufFloat4Size_ = constBufFloat4Size;
}

PixelMaterial::~PixelMaterial()
{
	for (const auto& tex : textures_)
	{
		DeleteGraph(tex);
	}

	DeleteGraph(postEScreen_);
	DeleteGraph(shader_);
	DeleteShaderConstantBuffer(constBufFloat4Size_);
}

void PixelMaterial::Init()
{
	postEScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	constBuf_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4Size_);
	shader_ = LoadPixelShader(
		(Application::PATH_SHADER + shaderFName).c_str());
}

//âÊëúégópêÍóp
void PixelMaterial::Init(const std::vector<int>& texs)
{
	textures_ = texs;
	postEScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	constBuf_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4Size_);
	shader_ = LoadPixelShader(
		(Application::PATH_SHADER + shaderFName).c_str());
}
