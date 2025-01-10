#pragma once
#include "../util/Input.h"
#include <memory>

class Player;
class GameOverScene
{
public:
	enum NextScene
	{
		kStage01,
		kTitle
	};

public:
	GameOverScene();
	~GameOverScene();

	// 初期化
	void Init();
	// 更新
	void Update(Input& input);
	// 描画
	void Draw();
	// シーン終了時の処理
	void End();
	// シーンが終了したかどうか
	bool IsSceneEnd();
	// 次のシーン
	NextScene IsNextScene() const { return m_nextScene; }
	// 次のシーンを変更する
	void ChangeNextScene(NextScene nextScene) { m_nextScene = nextScene; }
	NextScene GetTitle() const { return kTitle; }
	NextScene GetStage01() const { return kStage01; }

	// プレイヤーのポインタを設定する
	void SetPlayer(std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer; }
private:
	/*グラフィック*/
	int m_gameOverBg;
	int m_gameEndLogo;
	int m_gameOverLogo;
	int m_continueLogo;
	int m_cursor;
	int m_gameOverChara;

	/*BGM*/
	int m_bgmHandle;

	/*SE*/
	int m_gameEndSeHandle;
	int m_continueSeHandle;

	// 画像のサイズ
	int m_endW;
	int m_endH;
	int m_continueW;
	int m_continueH;
	int m_gameOverW;
	int m_gameOverH;

	// 画像の位置
	float m_gameOverX;
	float m_gameOverY;
	float m_continueX;
	float m_continueY;
	float m_endX;
	float m_endY;
	float m_cursorX;
	float m_cursorY;

	// 画像のスケール値
	float m_continueScale;
	float m_endScale;

	// カーソル移動用のカウント
	int m_cursorCount;

	// フェードイン・アウト
	int m_fadeAlpha;
	// 次のシーンの設定
	NextScene m_nextScene;

	// ポインタの取得
	std::shared_ptr<Player> m_pPlayer;

	// シーンが終了したかどうか
	bool m_isSceneEnd;
};

