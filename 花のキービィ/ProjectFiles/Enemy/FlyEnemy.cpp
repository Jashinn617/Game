#include "FlyEnemy.h"
#include "../Player.h"
#include "../Attack/Vomit.h"
#include "../Attack/AirBullet.h"
#include "../Attack/Inhale.h"
#include "../Camera.h"
#include "../Game.h"
#include "DxLib.h"

namespace
{
	// 吸い込まれる速度
	constexpr float kInhaleSpeed = 3.5f;

	// アニメーションスピード
	constexpr float kSinSpeed = 0.15f;
	// アニメーションの振幅
	constexpr float kAnimationSize = 70.0f;
}

FlyEnemy::FlyEnemy():
	m_w(0),
	m_h(0),
	m_handle(-1),
	m_onInhaleHandle(-1),
	m_dir(kDirLeft),
	m_isExist(false),
	m_isInhale(false),
	m_walkAnimFrame(0),
	m_walkFrame(0),
	m_speed(2),
	m_x(0),
	m_y(0),
	m_sinCount(0),
	m_animationHeight(0),
	m_walkCycle(0),
	m_isDamage(false),
	m_damageFrame(0)
{
	// 位置の初期化
	m_pos = Vec2(0, 0);
	/*グラフィックのロード*/
	m_handle = LoadGraph(_T("data/Enemy/FlyEnemy.png"));
	m_onInhaleHandle = LoadGraph(_T("data/Enemy/DamageFlyEnemy.png"));
	m_damageEffectHandle = LoadGraph(_T("data/Effect/Delete.png"));
	/*SEのロード*/
	m_hitSeHandle = LoadSoundMem(_T("data/Sound/SE/Hit.wav"));
}

FlyEnemy::~FlyEnemy()
{
	/*グラフィックのデリート*/
	DeleteGraph(m_handle);
	DeleteGraph(m_onInhaleHandle);
	DeleteGraph(m_damageEffectHandle);
	/*SEのデリート*/
	DeleteSoundMem(m_hitSeHandle);
}

void FlyEnemy::Init(float x, float y, int cycle)
{
	// 初期位置の設定
	m_pos.x = x;
	m_pos.y = y + 7;

	// 折り返しフレームの設定
	m_walkCycle = cycle;
	m_walkFrame = 0;
	if (m_dir == kDirRight)
	{
		m_speed *= -1;
		m_dir = kDirLeft;
	}

	// サイズ設定
	m_w = 64;
	m_h = 64;
	
	// リスタート用の初期化
	m_damageFrame = 0;
	m_isDamage = false;
	m_isExist = true;
}

void FlyEnemy::Update()
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
	m_sinCount += kSinSpeed;
	m_animationHeight = sinf(m_sinCount) * kAnimationSize;

	

	// 通常の処理
	if (!m_isInhale)// すいこまれていない時
	{
		if (!m_isDamage)// ダメージを受けていない
		{
			// 移動
			m_pos.x += m_speed;
			m_walkFrame++;

			// 折り返す時に反対にする
			if (m_walkFrame >= m_walkCycle)
			{
				m_speed *= -1;
				m_walkFrame = 0;
				if (m_dir == kDirLeft)
				{
					m_dir = kDirRight;
				}
				else if (m_dir == kDirRight)
				{
					m_dir = kDirLeft;
				}
			}
		}
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

void FlyEnemy::Draw()
{
	// 存在しないエネミーは描画しない
	if (!m_isExist)return;

	// 足元中央を左上座標に変換する
	int x = static_cast<int>(m_pos.x - m_w * 0.5f);
	int y = static_cast<int>(m_pos.y - m_h);
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	// アニメーション
	int posY = y + static_cast<int>(m_animationHeight);

	// 爆発エフェクト
	if (m_isDamage)
	{
		DrawGraph(x, posY, m_damageEffectHandle, true);
	}
	else if (!m_isDamage)
	{
		if (m_isInhale)// すいこまれ中
		{
			if (m_dir == kDirLeft)
			{
				DrawGraph(x, posY, m_onInhaleHandle, true);
			}
			else if (m_dir == kDirRight)
			{
				DrawTurnGraph(x, posY, m_onInhaleHandle, true);
			}
		}
		else if (!m_isInhale)// 通常
		{
			if (m_dir == kDirLeft)
			{
				DrawTurnGraph(x, posY, m_handle, true);		
			}
			else if (m_dir == kDirRight)
			{
				DrawGraph(x, posY, m_handle, true);
			}
		}
	}

#ifdef _DEBUG
	// 当たり判定の矩形
	DrawBox(static_cast<int>(GetLeft()), static_cast<int>(GetTop()), static_cast<int>(GetRight()), static_cast<int>(GetBottom()), 0xff0000, false);
#endif
}

Rect FlyEnemy::GetRect()
{
	m_colRect.top = GetTop();
	m_colRect.left = GetLeft();
	m_colRect.bottom = GetBottom();
	m_colRect.right = GetRight();

	return m_colRect;
}

float FlyEnemy::GetLeft() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// アニメーションに追従させる
	float posY = y + m_animationHeight;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	posY -= m_pCamera->GetScrollY();

	return x;
}

float FlyEnemy::GetRight() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// アニメーションに追従させる
	float posY = y + m_animationHeight;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	posY -= m_pCamera->GetScrollY();

	return x + m_w;
}

float FlyEnemy::GetTop() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// アニメーションに追従させる
	float posY = y + m_animationHeight;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	posY -= m_pCamera->GetScrollY();

	return posY + 10;
}

float FlyEnemy::GetBottom() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// アニメーションに追従させる
	float posY = y + m_animationHeight;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	posY -= m_pCamera->GetScrollY();

	return posY + static_cast<float>(m_h);
}

void FlyEnemy::PlayerCollision()
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

void FlyEnemy::InhaleCollision()
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

void FlyEnemy::VomitCollision()
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

void FlyEnemy::AirBulletCollision()
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
