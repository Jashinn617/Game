#pragma once
#include "DxLib.h"

class MoveDirectionVec
{
public:
	MoveDirectionVec();
	~MoveDirectionVec();

	void Update();

	/// <summary>
	/// 移動方向を設定する
	/// </summary>
	void SetDirectionVec();

	/// <summary>
	/// 移動方向の取得
	/// </summary>
	/// <returns>移動方向</returns>
	VECTOR GetDirectionVec()const { return m_directionVec; }

private:
	VECTOR m_directionVec;	// 方向ベクトル
};

