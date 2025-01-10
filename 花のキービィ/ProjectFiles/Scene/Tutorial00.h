#pragma once

class Input;
class Tutorial00
{
public:
	enum NextScene
	{
		kTutorial1,
		kStage01
	};

public:
	Tutorial00();
	~Tutorial00();

	void  Init();
	void Update(Input& input);
	void Draw();

	void End();

	// シーンを終了させたいか
	bool IsSceneEnd() const;


	// 次のシーンの設定
	NextScene IsNextScene()const { return m_nextScene; }

	NextScene GetStage01() const { return kStage01; }
	NextScene GetTutorial1() const { return kTutorial1; }

private:

	// 画像ハンドル
	int m_logo01;
	int m_logo02;
	int m_logo03;
	int m_cursor;

	// カーソルの移動
	int m_cursorCount;
	int m_cursorX;
	int m_cursorY;
	int m_cursorMove;

	// ロゴのアニメーション
	int m_logoY;

	// SEのハンドル
	int m_selectSeHandle;
	int m_startSeHandle;
	int m_landingLogoSeHandle;

	// フェードイン・アウト
	int m_fadeAlpha;

	bool m_isOperation;

	bool m_isRingSe;

	// シーン終了フラグ
	bool m_isSceneEnd;



	// 次のシーン
	NextScene m_nextScene;

};

