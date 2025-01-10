#pragma once
#include "SceneBase.h"

class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	~SceneTitle();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力</param>
	/// <returns>現在のシーン</returns>
	virtual shared_ptr<SceneBase> Update(Input& input);
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();
	/// <summary>
	/// シーン終了時の処理
	/// </summary>
	virtual void End();

private:	// 関数
	/// <summary>
	/// カーソルの移動
	/// </summary>
	/// <param name="input">入力</param>
	void CursorMove(Input& input);

private:	// 変数
	int m_titleLogoHandle;			// タイトルロゴの画像
	int m_startLogoHandle;			// スタートの画像
	int m_endLogoHandle;			// ゲーム終了の画像
	int m_selectCursorHandle;		// 選択用のカーソルの画像
	int m_bgHandle;					// 背景の画像
	int m_bgmHandle;				// BGM
	int m_cursorMoveSeHandle;		// カーソルを移動した際のSE
	int m_startSeHandle;			// スタート時のSE

	double m_startExtRate;			// スタートの画像の拡大率
	double m_endExtRate;			// ゲーム終了の画像の拡大率

	float m_cursorSinCount;			// カーソルを動かす際のカウント
	float m_cursorSinPosX;			// カーソルの位置計算に使うSin
	float m_cursorPosY;				// カーソルのY位置

	int m_cursorCount;				// カーソルの表示位置用のカウント

	int m_fadeAlpha;				// フェードイン、アウト

	bool m_isSceneEnd;				// シーンが終了したかどうか

private: // 定数
	static constexpr float kTitlePosX = 800.0f;	// タイトルのX位置
	static constexpr float kTitlePosY = 250.0f;	// タイトルのY位置
	static constexpr double kTitleExtRate = 0.5;	// タイトルの拡大率

	static constexpr float kStartPosX = kTitlePosX;	// スタートのX位置
	static constexpr float kStartPosY = kTitlePosY + 300.0f;	// スタートのY位置
	static constexpr double kSmallStartExtRate = 0.3;	// スタートの拡大率(通常時)
	static constexpr double kBigStartExtRate = 0.35;	// スタートの拡大率(拡大時)

	static constexpr float kEndPosX = kTitlePosX;	// エンドのX位置
	static constexpr float kEndPosY = kStartPosY + 200.0f;	// エンドのY位置
	static constexpr double kSmallEndExtRate = kSmallStartExtRate;	// エンドの拡大率(通常時)
	static constexpr double kBigEndExtRate = kBigStartExtRate;	// エンドの拡大率(拡大時)

	static constexpr float kCursorPosX = kTitlePosX - 300.0f;	// カーソルのX位置
	static constexpr double kCursorExtRate = 0.15;	// カーソルの拡大率

	static constexpr float kCursorSinSpeed = 0.08f;	// カーソルの移動スピード
	static constexpr float kCursorAnimationSwing = 30.0f;	// カーソルの移動幅


	static constexpr int kBgmVolume = 130;	// BGMの音量
	static constexpr int kSeVolume = 255;	// SEの音量

	static constexpr int kScreenWidth = 1600;	// 画面の横サイズ
	static constexpr int kScreenHeight = 900;	// 画面の縦サイズ

	static constexpr int kFadeSpeed = 3;	// フェードのスピード
};

