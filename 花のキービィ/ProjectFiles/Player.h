#pragma once
#include "util/Vec2.h"
#include "util/Rect.h"

#include <memory>
#include <vector>

class Vomit;
class Inhale;
class AirBullet;
class Input;
class Camera;
class StopEnemy;
class WalkEnemy;
class Player
{
public:
	// プレイヤーの向き
	enum Dir
	{
		kDirRight,	// 右
		kDirLeft	// 左
	};
	// プレイヤーの状態
	enum Condition
	{
		kMove,		// 移動
		kInhale,	// すいこみ
		kVomit,		// 吐き出し
		kGulp,		// ほおばり
		kJump,		// ジャンプ
		kHovering,	// ホバリング
		kFall,		// 落下
		kMiss,		// ミス
		kDance		// ダンス
	};
	// ダンスの振り付け
	enum DanceChoreography
	{
		kRotate,		// 回転
		kLateralMove	// 横移動
	};

public:
	Player();
	~Player();

	// 初期化
	void Init();
	// 描画
	void Draw();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力情報</param>
	/// <param name="mapData">マップチップの配置情報</param>
	/// <param name="chipNumX">マップチップの横の配置数</param>
	/// <param name="chipNumY">マップチップの縦の配置数</param>
	/// <param name="chipWidth">マップチップ一つの横幅</param>
	/// <param name="chipHeight">マップチップ一つの縦幅</param>
	void Update(Input& input,
		std::vector<std::vector<int>> mapData,
		int chipNumX, int chipNumY,
		int chipWidth, int chipHeight);
	/// <summary>
	/// マップチップとの当たり判定の取得
	/// </summary>
	/// <param name="mapData">マップチップの配置情報</param>
	/// <param name="chipNumX">マップチップの横の配置数</param>
	/// <param name="chipNumY">マップチップの縦の配置数</param>
	/// <param name="chipWidth">マップチップ一つの横幅</param>
	/// <param name="chipHeight">マップチップ一つの縦幅</param>
	/// <param name="playerRect">プレイヤーの当たり判定の矩形</param>
	/// <param name="chipRect">マップチップの当たり判定の矩形</param>
	/// <returns>マップチップと当たっているかどうか</returns>
	bool IsMapCollision(std::vector<std::vector<int>> mapData,
		int chipNumX, int chipNumY,
		int chipWidth, int chipHeight,
		Rect playerRect,
		Rect& chipRect);
	// ダンスの描画
	void DanceDraw();
	// 吸い込み
	void AttackInhale(Input& input);
	// 吐き出し
	void AttackVomit(Input& input);
	// 空気弾
	void AttackAirBullet(Input& input);
	// 当たり判定の取得
	Rect GetColRect()const { return m_colRect; }
	// 当たり判定の矩形情報(マップチップ)
	Rect GetRect();
	// 当たり判定の矩形情報(マップチップ以外)
	Rect GetColRect();

	// 初期座標の設定
	void SetStartPos(float posX, float posY);	
	// カメラのポインタの設定
	void SetCamera(std::shared_ptr<Camera> pCamera) { m_pCamera = pCamera; }

	// 攻撃のポインタの設定
	// すいこみ
	void SetInhale(std::shared_ptr<Inhale> pInhale) { m_pInhale = pInhale; }
	// 吐き出し
	void SetVomit(std::shared_ptr<Vomit> pVomit) { m_pVomit = pVomit; }
	// 空気弾
	void SetAirBullet(std::shared_ptr<AirBullet>pAriBullet) { m_pAirBullet = pAriBullet; }

	// 残機を減らす
	void DeleteLives() { --m_remainingLives; }
	// プレイヤーの残機を全快する
	void CompleteRecoveryLives();
	// HPを全快する
	void CompleteRecoveryHp();
	// HPを回復する
	void RecoveryHp2();
	// 重力をリセットする
	void GravityReset() { m_move.y = 0; }
	// 物体を吸い込んだ時に状態をほおばり状態にする
	void OnGlup() { m_condition = kGulp; m_isGlup = true; }
	// ダメージを受けた
	void OnDamage();
	// ダメージ中かどうか
	bool IsDamege()const { return m_isDamage; }
	// ミスをした
	void OnMiss();
	// プレイヤーのミス演出用のフレーム数の取得
	int GetMissFrame()const { return static_cast<int>(m_missFrame); }
	/*どのようにミスをしたか*/
	// 落ちてミスをした
	void FellMiss() { m_isFellMiss = true; }
	// HPがゼロになってミスをした
	void DamageMiss() { m_isDamageMiss = true; }
	// クリア演出のダンス
	void OnDance();

