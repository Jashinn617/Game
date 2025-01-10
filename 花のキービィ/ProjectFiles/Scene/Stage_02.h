#pragma once
#include "../Game.h"
#include "../util/Rect.h"
#include "../util/Vec2.h"

#include <memory>
#include <vector>

class Camera;
class Player;
class StopEnemy;
class WalkEnemy;
class FlyEnemy;
class Input;
class UI;
class Needle;
class RecoveryItem;
class Door;
class Stage_02
{
public:
	// 次のシーン
	enum NextScene
	{
		kLastStage,
		kGameOver
	};
	// ダンスの振り付け
	enum DanceChoreography
	{
		kRotate,
		kMove
	};
	// ダンス中の向き
	enum DanceDir
	{
		kDirLeft,
		kDirRight
	};
public:
	Stage_02();
	~Stage_02();

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
	
	/*ポインタの設定*/
	// プレイヤー
	void SetPlayer(std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer; }
	// カメラ
	void SetCamera(std::shared_ptr<Camera> pCamera) { m_pCamera = pCamera; }
	// UI
	void SetUI(std::shared_ptr<UI> pUI) { m_pUI = pUI; }
	// 移動用のドア
	void SetDoor(std::shared_ptr<Door> pDoor) { m_pDoor = pDoor; }
	// 止まっている敵
	void SetStopEnemy(std::shared_ptr<StopEnemy> pStopEnemy, int i) { m_pStopEnemy[i] = pStopEnemy; }
	// 歩いている敵
	void SetWalkEnemy(std::shared_ptr<WalkEnemy> pWalkEnemy, int i) { m_pWalkEnemy[i] = pWalkEnemy; }
	// 飛んでいる敵
	void SetFlyEnemy(std::shared_ptr<FlyEnemy> pFlyEnemy, int i) { m_pFlyEnemy[i] = pFlyEnemy; }
	// 針
	void SetNeedle(std::shared_ptr<Needle> pNeedle, int i) { m_pNeedle[i] = pNeedle; }
	// 回復アイテム
	void SetItem(std::shared_ptr<RecoveryItem> pRecoveryItem, int i) { m_pRecoveryItem[i] = pRecoveryItem; }

	/*次のシーンの設定*/
	// どのシーンに行くか取得する
	NextScene IsNextScene()const { return m_nextScene; }
	// 次のシーンをクリアシーンに設定する
	NextScene GetLastStage() const { return kLastStage; }
	// 次のシーンをゲームオーバーシーンに設定する
	NextScene GetGameOver() const { return kGameOver; }
private:
	// リスタート時の初期化
	void RestartInit();
	// 敵キャラクターの初期化
	void EnemyInit();
	// 針の初期化
	void NeedleInit();
	// アイテムの初期化
	void ItemInit();
	// 敵キャラクターの更新
	void EnemyUpdate();
	// 針の更新
	void NeedleUpdate();
	// アイテムの更新
	void ItemUpdate();
	// 背景の描画
	void BackDraw();
	// マップチップの描画
	void ChipDraw();
	// 敵キャラクターの描画
	void EnemyDraw();
	// 針の描画
	void NeedleDraw();
	// アイテムの描画
	void ItemDraw();
	// 音量設定
	void VolumeSetting();
	// プレイヤーがミスをした時の演出
	void OnMiss();
private:
	/*グラフィック*/
	// ゲーム画面
	int m_gameScreenHandle;
	// 背景
	int m_background;
	// 前景
	int m_foreground;
	// マップチップ
	int m_chipHandle;
	// ゴールイベント
	int m_moveFlowerHandle;

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
	// ゴールの近くで流れているSE
	int m_goalSeHandle;
	// ステージ移動時
	int m_moveSeHandle;
	// SEの音量
	int m_goalSeVolume;

	// フェードイン・アウト
	int m_fadeAlpha;
	// ミスをした際のフェード
	bool m_isMissFade;

	/*ミスをした時の画面の揺れ*/
	int m_shakeFrame;

	// 次のシーン
	NextScene m_nextScene;

	// ミスをしたかどうか
	bool m_isMiss;

	/*前景のスクロール*/
	// スクロール値
	float m_scrollX;
	// 横幅
	int m_fgW;
	// 縦幅
	int m_fgH;

	// ゴールしたかどうか
	bool m_isGoal;	

	/*ゴール後の移動イベント*/
	// 移動イベントの移動値X
	int m_moveEventX;
	// 移動イベントの移動値Y
	int m_moveEventY;
	// 移動イベントのフレーム数
	int m_moveEventFrame;
	// 拡大率
	float m_ExtRate;

	// シーンが終了しているかどうか
	bool m_isSceneEnd;

	/*ポインタ*/
	// プレイヤー
	std::shared_ptr<Player> m_pPlayer;
	// カメラ
	std::shared_ptr<Camera> m_pCamera;
	// UI
	std::shared_ptr<UI> m_pUI;
	// ドア
	std::shared_ptr<Door> m_pDoor;
	/*エネミー*/
	// 止まっている敵
	std::vector<std::shared_ptr<StopEnemy>> m_pStopEnemy;
	// 歩く敵
	std::vector < std::shared_ptr<WalkEnemy>> m_pWalkEnemy;
	// 飛ぶ敵
	std::vector<std::shared_ptr<FlyEnemy>> m_pFlyEnemy;
	/*オブジェクト*/
	// 針
	std::vector < std::shared_ptr <Needle>> m_pNeedle;
	// 回復アイテム
	std::vector<std::shared_ptr<RecoveryItem>> m_pRecoveryItem;
};

