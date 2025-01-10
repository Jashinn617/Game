#pragma once
#include <vector>

class WorldSprite;

/// <summary>
/// 2Dの背景マップ
/// </summary>
class Map
{
public:
	Map();
	~Map();

	/// <summary>
	/// ロード
	/// </summary>
	void Load();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/*マップチップのサイズ*/
	static const float	kChipSize;
	static const int	kChipPixelSize;

	static const int	kStageDataColNum = 16;		// マップ行
	static const int	kStageDataRowNum = 480;		// マップ列

private:
	static const int m_stage1Data[kStageDataColNum][kStageDataRowNum];	// マップチップを並べる配列

	std::vector<WorldSprite*> m_pSprites;		// ワールドスプライトのポインタ
	int m_chipHandle;							// マップチップの画像
};

