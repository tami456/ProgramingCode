#pragma once
class Stage
{
public:
	// コンストラクタ
	Stage(void);

	// デストラクタ
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
	int GetModelId(void) const;
private:

	// ３ＤモデルのハンドルID
	int modelId_;
	int imgBack_;
};

