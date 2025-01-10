#pragma once
#include "DxLib.h"

// ゲーム全体で使用する定数を定義する
namespace Game
{
	constexpr int kScreenWidth = 1600;	// 画面の横サイズ
	constexpr int kScreenHeight = 900;	// 画面の縦サイズ

	constexpr int kColorDepth = 32;

	constexpr float kRadianConversion = DX_PI_F / 180.0f;	// 角度をラジアンに変換する式
}