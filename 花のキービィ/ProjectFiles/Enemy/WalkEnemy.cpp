#include "WalkEnemy.h"
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
	// キャラクターのアニメーション
	constexpr int kUseFrame[] = { 0,1,2, 1};
	// アニメーション1コマのフレーム数
	constexpr int kAnimFrameNum = 12;
	// アニメーション１サイクルのフレーム数
	constexpr int kAnimFrameCycle = _countof(kUseFrame) * kAnimFrameNum;
}

WalkEnemy::WalkEnemy() :
	m_w(0),
	m_h(0),
	m_handle(-1),
	m_damageHandle(-1),
	m_dir(kDirLeft),
	m_isExist(false),
	m_isInhale(false),
	m_walkAnimFrame(0),
	m_walkFrame(0),
	m_speed(2),
	m_x(0),
	m_y(0),
	m_walkCycle(0),
	m_isDamage(false),
	m_damageFrame(0)
{
	// 位置の初期化
	m_pos = Vec2(0, 0);
	/*グラフィックのロード*/
	m_handle = LoadGraph(_T("data/Enemy/WalkEnemy.png"));
	m_damageHandle = LoadGraph(_T("data/Enemy/DamageEnemy.png"));
	m_damageEffectHandle = LoadGraph(_T("data/Effect/Delete.png"));
	/*SEのロード*/
	m_hitSeHandle = LoadSoundMem(_T("data/Sound/SE/Hit.wav"));
}

WalkEnemy::~WalkEnemy()
{
	/*グラフィックのデリート*/
	DeleteGraph(m_handle);
	DeleteGraph(m_damageHandle);
	DeleteGraph(m_damageEffectHandle);
	/*SEのデリート*/
	DeleteSoundMem(m_hitSeHandle);
}

void WalkEnemy::Init(float x, float y, int cycle)
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
	m_w = 68;
	m_h = 72;

	// リスタート用の初期化
	m_damageFrame = 0;
	m_isDamage = false;
	m_isExist = true;
}

void WalkEnemy::Update()
{
	// 存在しないエネミーの処理はしない
	if (!m_isExist)return;

	// 当たり判定の更新
	GetRect();
	PlayerCollision();
	InhaleCollision();
	VomitCollision();
	AirBulletCollision();


	if (!m_isInhale)// すいこまれていないとき
	{
		// 歩きのアニメーション
		m_walkAnimFrame++;
		if (m_walkAnimFrame >= kAnimFrameCycle)
		{
			m_walkAnimFrame = 0;
		}

		if (!m_isDamage)// ダメージを受けていない
		{
			// 移動
			m_pos.x += m_speed;
			m_walkFrame++;
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
	if (m_isDamage)
	{
		m_damageFrame++;

		if (m_damageFrame >= 30)
		{
			m_isExist = false;
		}

	}


}

void WalkEnemy::Draw()
{
	// 存在しないエネミーは描画しない
	if (!m_isExist)return;

	// 足元中央を左上座標に変換する
	int x = static_cast<int>(m_pos.x - m_w * 0.5f);
	int y = static_cast<int>(m_pos.y - m_h);

	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	// アニメーション
	int animFrame = m_walkAnimFrame / kAnimFrameNum;
	int srcX = kUseFrame[animFrame] * m_w;
	int srcY = m_h * m_dir;	

	
	if (m_isDamage)// ダメージを受けた時
	{
		DrawGraph(x, y, m_damageEffectHandle, true);
	}
	else if (!m_isDamage)
	{
		if (m_isInhale)// 吸い込まれているとき
		{
			if (m_dir == kDirLeft)
			{
				DrawGraph(x, y, m_damageHandle, true);
			}
			else if (m_dir == kDirRight)
			{
				DrawTurnGraph(x, y, m_damageHandle, true);
			}
		}
		else if (!m_isInhale)// 通常時
		{
			DrawRectGraph(x, y, srcX, srcY, m_w, m_h, m_handle, true);
		}
	}

#ifdef _DEBUG
	DrawBox(static_cast<int>(GetLeft()), static_cast<int>(GetTop()), static_cast<int>(GetRight()), static_cast<int>(GetBottom()), 0xff0000, false);
#endif
}

Rect WalkEnemy::GetRect()
{
	m_colRect.top = GetTop();
	m_colRect.left = GetLeft();
	m_colRect.bottom = GetBottom();
	m_colRect.right = GetRight();

	return m_colRect;
}

float WalkEnemy::GetLeft() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();

	return x;
}

float WalkEnemy::GetRight() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();

	return x + m_w;
}

float WalkEnemy::GetTop() const
{
	// 足元中央を左上座標に変換する
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	y -= m_pCamera->GetScrollY();

	return y;
}

float WalkEnemy::GetBottom() const
{
	// 足元中央を左上座標に変換する
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	y -= m_pCamera->GetScrollY();

	return y + m_h;
}

void WalkEnemy::PlayerCollision()
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

void WalkEnemy::InhaleCollision()
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

void WalkEnemy::VomitCollision()
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

void WalkEnemy::AirBulletCollision()
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