	/*当たり判定の取得(マップチップ)*/
	float GetLeft()const { return m_pos.x - m_w * 0.5f; }
	float GetRight() const { return m_pos.x + m_w * 0.5f; }
	float GetTop() const { return m_pos.y - m_h; }
	float GetBottom()const { return m_pos.y; }
	/*当たり判定の取得(マップチップ以外)*/
	float GetColLect()const;
	float GetColRight()const;
	float GetColTop()const;
	float GetColBottom()const;
	// 横幅の取得
	int GetW()const { return m_w; }
	// 縦幅の取得
	int GetH()const { return m_h; }
	// 位置の取得
	float GetX()const { return m_pos.x; }
	float GetY()const { return m_pos.y; }
	// HPの取得
	int GetHp()const { return m_hp; }
	// 残機の取得
	int GetRemainingLives() const { return m_remainingLives; }
	// 向きの取得
	Dir GetDir()const { return m_dir; }
	Dir GetDirLeft()const { return kDirLeft; }
	Dir GetDirRight()const { return kDirRight; }
	// 花での移動中かどうか
	bool IsFlowerMove()const { return m_isFlowerMove; }

	bool IsMiss()const { return m_isMiss; }
	

private:
	// 座標
	Vec2 m_pos;
	// 移動値
	Vec2 m_move;
	// 当たり判定用の矩形
	Rect m_colRect;
	// 現在の状態
	Condition m_condition;
	// 向いている方向
	Dir m_dir;
	// スクロール用の位置
	float m_x;
	float m_y;
	// プレイヤーのサイズ
	int m_w;
	int m_h;

	/*立ち絵のハンドル*/
	// 歩き
	int m_walkHandle;
	// すいこみ
	int m_inhaleHandle;
	// ホバリング
	int m_hoveringHandle;
	// ほおばり
	int m_gulpHandle;
	// ジャンプ
	int m_jumpHandle;
	// ミス
	int m_missHandle;
	// クリア後のダンス1
	int m_danceHandle1;
	// クリア後のダンス2
	int m_danceHandle2;

	/*BGMのハンドル*/
	// クリア時
	int m_clearBgmHandle;

	/*SEのハンドル*/
	// すいこみ
	int m_inhaleSeHandle;
	// ジャンプ
	int m_jumpSeHandle;
	// 吐き出し、空気弾
	int m_attackSeHandle;
	// ダメージ
	int m_damageSeHandle;

	// 落下速度
	float m_fallSpeed;
	// ジャンプした回数(ホバリング用)
	int m_jumpCount;
	// HP
	int m_hp;
	// 残機
	int m_remainingLives;
	// 敵をほおばっているかどうか
	bool m_isGlup;
	// 敵を吐き出したかどうか
	bool m_isVomit;
	// 敵を吐き出してからどのくらい経ったか
	int m_vomitFrame;
	// アニメーション
	int m_animFrame;
	// ダメージを受けてからのフレーム数
	int m_damageFrame;
	// ダメージ中かどうか
	bool m_isDamage;
	// ミスをしたかどうか
	bool m_isMiss;
	// ミスをしてからのフレーム数
	float m_missFrame;
	// 落ちるときの回転
	float m_missRotate;

	/*落下のミスかダメージのミスか*/
	// 落ちたとき
	bool m_isFellMiss;
	// HPがゼロになったとき
	bool m_isDamageMiss;

	/*クリア演出*/
	// ダンス中かどうか
	bool m_isDance;
	// 花が移動しているかどうか
	bool m_isFlowerMove;
	// イベント中のフレーム数
	int m_eventFrame;
	// ダンスの回転度
	float m_danceRotate;
	// ダンスの回転パターン
	int m_rotatePattern;
	// ダンスのSinの縦幅
	float m_danceSinCount;
	// ダンスの縦のふり幅
	float m_danceAnimationPosY;
	// ダンスの振付
	DanceChoreography m_choreography;
	// ダンスのアニメーションのフレーム数
	int m_danceAnimFrame;

	/*ゴール後の移動イベント*/
	// 移動イベントの移動値X
	int m_moveEventX;
	// 移動イベントの移動値Y
	int m_moveEventY;
	// 移動イベントのフレーム数
	int m_moveEventFrame;	

	/*各ポインタの取得*/
	// 空気弾
	std::shared_ptr<AirBullet> m_pAirBullet;
	// すいこみ
	std::shared_ptr<Inhale> m_pInhale;
	// 吐き出し
	std::shared_ptr<Vomit> m_pVomit;
	// カメラ
	std::shared_ptr<Camera> m_pCamera;

};

