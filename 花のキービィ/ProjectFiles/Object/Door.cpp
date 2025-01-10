#include "Door.h"
#include "../Player.h"
#include "../Camera.h"
#include "../Game.h"
#include "DxLib.h"

#include <cassert>

Door::Door():
	m_w(0),
	m_h(0),
	m_handle(-1),
	m_x(0),
	m_y(0),
	m_decorationAngle(0),
	m_isColition(false)
{
	// 座標の初期化
	m_pos = Vec2(0, 0);

	/*グラフィックのロード*/
	m_handle = LoadGraph(_T("data/Object/Door.png"));
	m_decorationHandle = LoadGraph(_T("data/Object/ColorFlower.png"));
}

Door::~Door()
{
	/*グラフィックのデリート*/
	DeleteGraph(m_handle);
	DeleteGraph(m_decorationHandle);
}

void Door::Init(float x, float y)
{
	// 初期位置の設定
	m_pos.x = x;
	m_pos.y = y;

	// サイズの設定
	GetGraphSize(m_handle, &m_w, &m_h);
}

void Door::Update()
{
	// 当たり判定の更新
	GetRect();
	PlayerCollision();

	// 花を回す
	m_decorationAngle += 0.05f;
}

void Door::Draw()
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	// ドア本体の描画
	DrawGraph(static_cast<int>(x),static_cast<int>(y), m_handle, true);
	// 花の描画
	DrawRotaGraph(static_cast<int>(x + (m_w * 0.5f)), static_cast<int>(y + (m_h * 0.5f)) - 64, 1.2, m_decorationAngle, m_decorationHandle, true);

#ifdef _DEBUG
	// 当たり判定の矩形
	DrawBox(static_cast<int>(GetLeft()), static_cast<int>(GetTop()), static_cast<int>(GetRight()), static_cast<int>(GetBottom()), 0xff0000, false);
#endif
}

Rect Door::GetRect()
{
	m_colRect.top = GetTop();
	m_colRect.left = GetLeft();
	m_colRect.bottom = GetBottom();
	m_colRect.right = GetRight();

	return m_colRect;
}

float Door::GetLeft() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	return x;
}

float Door::GetRight() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	return x + m_w;
}

float Door::GetTop() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	return y;
}

float Door::GetBottom() const
{
	// 足元中央を左上座標に変換する
	float x = m_pos.x - m_w * 0.5f;
	float y = m_pos.y - m_h;
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	return y + m_h;
}

void Door::PlayerCollision()
{
	if (m_colRect.isCol(m_pPlayer->GetColRect()))
	{
		m_isColition = true;
	}
	else
	{
		m_isColition = false;
	}
}
