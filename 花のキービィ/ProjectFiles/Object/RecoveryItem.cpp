#include "RecoveryItem.h"
#include "../Player.h"
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


RecoveryItem::RecoveryItem():
	m_w(0),
	m_h(0),
	m_isExist(false),
	m_isInhale(false),
	m_x(0),
	m_y(0),
	m_sinCount(0),
	m_animationHeight(0)
{
	// 位置の初期化
	m_pos = Vec2(0, 0);
	/*グラフィックのロード*/
	m_handle = LoadGraph(_T("data/Object/RecoveryItem.png"));
	/*SEのロード*/
	m_hitSeHandle = LoadSoundMem(_T("data/Sound/SE/Recovery.wav"));
}

RecoveryItem::~RecoveryItem()
{
	/*グラフィックのデリート*/
	DeleteGraph(m_handle);
	/*SEのデリート*/
	DeleteSoundMem(m_hitSeHandle);
}

void RecoveryItem::Init(float x, float y)
{
	// 初期位置の設定
	m_pos.x = x;
	m_pos.y = y;

	// サイズの設定
	GetGraphSize(m_handle, &m_w, &m_h);

	// リスタート用の初期化
	m_isExist = true;
}

void RecoveryItem::Update()
{
	// 存在しないエネミーの処理はしない
	if (!m_isExist)return;

	// 当たり判定の更新
	GetRect();
	PlayerCollision();
	InhaleCollision();

	// アニメーション
	m_sinCount += kSinSpeed;
	m_animationHeight = sinf(m_sinCount) * kAnimationSize;	
}

void RecoveryItem::Draw()
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

	DrawExtendGraph(static_cast<int>(x), static_cast<int>(posY), static_cast<int>(x) + m_w, static_cast<int>(y) + m_h, m_handle, true);

#ifdef _DEBUG
	// 当たり判定の矩形
	DrawBox(static_cast<int>(GetLeft()), static_cast<int>(GetTop()), static_cast<int>(GetRight()), static_cast<int>(GetBottom()), 0xff0000, false);
#endif
}

Rect RecoveryItem::GetRect()
{
	m_colRect.top = GetTop();
	m_colRect.left = GetLeft();
	m_colRect.bottom = GetBottom();
	m_colRect.right = GetRight();

	return m_colRect;
}

float RecoveryItem::GetLeft() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	return x;
}

float RecoveryItem::GetRight() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	return x + m_w;
}

float RecoveryItem::GetTop() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	return y;
}

float RecoveryItem::GetBottom() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	return y + m_h;
}

void RecoveryItem::PlayerCollision()
{
	// プレイヤーに当たった場合は、プレイヤーを回復する
	if (m_colRect.isCol(m_pPlayer->GetColRect()))
	{
		PlaySoundMem(m_hitSeHandle, DX_PLAYTYPE_BACK, true);
		m_pPlayer->RecoveryHp2();
		m_isExist = false;
	}

}

void RecoveryItem::InhaleCollision()
{
	if (!m_pInhale->IsExsist())return;
	if (m_colRect.isCol(m_pInhale->GetRect()))
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
