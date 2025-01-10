#pragma once

#include <memory>

class Player;
class Camera
{
public:
	// 今どのマップにいるか
	enum ScrollPattern
	{
		// 横スクロール
		kHorizontal,
		kHorizontal2,
		// 縦スクロール
		kVertical,
		// 縦横スクロール
		kMixture
	};

public:
	Camera();
	~Camera();

	// 初期化
	void Init();
	// 横スクロール
	int GetScrollX();
	// 縦スクロール
	int GetScrollY();
	/*マップの大きさの決定*/
	void StartHorizon() { m_scrollPattern = kHorizontal; }
	void StartHorizon2() { m_scrollPattern = kHorizontal2; }
	void StartVertical() { m_scrollPattern = kVertical; }
	void StartMixture() { m_scrollPattern = kMixture; }
	/*マップサイズの取得*/
	int GetMapWidth()const { return m_mapWidth; }
	int GetMapHeight()const { return m_mapHeight; }

	// プレイヤーのポインタを設定する
	void SetPlayer(std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer; }


private:
	// プレイヤーのポインタ
	std::shared_ptr<Player> m_pPlayer;
	// 現在のマップ情報
	ScrollPattern m_scrollPattern;
	/*現在のマップのサイズ*/
	// 横
	int m_mapWidth;
	// 縦
	int m_mapHeight;

};

