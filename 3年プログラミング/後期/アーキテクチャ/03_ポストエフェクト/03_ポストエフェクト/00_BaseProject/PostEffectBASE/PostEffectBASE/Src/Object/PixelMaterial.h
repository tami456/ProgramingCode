#pragma once
#include<DxLib.h>
#include <string>
#include <vector>
#include <memory>

//DxLibの関数でこれを設定する関数がある
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
	/// コンストラクタ
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

	// シェーダハンドル
	int shader_;

	// 定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	int constBufFloat4Size_;

	// 定数バッファハンドル
	int constBuf_;

	int postEScreen_;

	// テクスチャアドレス
	TEXADDRESS texAddress_;

	// 定数バッファ
	std::vector<FLOAT4> constBufs_;

	// 画像
	std::vector<int> textures_;

	std::string shaderFName;
};

