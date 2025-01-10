#pragma once
#include "../util/Vec2.h"
#include "../util/Rect.h"

#include <memory>
#include <vector>

class Player;
class Camera;
class Input;
class GoalFlower;
class UI;
class Boss;
class LastStage
{
public:
	// 次のシーン
	enum NextScene
	{
		kClear,
		kGameOver
	};

public:
	LastStage();
	~LastStage();

	// 初期化
	void  Init();
	// 更新
	void Update(Input& input);
	// 描画
	void Draw();
	// 終了処理
	void End();
	// シーンが終了したかどうか
	bool IsSceneEnd() const;

	// リスタート時の初期化
	void RestartInit();
	// 背景の描画
	void BackDraw();
	// マップチップの描画
	void ChipDraw();
	// プレイヤーがミスをした時の演出
	void OnMiss();
	// クリア時の演出
	void ClearPerformance();

	/*ポインタの初期化*/
	void SetPlayer(std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer; }
	void SetCamera(std::shared_ptr<Camera> pCamera) { m_pCamera = pCamera; }
	void SetUI(std::shared_ptr<UI> pUI) { m_pUI = pUI; }
	void SetFlower(std::shared_ptr<GoalFlower> pFlower) { m_pFlower = pFlower; }
	void SetBoss(std::shared_ptr<Boss> pBoss) { m_pBoss = pBoss; }

	/*次のシーンの設定*/
	// どのシーンに行くか取得する
	NextScene IsNextScene()const { return m_nextScene; }
	// 次のシーンをクリアシーンに設定する
	NextScene GetClear() const { return kClear; }
	// 次のシーンをゲームオーバーシーンに設定する
	NextScene GetGameOver() const { return kGameOver; }

private:
	/*グラフィック*/
	// ゲーム画面
	int m_gameScreenHandle;
	// 背景
	int m_bgHandle;
	// 前景
	int m_fgHandle;
	// マップチップ
	int m_chipHandle;
	// ゴールイベント
	int m_moveFlowerHandle;
	// ボスキャラクター
	int m_bossHandle;

	// グラフィックに含まれるマップチップの数
	int m_graphChipNumX;
	int m_graphChipNumY;
	// マップチップの配置情報
	std::vector<std::vector<int>> m_chipData;

	/*BGM*/
	// ステージのBGM
	int m_bgmHandle;

	/*SE*/
	// ミス演出
	int m_missSeHandle;
	// ミスをした瞬間
	int m_missSeHandle02;
	// 花での移動時のSE
	int m_flowerMoveSeHandle;

	// フェードイン・アウト
	int m_fadeAlpha;
	// ミスをした際のフェード
	bool m_isMissFade;

	/*ボス戦*/
	bool m_isBossExist;

	/*ミスをした時の画面の揺れ*/
	int m_shakeFrame;

	// 次のシーン
	NextScene m_nextScene;

	// ミスをしたかどうか
	bool m_isMiss;
	
	/*背景のスクロール*/
	// スクロール値
	float m_scrollX;
	// 横幅
	int m_backW;
	// 縦幅
	int m_backH;

	// クリアしたかどうか
	bool m_isClear;

	/*ゴール後の移動イベント*/
	// 移動イベントの移動値X
	int m_moveEventX;
	// 移動イベントの移動値Y
	int m_moveEventY;
	// 移動イベントのフレーム数
	int m_moveEventFrame;
	// 拡大率
	float m_extRate;

	// シーンが終了しているかどうか
	bool m_isSceneEnd;

	// ポインタの設定
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<UI> m_pUI;
	std::shared_ptr<GoalFlower> m_pFlower;
	std::shared_ptr<Boss> m_pBoss;
};

