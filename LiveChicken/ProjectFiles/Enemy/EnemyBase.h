#pragma once
#include "DxLib.h"

// 敵クラスのベース
class EnemyBase abstract
{
public:
	EnemyBase(int modelHandle);
	virtual ~EnemyBase();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() abstract;
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();
	/// <summary>
	/// モデルハンドルの取得
	/// </summary>
	/// <returns>モデルハンドル</returns>
	int GetModelHandle()const { return m_modelHandle; }

	/// <summary>
	/// 位置の取得
	/// </summary>
	/// <returns>現在位置</returns>
	const VECTOR& GetPos() const;
	/// <summary>
	/// 位置のセット
	/// </summary>
	/// <param name="pos">セットしたい位置</param>
	void SetPos(const VECTOR pos) { m_pos = pos; }
	/// <summary>
	/// モデルの位置の調整
	/// </summary>
	void AdjustmentModelPos();
	/// <summary>
	/// 半径の取得
	/// </summary>
	/// <returns>半径</returns>
	const float GetRad()const { return m_rad; }	

protected:	// 変数
	int m_modelHandle;		// 3Dモデルのハンドル
	float m_rad;			// 半径
	VECTOR m_pos;			// 位置

private: // 定数
	static constexpr float kScale = 0.005f;						// スケール
	static constexpr float kRadius = 0.3f;						// 半径
	static constexpr float kDirY = 180.0f * DX_PI_F / 270.0f;	// Y軸方向の向き
};

