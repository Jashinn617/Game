#pragma once
#include "DxLib.h"
#include <memory>

using namespace std;

class Input;

/// <summary>
/// プレイヤークラス
/// </summary>
class Player
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();
	/// <summary>
	/// 更新
	/// </summary>
	void Update(Input& input);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw()const;
	
	/// <summary>
	/// 位置を取得する
	/// </summary>
	/// <returns>現在位置</returns>
	const VECTOR& GetPos()const;
	/// <summary>
	/// 半径を取得する
	/// </summary>
	/// <returns>半径</returns>
	const float GetRad()const { return m_rad; }

private:	// 関数
	/// <summary>
	/// ジャンプ処理
	/// </summary>
	/// <param name="input">入力</param>
	void Jump(Input& input);

	/// <summary>
	/// 重力の処理と地面の処理
	/// </summary>
	void GravityAndGround();

private:	// 変数
	VECTOR m_pos;		// 位置
	VECTOR m_move;		// 移動量

	int m_modelHandle;	// 3Dモデル

	float m_rad;		// 半径

	float m_jumpPower;	// ジャンプ力
	bool m_isJump;		// ジャンプ中かどうか
	int m_jumpSe;		// ジャンプ中のSE

private:	// 定数
	static constexpr float kSpeed = static_cast<float>(10000.0 / 60.0 / 60.0 / 60.0);	// 速度
	static constexpr float kScale = 0.002f;												// スケール
	static constexpr float kRadius = 0.3f;												// 当たり判定用の半径
	static constexpr float kGravity = 0.025f;											// 重力
	static constexpr float kJumpHeight = 0.5f;											// ジャンプ力
	static constexpr float kDirY = 180.0f * DX_PI_F / 180.0f;							// Y軸方向の向き
	static constexpr float kGroundPosY = 0.4f;											// 地面のY位置
};

