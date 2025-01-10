#pragma once
#include <vector>
#include <memory>

class Player;
class UI
{
public:
	UI();
	~UI();

	// 初期化
	void Init();
	// 描画
	void Draw();
	// 更新
	void Update();
	// プレイヤーのポインタの設定
	void SetPlayer(std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer; }

private:
	// プレイヤーのHP
	int m_playerHp;
	// プレイヤーの残機
	int m_playerRemainingLives;
	/*UIの画像のハンドル*/
	// HP(前)
	int m_hpHandle0;
	// HP(後)
	int m_hpHandle1;
	// UI(名前、×)
	int m_UIHandle0;
	// UI(キービィ)
	int m_UIHandle1;
	// UI(バー)
	int m_UIHandle2;
	/* UIのサイズ*/
	int m_w;
	int m_h;
	// UIの位置
	int m_UIX;
	int m_UIY;
	// アニメーション用のフレーム
	int m_animationFrame;
	// 上がっているかどうか
	bool m_isUp;
	/*フォントのハンドル*/
	// 前
	int m_fontHandle;
	// 後
	int m_fontBackHandle;
	// ポインタの取得
	std::shared_ptr<Player> m_pPlayer;
};

