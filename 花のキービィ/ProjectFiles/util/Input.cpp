#include "DxLib.h"
#include "Input.h"
#include <algorithm>

const InputTable_t Input::GetCommandTable() const
{
	InputTable_t ret = commandTable;

	for (const auto ex : exclusiveKeyConfigCommands)
	{
		ret.erase(ex);
	}
	return ret;
}

Input::Input()
{
	// 押されたコマンドの対応表

	// 上ボタン
	commandTable["up"] = { {InputType::kwybd,KEY_INPUT_UP},
								{InputType::pad,PAD_INPUT_UP} };

	// 下ボタン
	commandTable["down"] = { {InputType::kwybd,KEY_INPUT_DOWN},
								{InputType::pad,PAD_INPUT_DOWN} };

	// 左ボタン
	commandTable["left"] = { {InputType::kwybd,KEY_INPUT_LEFT},
								{InputType::pad,PAD_INPUT_LEFT} };
	// 右ボタン
	commandTable["right"] = { {InputType::kwybd,KEY_INPUT_RIGHT},
								{InputType::pad,PAD_INPUT_RIGHT} };

	// Aボタン
	commandTable["A"] = { {InputType::kwybd,KEY_INPUT_Z},
								{InputType::pad,PAD_INPUT_A} };

	// Bボタン
	commandTable["B"] = { {InputType::kwybd,KEY_INPUT_X},
								{InputType::pad,PAD_INPUT_B} };

	// スタートボタン・Yボタン
	commandTable["S"] = { {InputType::kwybd,KEY_INPUT_S},
								{InputType::pad,PAD_INPUT_Y} };

	// スタートボタン・Xボタン
	commandTable["P"] = { {InputType::kwybd,KEY_INPUT_P},
								{InputType::pad,PAD_INPUT_X} };
}

void Input::Update()
{
	// 直前入力をコピーしておく
	lastInputData = inputData;

	// ハードウェア入力チェック
	char keystate[256];	// キーボード用
	GetHitKeyStateAll(keystate);	// キーボード入力の取得
	int padstate = GetJoypadInputState(DX_INPUT_PAD1);	// パッド情報の取得

	// 登録された情報とハードの情報を照らし合わせる
	// ↑をもとにinputDataの内容を更新する
	for (const auto& cmd : commandTable)
	{
		// コマンドの名前から入力データを作る
		auto& input = inputData[cmd.first];

		for (const auto& hardIO : cmd.second)
		{
			// キーボードのチェック
			input = false;
			if (hardIO.first == InputType::kwybd)
			{
				if (keystate[hardIO.second])
				{
					input = true;
					break;	// 同時押しをできないようにここでbreakする
				}
			}
			else if (hardIO.first == InputType::pad)
			{
				if (padstate & hardIO.second)
				{
					input = true;
					break;
				}
			}
		}
	}

}

bool Input::IsTriggered(const char* command) const
{
	auto it = inputData.find(command);
	if (it == inputData.end())	// 要素無し
	{
		return false;	// 反応しない
	}

	return(inputData.at(command) && !lastInputData.at(command));
}

bool Input::IsPressing(const char* command) const
{
	auto it = inputData.find(command);
	if (it == inputData.end())	// 要素無し
	{
		return false;	// 反応しない
	}

	return(inputData.at(command));
}

bool Input::IsReleased(const char* command) const
{
	auto it = inputData.find(command);
	if (it == inputData.end())	// 要素無し
	{
		return false;	// 反応しない
	}

	return(!inputData.at(command) && lastInputData.at(command));
}


