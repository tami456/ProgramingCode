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
	
	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	// ピクセルシェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	// 頂点数
	static constexpr int NUM_VERTEX = 4;

	// 頂点インデックス数
	static constexpr int NUM_VERTEX_IDX = 6;

	// ポリゴン数
	static constexpr int NUM_POLYGON = 2;


	// 定数バッファハンドル
	int monoShaderConstBuf_;

	int slShaderConstBuf_;

	int distorShaderConstBuf_;

	int vignettShaderConstBuf_;

	int afterImageShaderConstBuf_;

	int blurShaderConstBuf_;

	// 頂点
	VERTEX2DSHADER vertexs_[NUM_VERTEX];

	// 頂点インデックス
	WORD indexes_[NUM_VERTEX_IDX];

	std::unique_ptr<PixelMaterial> matMono_;

	std::unique_ptr<PixelMaterial> matScanL_;

	std::unique_ptr<PixelMaterial> matDistortion_;

	std::unique_ptr<PixelMaterial> matVignett_;

	std::unique_ptr<PixelMaterial> matAfterImage_;

	std::unique_ptr<PixelMaterial> matBlur_;


	std::vector<std::unique_ptr<PixelRenderer>> renderer_;

	// ステージ
	std::unique_ptr<Stage> stage_;

	// スカイドーム
	std::unique_ptr<SkyDome> skyDome_;

	// プレイヤー
	std::unique_ptr<Player> player_;

};
