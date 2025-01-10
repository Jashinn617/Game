#pragma once
#include "../util/Vec2.h"
#include "../util/Rect.h"

#include <memory>
#include <vector>

class Player;
class Camera;
class Input;
class UI;
class Door;
class Tutorial01
{
public:
	Tutorial01();
	~Tutorial01();

	// 初期化
	void  Init();
	// 更新
	void Update(Input& input);
	// 描画
	void Draw();
	// マップチップの描画
	void ChipDraw();
	// 背景の描画
	void BackDraw();
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

private:
	/*グラフィック*/
	// 前景
	int m_fgHandle;
	// 背景
	int m_bgHandle;
	// マップチップ
	int m_chipHandle;
	// ボタン
	int m_buttonHandle;
	// 説明用ウィンドウ
	int m_windowTextHandle[7];
	// 矢印
	int m_arrowHandle;

	// グラフィックに含まれるマップチップの数
	int m_graphChipNumX;
	int m_graphChipNumY;
	// マップチップの配置情報
	std::vector<std::vector<int>> m_chipData;

	// ボタンのアニメーション
	int m_buttonAnimFrame;
	// 矢印の点滅アニメーション
	int m_blinkingArrowAnimFrame;

	/*BGM*/
	int m_bgmHandle;

	/*SE*/
	// ボタンを押したとき
	int m_pushSeHandle;
	// ステージ移動時
	int m_moveSeHandle;

	// フェードイン・アウト
	int m_fadeAlpha;

	// ウィンドウのカウント
	int m_tutorialCount;

	// 背景のスクロール
	float m_scrollX;
	int m_backW;
	int m_backH;

	// 操作できるかどうか
	bool m_isOperation;
	// ドアが存在しているかどうか
	bool m_isDoor;
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
};

