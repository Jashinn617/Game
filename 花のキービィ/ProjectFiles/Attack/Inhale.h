#pragma once
#include "../util/Vec2.h"
#include "../util/Rect.h"

#include <memory>

class Camera;
class Player;
class Inhale
{
public:
	Inhale();
	~Inhale();

	// 初期化
	void Init();
	// 描画
	void Draw();
	// 更新
	void Update();
	// 矩形情報の取得
	Rect GetRect();

	/*当たり判定の取得*/
	float GetLeft()const;
	float GetTop() const;
	float GetRight() const;
	float GetBottom()const;

	// プレイヤーの左側に弾を出現させる
	void PutBulletLeft();
	// プレイヤーの右側に弾を出現させる
	void PutBulletRight();
	// 存在を消す
	void FalseExist();

	// カメラのポインタを設定する
	void SetCamera(std::shared_ptr<Camera> pCamera) { m_pCamera = pCamera; }
	// プレイヤーのポインタを設定する
	void SetPlayer(std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer; }

	// 弾が存在しているかどうか
	bool IsExsist()const { return m_isExist; }

private:
	// グラフィックのハンドル
	int m_inhaleHandle;
	// 吸い込み範囲
	int m_w;
	int m_h;
	// 位置
	Vec2 m_pos;
	// 弾が存在するかどうかのフラグ
	bool m_isExist;
	// 当たり判定の矩形
	Rect m_colRect;
	// アニメーション
	int m_animFrame;

	// スクロール量の取得用のCameraクラスのポインタ
	std::shared_ptr<Camera> m_pCamera;
	// プレイヤーのポインタ
	std::shared_ptr<Player> m_pPlayer;
};

