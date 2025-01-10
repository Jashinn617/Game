#pragma once
#include <memory>

using namespace std;

class Player;
class Input;

/// <summary>
/// カメラクラス
/// </summary>
class Camera
{
public:
	Camera(shared_ptr<Player> player);
	~Camera();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力</param>
	/// <param name="isOperate">操作ができるかどうか</param>
	void Update(Input& input, bool isOperate);

	/// <summary>
	/// 位置の取得
	/// </summary>
	/// <returns>現在位置</returns>
	const VECTOR GetPos()const { return m_pos; }

private:	// 変数
	VECTOR m_pos;	// 位置
	float m_angle;	// 角度

	shared_ptr<Player> m_pPlayer;	// プレイヤーのポインタ

private:	// 定数
	static constexpr float kCameraDist = 3.0f;		// カメラの距離
	static constexpr float kCameraHeight = 2.2f;	// カメラの高さ
	static constexpr float kCameraNear = 0.5f;		// カメラの最短描画距離
	static constexpr float kCameraFar = 1000.0f;	// カメラの最長描画距離
	static constexpr float kSpeed = 0.025f;			// 回転時移動速度
};

