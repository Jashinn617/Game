#pragma once
#include "SceneBase.h"

class Player;
class Camera;
class RecoveredItem;

/// <summary>
/// メインシーンクラス
/// </summary>
class SceneMain : public SceneBase
{
public:
	SceneMain();
	virtual ~SceneMain();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="handle">ハンドル</param>
	virtual void Init(HandleManager& handle);
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力</param>
	/// <param name="handle">ハンドル</param>
	/// <returns>シーンポインタ</returns>
	virtual shared_ptr<SceneBase> Update(Input& input, HandleManager& handle);
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="handle">ハンドル</param>
	virtual void Draw(HandleManager& handle);
	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void End();

private:	// 関数
	/// <summary>
	/// アイテムの初期化
	/// </summary>
	/// <param name="handle">ハンドル</param>
	void ItemInit(HandleManager& handle);

	/// <summary>
	/// スタート画面の更新
	/// </summary>
	/// <param name="input">入力</param>
	/// <param name="handle">ハンドル</param>
	void StartUpdate(Input& input, HandleManager& handle);
	/// <summary>
	/// カウントダウンの更新
	/// </summary>
	/// /// <param name="input">入力</param>
	/// <param name="handle">ハンドル</param>
	void CountdownUpdate(Input& input, HandleManager& handle);
	/// <summary>
	/// プレイ画面の更新
	/// </summary>
	/// <param name="input">入力</param>
	/// <param name="handle">ハンドル</param>
	void PlayUpdate(Input& input, HandleManager& handle);
	/// <summary>
	/// クリア画面の更新
	/// </summary>
	/// <param name="input">入力</param>
	/// <param name="handle">ハンドル</param>
	void ClearUpdate(Input& input, HandleManager& handle);

	/// <summary>
	/// スタート画面の描画
	/// </summary>
	/// <param name="handle">ハンドル</param>
	void StartDraw(HandleManager& handle);
	/// <summary>
	/// カウントダウン画面の描画
	/// </summary>
	/// <param name="handle"></param>
	void CountdownDraw(HandleManager& handle);
	/// <summary>
	/// プレイ画面の描画
	/// </summary>
	/// <param name="handle">ハンドル</param>
	void PlayDraw(HandleManager& handle);
	/// <summary>
	/// クリア画面の描画
	/// </summary>
	/// <param name="handle">ハンドル</param>
	void ClearDraw(HandleManager& handle);
	/// <summary>
	/// 背景の描画
	/// </summary>
	/// <param name="handle">ハンドル</param>
	void BackDraw(HandleManager& handle);

private:	// 変数
	float m_scrollX;			// 背景のスクロール
	int m_shadowMapHandle;		// シャドウマップのハンドル
	int m_timeFrame;			// 経過フレーム
	int m_time;					// 経過秒数
	int m_clearTime;			// クリア時の秒数
	int m_itemNum;				// とったアイテム数
	int m_fadeAlpha;			// フェードイン・アウトの濃さ
	int m_countdownFrame;		// カウントダウンの経過フレーム
	double m_countdownScale;	// カウントダウンのスケール
	bool m_isStart;				// スタート画面かどうか
	bool m_isCountdown;			// カウントダウン中かどうか
	bool m_isPlay;				// プレイ画面かどうか
	bool m_isClear;				// ゴール画面かどうか
	bool m_isSe;				// SEがなったかどうか

	/*ポインタ*/
	shared_ptr<Player> m_pPlayer;					// プレイヤーのポインタ
	shared_ptr<Camera> m_pCamera;					// カメラのポインタ
	vector<shared_ptr<RecoveredItem>> m_pItem;		// アイテムのポインタ

	// 背景画像のサイズ
	struct BgSize	
	{
		int width;	// 横
		int height;	// 縦
	};

private:	 // 定数
	static constexpr float kWallX = 50;						// x軸の壁
	static constexpr float kWallZ = 30;						// z軸の壁
	static constexpr int kItemNum = 20;						// アイテムの数
	static constexpr int kFadeSpeed = 2;					// フェードのスピード
	static constexpr float kGroundModelScale = 20;			// 地面モデルのスケール
	static constexpr float kGroundPosY = -1.2f;				// 地面のY位置
	static constexpr float kCountdownTime = 3 * 60;			// カウントダウンをする秒数
	static constexpr float kOneSecond = 60;					// 1秒
	static constexpr float kTwoSecond = 120;				// 2秒
	static constexpr double kCountdownShrinkSpeed = 0.01;	// カウントダウンの秒数が縮まるスピード
	static constexpr double kCountdownMaxScale = 0.2;		// カウントダウンの最大スケール
	static constexpr int kScreenPosX = 200;					// スタート、クリアの画像のX位置
	static constexpr int kScreenPosY = 80;					// スタート、クリアの画像のY位置
	static constexpr float kScrollSpeed = 1.5f;				// 背景のスクロールスピード
	static constexpr int kIndexBack = 2;					// スクロールに必要な背景の数
	static constexpr int kLogoX = 200;						// スタート、クリアのロゴのX位置
	static constexpr int kLogoY = 640;						// スタート、クリアのロゴのY位置
	/*秒数の表示位置*/
	static constexpr int kCountdownPosX = 800;				// カウントダウンのX位置
	static constexpr int kCountdownPosY = 200;				// カウントダウンのY位置
	static constexpr int kGoPosX = 800;						// Goの文字のX位置
	static constexpr int kGoPosY = 300;						// Goの文字のY位置
	static constexpr int kTimePosX = 600;					// 秒数のX位置
	static constexpr int kTimePosY = 40;					// 秒数のY位置
	static constexpr int kItemPosX = 550;					// アイテムのX位置
	static constexpr int kItemPosY = 150;					// アイテムのY位置
	static constexpr int kClearTimePosX = 500;				// クリアタイムのX位置
	static constexpr int kClearTimePosY = 150;				// クリアタイムのY位置
	static constexpr int kBackFontShiftPosX = 10;			// 後ろのフォントをずらす位置
	static constexpr int kFrontCharColor = 0x00008b;		// 手前側の文字の色
	static constexpr int kBackCharColor = 0xff6347;			// 後ろ側の文字の色
	/*クリア時のタイム*/
	static constexpr int kClearFastTime = 25;				// 速いクリア
	static constexpr int kClearSlowTime = 45;				// 遅いクリア
	/*シャドウマップ*/
	static constexpr int kShadowMapSizeX = 1024;							// シャドウマップのサイズX
	static constexpr int kShadowMapSizeY = 1024;							// シャドウマップのサイズY
	static constexpr VECTOR kShadowMapRangeMin = { -55.0f,-1.2f,-35.0f };	// シャドウマップを描画する最大範囲
	static constexpr VECTOR kShadowMapRangeMax = { 55.0f,10.0f,35.0f };		// シャドウマップを描画する最小範囲
	static constexpr VECTOR kShadowMapLightDir = { 0.01f, -0.5f, 0.0f };	// シャドウマップが想定するライトの方向
	
	static constexpr VECTOR kLightDir = { 0.0f,-0.5f,0.0f };				// ライトの方向
};