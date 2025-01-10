#pragma once
#include "../util/Vec2.h"
#include "../util/Rect.h"

#include <memory>
#include <vector>

class Player;
class Camera;
class Input;
class UI;
class StopEnemy;
class GoalFlower;
class Input;
class RecoveryItem;
class Tutorial02
{
public:
	Tutorial02();
	~Tutorial02();
	// 初期化
	void  Init();
	// 更新
	void Update(Input& input);
	// 描画
	void Draw();
	// チュートリアルのカウンタによる処理
	void CountUpdate();
	// 移動イベントの処理
	void MoveEvent();
	// マップチップの描画
	void ChipDraw();
	// 背景の描画
	void BackDraw();
	// シーン終了時の処理
	void End();
	// シーンを終了させたいか
	bool IsSceneEnd() const;
	// 敵キャラクターの初期化
	void EnemyInit();
	// アイテムの初期化
	void ItemInit();

	/*ポインタの設定*/
	// プレイヤー
	void SetPlayer(std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer; }
	// カメラ
	void SetCamera(std::shared_ptr<Camera> pCamera) { m_pCamera = pCamera; }
	// UI
	void SetUI(std::shared_ptr<UI> pUI) { m_pUI = pUI; }
	// 花
	void SetFlower(std::shared_ptr<GoalFlower> pFlower) { m_pFlower = pFlower; }
	// 止まっている敵
	void SetStopEnemy(std::shared_ptr<StopEnemy> pStopEnemy, int i);
	// アイテム
	void SetItem(std::shared_ptr<RecoveryItem> pItem, int i);

private:
	/*グラフィック*/
	// 背景
	int m_bgHandle;
	// 前景
	int m_fgHandle;
	// マップチップ
	int m_chipHandle;
	// ボタン
	int m_buttonHandle;
	// 移動イベント
	int m_evevtHandle;
	// チュートリアルのテキスト
	int m_windowTextHandle[21];
	// チュートリアルの矢印
	int m_arrowHandle;

	// グラフィックに含まれるマップチップの数
	int m_graphChipNumX;
	int m_graphChipNumY;
	// マップチップの配置情報
	std::vector<std::vector<int>> m_chipData;

	// ボタンのアニメーション
	int m_buttonAnimFrame;
	// 矢印のアニメーション
	int m_arrowAnimFrame;

	/*SE*/
	// ボタンを押したとき
	int m_pushSeHandle;
	// 移動イベント中
	int m_moveSeHandle;

	// フェードイン・アウト
	int m_fadeAlpha;
	
	// ウィンドウのカウント
	int m_tutorialCount;

	// 背景のスクロール
	float m_scrollX;
	int m_backW;
	int m_backH;

	// 移動イベント
	bool m_isEvent;
	int m_eventX;
	int m_eventY;
	int m_eventFrame;

	// 操作できるかどうか
	bool m_isOperation;
	// エネミーがいるかどうか
	bool m_isEnemy;
	// アイテムがあるかどうか
	bool m_isItem;
	// シーンが終了しているかどうか
	bool m_isSceneEnd;

	/*ポインタ*/
	// プレイヤー
	std::shared_ptr<Player> m_pPlayer;
	// カメラ
	std::shared_ptr<Camera> m_pCamera;
	// UI
	std::shared_ptr<UI> m_pUI;
	// 花
	std::shared_ptr<GoalFlower> m_pFlower;
	// エネミー
	std::vector<std::shared_ptr<StopEnemy>> m_pStopEnemy;
	// 回復アイテム
	std::vector<std::shared_ptr<RecoveryItem>> m_pItem;
};

