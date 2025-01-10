#pragma once

/// <summary>
/// 2D画像を3D空間に表示するクラス
/// </summary>
class WorldSprite
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureGraph">画像のハンドル</param>
	/// <param name="chipPixelSizeW">画像の横サイズ</param>
	/// <param name="chipPixelSizeH">画像の縦サイズ</param>
	/// <param name="spriteNo">マップチップ等、切り取る場合の番号(切り取らない場合はデフォルトの0のままで良い)</param>
	void Initialize(int textureGraph, int chipPixelSizeW, int chipPixelSizeH, int spriteNo = 0);
	
	/// <summary>
	/// サイズと位置に応じて4頂点分の頂点の位置を調整する
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="spriteSize">サイズ</param>
	void SetTransform(const VECTOR& pos, float spriteSize);
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 位置の取得
	/// </summary>
	/// <returns></returns>
	const VECTOR& GetPos()const { return m_pos; }

private:
	int			m_textureHandle;	// スプライトを作成する元テクスチャのグラフィックハンドル
	VECTOR		m_pos;			// 描画位置
	VERTEX3D	m_vertex[4];		// 頂点バッファ
	WORD		m_index[6];		// インデックスバッファ
};

