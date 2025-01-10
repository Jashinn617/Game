#pragma once
#include<array>
#include<map>
#include<unordered_map>
#include<string>

enum class InputType
{
	kwybd,	// キーボード
	pad		// パッド
};

using InputTable_t = std::unordered_map<std::string, std::map<InputType, int>>;

class Input
{
private:
	// コマンド名と入力をペアにしたテーブル
	InputTable_t commandTable;
	std::vector<std::string> exclusiveKeyConfigCommands;

	// コマンドの入力を覚えておく
	std::map < std::string, bool> inputData;	// 現在の入力
	std::map<std::string, bool>lastInputData;	// 直前の入力

	const InputTable_t GetCommandTable() const;

public:

	Input();

	/// <summary>
	/// 入力情報の更新
	/// </summary>
	void Update();

	/// <summary>
	/// 指定のコマンドが押された瞬間なのかどうか
	/// </summary>
	/// <param name="command"></param>
	/// <returns></returns>
	bool IsTriggered(const char* command)const;

	/// <summary>
	/// 今押しているかどうか
	/// </summary>
	/// <param name="command"></param>
	/// <returns></returns>
	bool IsPressing(const char* command)const;

	/// <summary>
	/// 指定のコマンドが離された瞬間かどうか
	/// </summary>
	/// <param name="command"></param>
	/// <returns></returns>
	bool IsReleased(const char* command)const;



};

