#pragma once
#include "SceneBase.h"
#include "DxLib.h"

class Player;
class Camera;
class Map;
class EnemyManager;
class Input;

/// <summary>
/// メインシーン
/// </summary>
class MainScene:public SceneBase
{
public:
	MainScene();
	virtual ~MainScene();

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
	/// 秒数の経過を知らせるSEを鳴らす
	/// </summary>
	void NoticeSeRing();
	/// <summary>
	/// 秒数の経過を知らせるテキストの点滅処理
	/// </summary>
	void NoticeBlinking();
	/// <summary>
	/// 秒数の描画
	/// </summary>
	void TimeDraw();
	/// <summary>
	/// 秒数の経過を知らせるテキストの描画
	/// </summary>
	void NoticeDraw();
	/// <summary>
	/// 背景の描画
	/// </summary>
	void BackDraw();

private:	// 変数

	VECTOR m_bgPos;				// 背景の表示位置
	int m_bgHandle;				// 背景画像

	int m_noticeLogoHandle1;	// 20秒の経過を知らせるテキスト
	int m_noticeLogoHandle2;	// 40秒の経過を知らせるテキスト
	int m_noticeLogoHandle3;	// 60秒の経過を知らせるテキスト
	int m_noticeLogoHandle4;	// 80秒の経過を知らせるテキスト
	int m_tutorialHandle;		// チュートリアルのテキスト

	int m_bgmHandle;			// BGMのハンドル
	int m_clearSeHandle;		// クリア時のSEのハンドル
	int m_noticeSeHandle;		// 秒数経過を知らせるSEのハンドル
	int m_missSeHandle;			// ミスをした時のSEのハンドル

	int m_timeFontHandle;		// 秒数表示用のフォントのハンドル(前)
	int m_timeBackFontHandle;	// 秒数表示用のフォントのハンドル(後)
	
	int m_timeFrame;			// 経過フレーム
	int m_time;					// 経過秒数

	int m_blinkingNoticeFrame;	// 秒数通知の点滅フレーム
	bool m_isNotice;			// 秒数通知が表示されているか

	struct BgSize				// 背景画像のサイズ
	{
		int width;	// 横
		int height;	// 縦
	};

	/*ポインタの取得*/
	shared_ptr<Player> m_pPlayer;		// プレイヤー	
	shared_ptr<Camera> m_pCamera;		// カメラ
	shared_ptr<Map> m_pMap;				// マップ	
	shared_ptr<EnemyManager> m_pEnemy;	// 敵キャラクター

private:	// 定数
	static constexpr float kBgScale = 0.5f;		// 背景画像のスケールサイズ
	static constexpr float kBgSpeed = -1.0f;	// 背景の移動スピード

	/*背景の描画位置*/
	static constexpr float kBgPosX = 0.0f;		// X位置
	static constexpr float kBgPosY = 0.0f;		// Y位置
	static constexpr float kBgPosZ = 200.0f;	// Z位置

	static constexpr int kBgmVolume = 130;		// BGMの音量
	static constexpr int kSeVolume = 255;		// SEの音量

	static constexpr int kScreenWidth = 1600;	// 画面の横サイズ
	static constexpr int kScreenHeight = 900;	// 画面の縦サイズ

	static constexpr int kFadeSpeed = 3;		// フェードのスピード

	static constexpr int kNoticeBlinkingSpeed = 30;		// 秒数通知の点滅速度
	static constexpr int kNoticeBlinkingFrequency = 20; // 秒数通知の点滅頻度

	/*秒数通知の描画位置*/
	static constexpr float kNoticePosX = 850;		// X位置
	static constexpr float kNoticePosY = 400;		// Y位置
	static constexpr double kNoticeExtRate = 0.3;	// 拡大率

	/*秒数の表示位置*/
	static constexpr int kTimePosX = 750;			// X位置
	static constexpr int kTimePosY = 80;			// Y位置

	/*秒数通知をする間隔*/
	static constexpr int kTimeInterval1 = 20;
	static constexpr int kTimeInterval2 = kTimeInterval1 + 20;
	static constexpr int kTimeInterval3 = kTimeInterval2 + 20;
	static constexpr int kTimeInterval4 = kTimeInterval3 + 20;

	static constexpr int kNoticeDisplayTime = 3;	//秒数通知を表示する秒数
	static constexpr int kClearTime = 90;			// クリアタイム
};

