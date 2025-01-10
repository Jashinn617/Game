#include "UI.h"
#include "DxLib.h"
#include "Game.h"
#include "Player.h"

namespace
{

	constexpr int kAnimationFrameMax = 15;
	constexpr int kMaxHp = 6;
}

UI::UI() :
	m_UIX(0),
	m_UIY(0),
	m_playerHp(0),
	m_animationFrame(0),
	m_isUp(false),
	m_playerRemainingLives(0),
	m_fontHandle(-1),
	m_fontBackHandle(-1)
{
	// HPバーの画像をロードする
	m_UIHandle0 = LoadGraph("data/UI/UI0.png");
	m_UIHandle1 = LoadGraph("data/UI/UI1.png");
	m_UIHandle2 = LoadGraph("data/UI/UI2.png");
	// HPの画像をロードする
	m_hpHandle1 = LoadGraph("data/UI/HP1.png");
	m_hpHandle0 = LoadGraph("data/UI/HP.png");
	// UIのサイズを取得する
	GetGraphSize(m_UIHandle0, &m_w, &m_h);

	// 残機数表示用のフォント
	m_fontHandle = CreateFontToHandle("GN-キルゴUかなNA", 150, -1);
	m_fontBackHandle = CreateFontToHandle("GN-キルゴUかなNA", 160, -1);
}

UI::~UI()
{
	/*画像のデリート*/
	DeleteGraph(m_UIHandle0);
	DeleteGraph(m_UIHandle1);
	DeleteGraph(m_UIHandle2);
	DeleteGraph(m_hpHandle1);
	DeleteGraph(m_hpHandle0);
	/*フォントのデリート*/
	DeleteFontToHandle(m_fontHandle);
	DeleteFontToHandle(m_fontBackHandle);
}

void UI::Init()
{
	// プレイヤーのHPを取得する
	m_playerHp = m_pPlayer->GetHp();
	// プレイヤーの残機を取得する
	m_playerRemainingLives = m_pPlayer->GetRemainingLives();	
}

void UI::Draw()
{
	// アニメーション
	int UIY = Game::kScreenHeight - m_h - m_UIY;
	// UIの描画
	DrawGraph(m_UIX, Game::kScreenHeight - m_h, m_UIHandle2, true);
	DrawGraph(m_UIX, UIY, m_UIHandle1, true);
	DrawGraph(m_UIX, UIY, m_UIHandle0, true);

	// アニメーション
	int HpY = Game::kScreenHeight - m_h + 25 - m_UIY;
	for (int i = 0; i < 6; i++)
	{
		DrawGraph(450 + (i * 100), HpY, m_hpHandle1, true);
	}
	// HPの数だけ表示する
	for (int i = 0; i < m_playerHp; i++)
	{
		DrawGraph(450 + (i * 100),HpY, m_hpHandle0, true);
	}

	// アニメーション
	int SY = Game::kScreenHeight - m_h + 80 - m_UIY;
	// 残機の描画
	DrawFormatStringToHandle(1500, SY, 0xffff00, m_fontBackHandle, "%d", m_playerRemainingLives);
	DrawFormatStringToHandle(1505, SY, 0x000000, m_fontHandle, "%d", m_playerRemainingLives);
}

void UI::Update()
{
	// プレイヤーのHPを取得する
	m_playerHp = m_pPlayer->GetHp();
	// プレイヤーの残機を取得する
	m_playerRemainingLives = m_pPlayer->GetRemainingLives();

	// アニメーション
	m_animationFrame++;
	if (m_animationFrame >= kAnimationFrameMax)
	{
		if (!m_isUp)// 下がっている
		{
			m_isUp = true;
			m_UIY = 5;
			m_animationFrame = 0;
		}
		else if (m_isUp)// 上がっている
		{
			m_isUp = false;
			m_UIY = 0;
			m_animationFrame = 0;
		}
	}
}
