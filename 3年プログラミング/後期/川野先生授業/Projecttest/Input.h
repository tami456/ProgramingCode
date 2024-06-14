#pragma once
#include<string>
#include<unordered_map>
#include<vector>

enum class InputType
{
	keyboard,
	gamepad,
	mouse
};

struct InputInfo {
	InputType type;
	int inputID;
};

//struct InputState {
//	char keycode[256];
//	bool isPressed = false;
//};

//ゲーム中の意味と入力されたかどうかをペアにしたもの
//とにかく対象の「意味」が押されたら値がtrueになる型
using InputState_t = std::unordered_map<std::string, bool>;

//ゲーム中の"意味"と実際のキーコードのペア
//キー(first)は命令の意味、
//ゲーム中の"意味"と実際の入力(ハード入力)の対応表を表す「型」
using InputTable_t = std::unordered_map<std::string, std::vector<InputInfo>>;

class KeyconfigScene;

class Input
{
	friend KeyconfigScene;
private:

	std::vector<std::string> excludeOnConfigList_;//キーコンフィグ除外リスト

	//意味とハード入力の対応表
	InputTable_t inputTable_;
	InputTable_t tempTable_;
	

	//押した記録
	InputState_t currentState_ = {};
	InputState_t lastState_ = {};


	int currentpadState_;
	int lastpadState_;

	//値の代入とコピーの禁止
	//シングルトンではないが複数出てくるとややこしくなるため
	Input(const Input&) = delete;
	void operator=(const Input&) = delete;

	//現在そのキーが押されているか
	bool GetCurrentInput(const std::string& input)const;
	//直前にそのキーが押されているか
	bool GetLastInput(const std::string& input)const;
	
public:
	
	Input();
	/// <summary>
	/// 入力情報を更新する(毎フレーム呼ばれる)
	/// </summary>
	void Update();

	/// <summary>
	/// 今押した瞬間かどうか返す(押しっぱなしはfalse)
	/// </summary>
	/// <param name="input">入力識別文字列<"next"や"pause"など/param>
	/// <returns>押した瞬間ならtrue</returns>
	bool IsTriggered(const std::string& input)const;

	/// <summary>
	/// 今押しているかどうかを返す
	/// </summary>
	/// <param name="input">入力識別文字列<"next"や"pause"など/param>
	/// <returns>押した瞬間ならtrue</returns>
	bool IsPressed(const std::string& input)const;

	/// <summary>
	/// 入力テーブルの書き換え
	/// </summary>
	/// <param name="key">命令文字列</param>
	/// <param name="inputInfo">機器からの入力情報(ハード)</param>
	void RewriteInput(const std::string& key,
						const InputInfo& inputInfo);

	void CommitInputConfig();

	/// <summary>
	/// 入力テーブルを考える
	/// </summary>
	/// <returns></returns>
	const InputTable_t GetInputTabel()const;

	void SaveInputTable();

	void LoadInputTable();

	/// <summary>
	/// 現在の入力が除外入力に当たるかどうかをチェックする
	/// </summary>
	/// <returns>除外入力にあたる、false:除外する必要はない</returns>
	bool CheckExclusiveInput();
};

