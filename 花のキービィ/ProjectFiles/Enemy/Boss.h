#pragma once
#include <memory>
#include "../util/Vec2.h"
#include "../util/Rect.h"


class Camera;
class Player;
class Inhale;
class Vomit;
class Boss
{
public:
	Boss();
	~Boss();

	// 初期化
	// 初期位置を設定するための引数を渡しておく
	void Init();
	// 更新
	void Update();
	// 描画
	void Draw();
	// 矩形情報の取得
	Rect GetRect();
	Rect GetLeftFlowerRect();
	Rect GetRightFlowerRect();

	// 当たり判定の取得
	float GetLeft()const;
	float GetRight() const;
	float GetTop() const;
	float GetBottom()const;

	// 周りに落ちる花の当たり判定の取得
	// 左の花
	float GetLeftFlowerLeft()const;
	float GetLeftFlowerRight()const;
	float GetLeftFlowerTop()const;
	float GetLeftFlowerBottom()const;
	// 右の花
	float GetRightFlowerLeft()const;
	float GetRightFlowerRight()const;
	float GetRightFlowerTop()const;
	float GetRightFlowerBottom()const;

	// プレイヤーとの当たり判定
	void PlayerCollision();
	void PlayerToLeftFlowerCollision();
	void PlayerToRightFlowerCollision();
	/*プレイヤーの攻撃の当たり判定*/
	// 吐き出しとの当たり判定
	void VomitCollision();
	void InhaleToLeftFlowerCollision();
	void InhaleToRightFlowerCollision();

	// 消える
	void DeleteExist() { m_isExist = false; }
	// 左右の花の描画
	void FlowerDraw();

	/*攻撃*/
	// 中央に落ちるジャンプ
	void JumpCenterFellAttack();
	// 左側に落ちるジャンプ
	void JumpLeftFellAttack();
	// 右側に落ちるジャンプ
	void JumpRightFellAttack();
	/*ポインタの設定*/
	// カメラ
	void SetCamera(std::shared_ptr<Camera> pCamera) { m_pCamera = pCamera; }
	// プレイヤー
	void SetPlayer(std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer; }
	// 吐き出し
	void SetVomit(std::shared_ptr<Vomit> pVomit) { m_pVomit = pVomit; }
	// 吸い込み
	void SetInhale(std::shared_ptr<Inhale> pInhale) { m_pInhale = pInhale; }

	// ボス戦が終わっているかどうか
	bool IsBossEnd()const { return m_isBossEnd; }

	// 存在しているかどうか
	bool IsExist()const { return m_isExist; }

private:
	/*ポインタの取得*/
	// カメラ
	std::shared_ptr<Camera> m_pCamera;
	// プレイヤー
	std::shared_ptr<Player> m_pPlayer;
	// 吐き出し
	std::shared_ptr<Vomit> m_pVomit;
	// 吸い込み
	std::shared_ptr<Inhale> m_pInhale;

	// 座標
	Vec2 m_pos;
	// スクロール用の座標
	float m_x;
	float m_y;
	// 左の花の座標
	Vec2 m_leftFlowerPos;
	// スクロール用の座標
	float m_leftFlowerX;
	float m_leftFlowerY;
	// 右の花の座標
	Vec2 m_rightFlowerPos;
	// スクロール用の座標
	int m_rightFlowerX;
	int m_rightFlowerY;
	// サイズ
	float m_w;
	float m_h;
	// 当たり判定用の矩形
	Rect m_colRect;
	Rect m_leftFlowerColRect;
	Rect m_rightFlowerColRect;

	/*グラフィック*/
	// 通常
	int m_normalHandle;
	// 弾に当たった時
	int m_damageHandle;
	// ジャンプの準備
	int m_preparationHandle;
	// 左右に落ちる花
	int m_flowerHandle;

	/*SE*/
	// 弾に当たった時
	int m_hitSeHandle;
	// 地面に着地したとき
	int m_landingSeHandle;
	// 落ちるとき
	int m_fellSeHandle;
	// 準備中
	int m_preparationSeHandle;
	// 倒れた時
	int m_endSeHandle;

	/*アニメーション*/
	// アニメーション用のSinのカウント
	float m_sinCount;
	// アニメーションの縦幅
	float m_animationHeight;

	/*技演出*/
	// 待機中かどうか
	bool m_isStandBy;
	// 攻撃感覚のフレーム数
	float m_attackIntervalFrame;

	/*ジャンプ*/
	// ジャンプ中かどうか
	// 中央
	bool m_isCenterJump;
	// 左
	bool m_isLeftJump;
	// 右
	bool m_isRightJump;
	// 一連の動きのフレーム数
	int m_jumpFrame;
	// 準備中かどうか
	bool m_isJumpPreparation;
	// 上昇中かどうか
	bool m_isJumpRise;
	// 落下中かどうか
	bool m_isJumpFell;
	// 定位置に戻っている途中かどうか
	bool m_isJumpFixedPos;
	// 定位置に戻ったかどうか
	bool m_isJumpFixedEndPos;
	// 花が存在しているかどうか
	bool m_isLeftFlower;
	bool m_isRightFlower;
	// 花が存在するフレーム数
	int m_flowerFrame;
	// 花が吸われている状態か
	bool m_isFlowerInhale;
	// 花の回転
	float m_flowerRotate;

	/*ダメージ演出*/
	// ダメージを受けたかどうか
	bool m_isDamage;
	// ダメージを受けてからのフレーム数
	int m_damageFrame;

	// HP
	int m_hp;

	// ボス戦が終わったかどうか
	bool m_isBossEnd;
	// ボス戦が終わってからのフレーム数
	int m_BossEndFrame;

	bool m_isEndSe;

	float m_bossRotate;

	// 存在しているかのフラグ
	bool m_isExist;

	// 仮
	unsigned int m_Color;
};

