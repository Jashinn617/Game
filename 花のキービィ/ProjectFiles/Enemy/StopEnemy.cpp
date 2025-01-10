#include "StopEnemy.h"
#include "../Player.h"
#include "../Attack/Vomit.h"
#include "../Attack/AirBullet.h"
#include "../Attack/Inhale.h"
#include "../Camera.h"
#include "../Game.h"
#include "DxLib.h"

#include <cmath>

namespace
{
	// すいこまれる速度
	constexpr float kInhaleSpeed = 3.5f;

	// アニメーションスピード
	constexpr float kSinSpeed = 0.25f;
	// アニメーションの振幅
	constexpr float kAnimationSize = 6.0f;
}


StopEnemy::StopEnemy() :
	m_w(0),
	m_h(0),
	m_isExist(false),
	m_isInhale(false),
	m_x(0),
	m_y(0),
	m_sinCount(0),
	m_animationHeight(0),
	m_isDamage(false),
	m_damageFrame(0)
{
	// 位置の初期化
	m_pos = Vec2(0, 0);
	/*グラフィックのロード*/
	m_handle = LoadGraph(_T("data/Enemy/StopEnemy.png"));
	m_damageEffectHandle = LoadGraph(_T("data/Effect/Delete.png"));
	/*SEのロード*/
	m_hitSeHandle = LoadSoundMem(_T("data/Sound/SE/Hit.wav"));
	
}

StopEnemy::~StopEnemy()
{
	/*グラフィックのデリート*/
	DeleteGraph(m_handle);
	DeleteGraph(m_damageEffectHandle);
	/*SEのデリート*/
	DeleteSoundMem(m_hitSeHandle);
}

void StopEnemy::Init(float x, float y)
{
	// 初期位置の設定
	m_pos.x = x;
	m_pos.y = y;

	// サイズの設定
	GetGraphSize(m_handle, &m_w, &m_h);

	// リスタート用の初期化
	m_damageFrame = 0;
	m_isDamage = false;
	m_isExist = true;
}

void StopEnemy::Update()
{
	// 存在しないエネミーの処理はしない
	if (!m_isExist)return;
	
	// 当たり判定の更新
	GetRect();
	PlayerCollision();
	InhaleCollision();
	VomitCollision();
	AirBulletCollision();

	// アニメーション
	if (!m_isInhale)
	{
		m_sinCount += kSinSpeed;
		m_animationHeight = sinf(m_sinCount) * kAnimationSize;
	}
	
	// 消えるときの処理
	// 爆発エフェクトを表示して消える
	if (m_isDamage)
	{
		m_damageFrame++;

		if (m_damageFrame >= 30)
		{
			m_isExist = false;
		}
	}
}

void StopEnemy::Draw()
{
	// 存在しないエネミーは描画しない
	if (!m_isExist)return;

	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	// アニメーション
	float posY = y + m_animationHeight;

	
	if (m_isDamage)// 爆発エフェクト
	{
		DrawGraph(static_cast<int>(x), static_cast<int>(posY), m_damageEffectHandle, true);
	}
	else if (!m_isDamage)// 通常
	{
		DrawExtendGraph(static_cast<int>(x), static_cast<int>(posY), static_cast<int>(x) + m_w, static_cast<int>(y) + m_h, m_handle, true);
	}

#ifdef _DEBUG
	// 当たり判定の矩形
	DrawBox(static_cast<int>(GetLeft()), static_cast<int>(GetTop()), static_cast<int>(GetRight()), static_cast<int>(GetBottom()), 0xff0000, false);
#endif
}

Rect StopEnemy::GetRect()
{
	m_colRect.top = GetTop();
	m_colRect.left = GetLeft();
	m_colRect.bottom = GetBottom();
	m_colRect.right = GetRight();

	return m_colRect;
}

float StopEnemy::GetLeft() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	return x;
}

float StopEnemy::GetRight() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	return x + m_w;
}

float StopEnemy::GetTop() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	return y;
}

float StopEnemy::GetBottom() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	return y + m_h;
}

void StopEnemy::PlayerCollision()
{
	if (m_pPlayer->IsMiss())return;
	if (m_isDamage)return;
	// プレイヤーに当たった場合は、プレイヤーにダメージを与えたうえで自分も消える
	// プレイヤーに吸い込まれている状態だった場合はプレイヤーをほおばり状態にする
	if (m_colRect.isCol(m_pPlayer->GetColRect()))
	{
		if (m_isInhale)
		{
			m_pPlayer->OnGlup();
			m_isExist = false;
		}
		else
		{
			m_pPlayer->OnDamage();		
		}

		if (!m_pPlayer->IsDamege())
		{
			m_isDamage = true;
		}
	}

}

void StopEnemy::InhaleCollision()
{
	if (m_isDamage)return;
	if (m_colRect.isCol(m_pInhale->GetRect()) && m_pInhale->IsExsist())
	{
		// 敵の吸い込みの処理
		Vec2 E = m_pos;
		Vec2 P = Vec2(m_pPlayer->GetX(), m_pPlayer->GetY());
		Vec2 EP = P.operator-=(E);
		Vec2 Direction = EP.GetNormalize();
		Vec2 Velocity = Direction.operator*=(kInhaleSpeed);

		E.operator+=(Velocity);

		m_pos = E;
		m_isInhale = true;
	}
	else
	{
		m_isInhale = false;
	}
}

void StopEnemy::VomitCollision()
{
	if (!m_pVomit->IsExsist())return;
	if (m_isDamage)return;
	if (m_colRect.isCol(m_pVomit->GetRect()))
	{
		// SEを鳴らす
		PlaySoundMem(m_hitSeHandle, DX_PLAYTYPE_BACK, true);
		// 消える
		m_isDamage = true;
		// 弾を消す
		m_pVomit->FalseExist();
	}
}

void StopEnemy::AirBulletCollision()
{
	if (!m_pAirBullet->IsExsist())return;
	if (m_isDamage)return;
	if (m_colRect.isCol(m_pAirBullet->GetRect()))
	{
		// SEを鳴らす
		PlaySoundMem(m_hitSeHandle, DX_PLAYTYPE_BACK, true);
		// 消える
		m_isDamage = true;
		// 弾を消す
		m_pAirBullet->FalseExist();
	}
}
