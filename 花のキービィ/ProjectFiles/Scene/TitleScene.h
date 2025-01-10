#pragma once
#include <memory>

class Input;
class Player;
class TitleScene
{
public:
	TitleScene();
	~TitleScene();

	void Init();
	void Update(Input& input);
	void Draw();

	// 背景のスクロール処理
	void BackDraw();

	void End();

	// シーンを終了させたいか
	bool IsSceneEnd() const;

	// ポインタを設定する
	void SetPlayer(std::shared_ptr<Player> pPlyer) { m_pPlayer = pPlyer; }


private:
	// 画像のハンドル
	int m_titleHandle;
	int m_endHandle;
	int m_startHandle;
	int m_cursorHandle;
	int m_backHandle;
	int m_animationPlayer;

	// 画像のサイズ
	int m_titleW;
	int m_titleH;
	int m_startW;
	int m_startH;
	int m_endW;
	int m_endH;

	// 画像の位置
	float m_titleX;
	float m_titleY;
	float m_startX;
	float m_startY;
	float m_endX;
	float m_endY;
	float m_cursorX;
	float m_cursorY;

	float m_playerX;
	float m_playerY;

	// 画像のスケール値
	float m_startScale;
	float m_endScale;

	// 背景のスクロール
	float m_scrollX;
	int m_backW;
	int m_backH;

	// カーソル移動
	int m_cursorCount;

	// BGMのハンドル
	int m_bgmHandle;
	// SEのハンドル
	int m_selectHandle;
	int m_startSeHandle;

	// カーソルアニメーション
	float m_cursorSinCount;
	float m_cursorAnimationPosX;

	// プレイヤーのアニメーション
	int m_animFrame;
	float m_playerSinCount;
	float m_playerAnimationPosY;

	// フェードイン・アウト
	int m_fadeAlpha;

	bool m_isSceneEnd;	// シーン終了フラグ(trueの場合はシーンを終了する)

	std::shared_ptr<Player> m_pPlayer;
};

