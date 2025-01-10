#pragma once
#include "../Game.h"
#include "../util/Vec2.h"
#include "../util/Rect.h"

#include <memory>
#include <vector>

class Camera;
class Player;
class GameOverScene;
class StopEnemy;
class WalkEnemy;
class FlyEnemy;
class RecoveryItem;
class Input;
class UI;
class Door;
class Stage_01
{
public:
	// 次のシーン
	enum NextScene
	{
		kStage02,	// ステージ2
		kGameOver	// ゲームオーバー
	};
public:
	Stage_01();
	~Stage_01();

	// 初期化
	void Init();
	// 更新
	void Update(Input& input);
	// 描画
	void Draw();
	// シーン終了時の処理
	void End();
	// シーンが終了したかどうか
	bool IsSceneEnd() const;
	// リスタート時の初期化
	void RestartInit();
	// 敵キャラクターの初期化
	void EnemyInit();
	// アイテムの初期化
	void ItemInit();
	// 敵キャラクターの更新
	void EnemyUpdate();
	// アイテムの更新
	void ItemUpdate();
	// 背景の描画
	void BackDraw();
	// マップチップの描画
	void ChipDraw();
	// 敵キャラクターの描画
	void EnemyDraw();
	// アイテムの描画
	void ItemDraw();
	// プレイヤーがミスをした時の演出
	void OnMiss();

	/*ポインタの設定*/
	// プレイヤー
	void SetPlayer(std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer; }
	// カメラ
	void SetCamera(std::shared_ptr<Camera> pCamera) { m_pCamera = pCamera; }
	// UI
	void SetUI(std::shared_ptr<UI> pUI) { m_pUI = pUI; }
	// ゲームオーバーシーン
	void SetGameOver(std::shared_ptr<GameOverScene> pGameOver) { m_pGameOver = pGameOver; }
	// 移動用のドア
	void SetDoor(std::shared_ptr<Door> pDoor) { m_pDoor = pDoor; }
	// 止まっている敵
	void SetStopEnemy(std::shared_ptr<StopEnemy> pStopEnemy, int i) { m_pStopEnemy[i] = pStopEnemy; }
	// 歩いている敵
	void SetWalkEnemy(std::shared_ptr<WalkEnemy> pWalkEnemy, int i) { m_pWalkEnemy[i] = pWalkEnemy; }
	// 飛んでいる敵
	void SetFlyEnemy(std::shared_ptr<FlyEnemy> pFlyEnemy, int i) { m_pFlyEnemy[i] = pFlyEnemy; }
	// 回復アイテム
	void SetItem(std::shared_ptr<RecoveryItem> pRecoveryItem, int i) { m_pRecoveryItem[i] = pRecoveryItem; }

	/*次のシーンの設定*/
	// どのシーンに行くか取得する
	NextScene IsNextScene()const { return m_nextScene; }
	// 次のシーンをステージ2に設定する
	NextScene GetStage02() const { return kStage02; }
	// 次のシーンをゲームオーバーに設定する
	NextScene GetGameOver() const { return kGameOver; }
private:
	/*グラフィック*/
	// ゲーム画面
	int m_gameScreenHandle;
	// 背景
	int m_backgroundHandle;
	// マップチップ
	int m_chipHandle;

	// グラフィックに含まれるマップチップの数
	int m_graphChipNumX;
	int m_graphChipNumY;
	// マップチップの配置情報
	std::vector<std::vector<int>> m_chipData;

	/*BGM*/
	int m_bgmHandle;

	/*SE*/
	// ミス演出
	int m_missSeHandle;
	// ミスをした瞬間
	int m_missSeHandle2;
	// ステージ移動時
	int m_moveSeHandle;

	// フェードイン・アウト
	int m_fadeAlpha;
	int m_backAlpha;
	// ミスをした際のフェード
	bool m_ismissFade;
	
	/* ミスをした時の画面揺れ*/
	int m_shakeFrame;

	// 次のシーン
	NextScene m_nextScene;

	// ミスをしたかどうか
	bool m_isMiss;

	// シーンが終了しているかどうか
	bool m_isSceneEnd;

	/*ポインタ*/
	// プレイヤー
	std::shared_ptr<Player> m_pPlayer;
	// カメラ
	std::shared_ptr<Camera> m_pCamera;
	// UI
	std::shared_ptr<UI> m_pUI;
	// ゲームオーバーシーン
	std::shared_ptr<GameOverScene> m_pGameOver;
	// ドア
	std::shared_ptr<Door> m_pDoor;
	/*エネミー*/
	// 止まっている敵
	std::vector<std::shared_ptr<StopEnemy>> m_pStopEnemy;
	// 歩く敵
	std::vector < std::shared_ptr<WalkEnemy>> m_pWalkEnemy;
	// 飛ぶ敵
	std::vector<std::shared_ptr<FlyEnemy>> m_pFlyEnemy;
	// 回復アイテム
	std::vector<std::shared_ptr<RecoveryItem>> m_pRecoveryItem;
};

