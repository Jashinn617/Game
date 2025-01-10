#pragma once
#include <memory>
#include "../util/Vec2.h"
#include "../util/Rect.h"

class Camera;
class Player;
class Inhale;
class Vomit;
class AirBullet;
class StopEnemy
{
public:
	StopEnemy();
	~StopEnemy();

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
	/*プレイヤーの攻撃の当たり判定*/
	// 吸い込みとの当たり判定
	void InhaleCollision();
	// 吐き出しとの当たり判定
	void VomitCollision();
	// 空気弾との当たり判定
	void AirBulletCollision();
	// 消える
	void DeleteExist() { m_isExist = false; }
	/*ポインタの設定*/
	// カメラ
	void SetCamera(std::shared_ptr<Camera> pCamera) { m_pCamera = pCamera; }
	// プレイヤー
	void SetPlayer(std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer; }
	// すいこみ
	void SetInhale(std::shared_ptr<Inhale> pInhale) { m_pInhale = pInhale; }
	// 吐き出し
	void SetVomit(std::shared_ptr<Vomit> pVomit) { m_pVomit = pVomit; }
	// 空気弾
	void SetAirBullet(std::shared_ptr<AirBullet>pAriBullet) { m_pAirBullet = pAriBullet; }
private:
	/*ポインタの取得*/
	// カメラ
	std::shared_ptr<Camera> m_pCamera;
	// プレイヤー
	std::shared_ptr<Player> m_pPlayer;
	// すいこみ
	std::shared_ptr<Inhale> m_pInhale;
	// 吐き出し
	std::shared_ptr<Vomit> m_pVomit;
	// 空気弾
	std::shared_ptr<AirBullet> m_pAirBullet;

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

	/*グラフィック*/
	// 通常
	int m_handle;
	// 弾に当たった時
	int m_damageEffectHandle;

	/*SE*/
	// 弾に当たった時
	int m_hitSeHandle;

	/*アニメーション*/
	// アニメーション用のSinのカウント
	float m_sinCount;
	// アニメーションの縦幅
	float m_animationHeight;

	/*消える時の演出*/
	// ダメージを受けたかどうか
	bool m_isDamage;
	// ダメージを受けてからのフレーム数
	int m_damageFrame;

	// 存在しているかのフラグ
	bool m_isExist;
	// 吸い込まれているかどうか
	bool m_isInhale;

	


};

