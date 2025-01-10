#pragma once
#include "DxLib.h"

class Player;

/// <summary>
/// カメラ
/// </summary>
class Camera
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera();	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Camera();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="playr">プレイヤーのクラス</param>
	void Update(const Player& player);

	/// <summary>
	/// ポジションのgetter/setter
	/// </summary>
	/// <returns>ポジション</returns>
	const VECTOR& GetPos()const { return m_pos; }

private:
	VECTOR m_pos;	// ポジション
};

