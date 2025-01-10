#include "Inhale.h"
#include "DxLib.h"
#include "../Player.h"
#include "../Camera.h"
#include "../Player.h"

namespace
{
	// アニメーション
	constexpr int kUseFrame[] = { 0,1,2,3,4,5,6,7,8,9,10,11,0 };
	// 1コマのフレーム数
	constexpr int kAnimFrameNum = 3;
	// １サイクルのフレーム数
	constexpr int kAnimFrameCycle = _countof(kUseFrame) * kAnimFrameNum;

	// グラフィックサイズ
	constexpr int kWidth = 130;
	constexpr int kHeight = 95;

}

Inhale::Inhale() :
	m_pos(0.0f, 0.0f),
	m_w(kWidth),
	m_h(kHeight),
	m_isExist(false),
	m_animFrame(0)
{
	/*グラフィックのロード*/
	m_inhaleHandle = LoadGraph("data/Player/AInhale.png");	
}

Inhale::~Inhale()
{
	/*グラフィックのデリート*/
	DeleteGraph(m_inhaleHandle);
}

void Inhale::Init()
{

}

void Inhale::Draw()
{
	// 存在しないと描画しない
	if (!m_isExist)return;

	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	// アニメーション
	int animFrame = m_animFrame / kAnimFrameNum;
	int srcX = kUseFrame[animFrame] * kWidth;
	int srcY = kHeight * m_pPlayer->GetDir();


	DrawRectGraph(static_cast<int>(x),static_cast<int>(y), srcX, srcY, kWidth, kHeight, m_inhaleHandle, true);

#ifdef _DEBUG
	DrawBox(static_cast<int>(GetLeft()),static_cast<int>(GetTop()),static_cast<int>(GetRight()),static_cast<int>(GetBottom()), 0x00ff00, false);
#endif
}

void Inhale::Update()
{
	if (!m_isExist)return;

	// 当たり判定の更新
	GetRect();

	// アニメーション
	if (m_isExist)
	{
		m_animFrame++;
		if (m_animFrame >= kAnimFrameCycle)
		{
			m_animFrame = 0;
		}
	}
}

Rect Inhale::GetRect()
{
	m_colRect.top = GetTop();
	m_colRect.left = GetLeft();
	m_colRect.bottom = GetBottom();
	m_colRect.right = GetRight();

	return m_colRect;
}

float Inhale::GetLeft() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	return x;
}

float Inhale::GetTop() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	return y;
}

float Inhale::GetRight() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	return x + m_w;
}

float Inhale::GetBottom() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	return y + m_h;
}

void Inhale::PutBulletLeft()
{
	m_isExist = true;
	m_pos.x = m_pPlayer->GetLeft() - (m_w * 0.5f);
	m_pos.y = m_pPlayer->GetBottom();
}

void Inhale::PutBulletRight()
{
	m_isExist = true;
	m_pos.x = m_pPlayer->GetLeft() + m_w;
	m_pos.y = m_pPlayer->GetBottom();
}

void Inhale::FalseExist()
{
	m_isExist = false;
	m_animFrame = 0;
}