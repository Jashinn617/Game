#pragma once
#include <memory>
#include "../util/Vec2.h"
#include "../util/Rect.h"

class Camera;
class Player;
class Inhale;
class RecoveryItem
{
public:
	RecoveryItem();
	~RecoveryItem();

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
	// 吸い込みとの当たり判定
	void InhaleCollision();
	// 消える
	void DeleteExist() { m_isExist = false; }
	/*ポインタの設定*/
		// カメラ
	void SetCamera(std::shared_ptr<Camera> pCamera) { m_pCamera = pCamera; }
	// プレイヤー
	void SetPlayer(std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer; }
	// すいこみ
	void SetInhale(std::shared_ptr<Inhale> pInhale) { m_pInhale = pInhale; }
private:
	/*ポインタの取得*/
	// カメラ
	std::shared_ptr<Camera> m_pCamera;
	// プレイヤー
	std::shared_ptr<Player> m_pPlayer;
	// すいこみ
	std::shared_ptr<Inhale> m_pInhale;

	// 座標
	Vec2 m_pos;
	// スクロール用の位置
	int m_x;
	int m_y;
	// サイズ
	int m_w;
	int m_h;
	// 当たり判定用の矩形
	Rect m_colRect;

	/*グラフィック*/
	int m_handle;

	/*SE*/
	int m_hitSeHandle;
	
	/*アニメーション*/
	// アニメーション用のSinのカウント
	float m_sinCount;
	// アニメーションの縦幅
	float m_animationHeight;	

	// 存在しているかのフラグ
	bool m_isExist;
	// 吸い込まれているかどうか
	bool m_isInhale;
};

