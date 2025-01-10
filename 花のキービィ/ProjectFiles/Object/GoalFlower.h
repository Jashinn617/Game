#pragma once
#include "../util/Vec2.h"
#include "../util/Rect.h"

#include <memory>

class Camera;
class Player;
class GoalFlower
{
public:
	GoalFlower();
	~GoalFlower();

	// 初期化
	// 初期位置を設定するための引数を渡しておく
	void Init(float x, float y);
	// 更新
	void Update();
	// 描画
	void Draw();
	// 矩形情報の取得
	Rect GetRect();
	// 当たり判定の取得
	float GetLeft()const;
	float GetRight() const;
	float GetTop() const;
	float GetBottom()const;
	// プレイヤーとの当たり判定
	void PlayerCollision();
	// プレイヤーと当たっているかどうかの取得
	bool IsColision()const { return m_isColition; }
	// 花の色を付ける
	void SetFlowerColor() { m_isColor = true; }
	// 花の色をモノクロにする
	void SetFlowerMonochrome() { m_isColor = false; }

	/*ポインタの設定*/
		// カメラ
	void SetCamera(std::shared_ptr<Camera> pCamera) { m_pCamera = pCamera; }
	// プレイヤー
	void SetPlayer(std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer; }
	// 位置の変更
	void RePosition(float x, float y);

private:
	/*ポインタの取得*/
		// カメラ
	std::shared_ptr<Camera> m_pCamera;
	// プレイヤー
	std::shared_ptr<Player> m_pPlayer;

	// 座標
	Vec2 m_pos;
	// スクロール用の座標
	int m_x;
	int m_y;
	// サイズ
	int m_w;
	int m_h;
	// 当たり判定用の矩形
	Rect m_colRect;
	// 回転
	float m_decorationAngle;

	/*グラフィック*/
	int m_colorHandle;
	int m_monochoromeHandle;

	// 色がついているかどうか
	int m_isColor;

	// プレイヤーに当たっているかどうか
	bool m_isColition;
};

