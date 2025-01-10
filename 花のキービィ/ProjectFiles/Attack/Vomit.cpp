#include "Vomit.h"
#include "DxLib.h"
#include "../Game.h"
#include "../Camera.h"
#include "../Player.h"

Vomit::Vomit() :
	m_bulletHandle(-1),
	m_w(0),
	m_h(0),
	m_speed(10),
	m_pos(0.0f, 0.0f),
	m_isExist(false)
{
	/*グラフィックのロード*/
	m_bulletHandle = LoadGraph("data/Object/MonochromeFlower.png");

	// グラフィックのサイズを得る
	GetGraphSize(m_bulletHandle, &m_w, &m_h);
}

Vomit::~Vomit()
{
	/*グラフィックのデリート*/
	DeleteGraph(m_bulletHandle);
}

void Vomit::Init()
{
	
}

void Vomit::Draw()
{
	// 存在しないと描画しない
	if (!m_isExist)return;

	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	if (m_pPlayer->GetDir() == m_pPlayer->GetDirLeft())
	{
		DrawTurnGraph(static_cast<int>(x), static_cast<int>(y), m_bulletHandle, true);
	}
	else if (m_pPlayer->GetDir() == m_pPlayer->GetDirRight())
	{
		DrawGraph(static_cast<int>(x), static_cast<int>(y), m_bulletHandle, true);
	}
#ifdef _DEBUG
	DrawBox(static_cast<int>(GetLeft()),static_cast<int>(GetTop()), static_cast<int>(GetRight()), static_cast<int>(GetBottom()), 0x00ff00, false);
#endif
}

void Vomit::Update()
{
	if (!m_isExist)return;

	m_pos.x -= m_speed;

	// 当たり判定の更新
	GetRect();

	 //足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	 //スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	 //画面外に出た場合は存在を消す
	if (x > Game::kScreenWidth)
	{
		m_isExist = false;
	}
	if (x < 0)
	{
		m_isExist = false;
	}
}

Rect Vomit::GetRect()
{
	m_colRect.top = GetTop();
	m_colRect.left = GetLeft();
	m_colRect.bottom = GetBottom();
	m_colRect.right = GetRight();

	return m_colRect;
}

float Vomit::GetLeft() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	return x;
}

float Vomit::GetTop() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	return y;
}

float Vomit::GetRight() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	return x + m_w;
}

float Vomit::GetBottom() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	return y + m_h;
}

void Vomit::PutBulletLeft()
{
	m_isExist = true;
	m_pos.x = m_pPlayer->GetLeft() - m_w;
	m_pos.y = m_pPlayer->GetBottom();
}

void Vomit::PutBulletRight()
{
	m_isExist = true;
	m_pos.x = m_pPlayer->GetLeft() + m_w;
	m_pos.y = m_pPlayer->GetBottom();
}