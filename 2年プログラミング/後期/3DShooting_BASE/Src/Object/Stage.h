#pragma once
class Stage
{
public:

	// グリッド線の大きさ
	static constexpr float LEN_MASS = 100.0f;

	// グリッド線の描画数
	static constexpr float LEN_NUM = 50.0f;

	// コンストラクタ
	Stage(void);

	// デストラクタ
	~Stage(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void DrawGrid(void);
	void Release(void);

	//Gameシーンにステージクラス宣言
	//DrawGridでXYZ線描画

	int GetModelDungeon(void);
	int GetModelCargoShip(void);
	VECTOR GetPosCargoShip(void);
private:

	//ダンジョン
	int mModelDungeon;

	//戦艦
	int mModelCargoShip;

	VECTOR mPosDungeon;
	VECTOR mPosCargoShip;
};

