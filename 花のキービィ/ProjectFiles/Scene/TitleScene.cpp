#include "TitleScene.h"
#include "DxLib.h"
#include "../util/Input.h"
#include "../Player.h"
#include "../Game.h"

#include <math.h>

namespace
{
	constexpr int kScreenWidht = Game::kScreenWidth;
	constexpr int kScreenHeight = Game::kScreenHeight;

	constexpr float kCursorSinSpeed = 0.1f;
	constexpr float kCursorAnimationSwing = 15.0f;

	// アニメーション
	constexpr int kUseFrame[] = { 0,1,2,0 };
	constexpr int kAnimFrameNum = 12;
	constexpr int kAnimFrameCycle = _countof(kUseFrame) * kAnimFrameNum;
	constexpr int kWidth = 68;
	constexpr int kHeight = 68;
	constexpr float kPlayerSinSpeed = 0.1f;
	constexpr float kPlayerAnimationSwing = 30;
}


TitleScene::TitleScene() :
	m_isSceneEnd(false),
	m_titleHandle(-1),
	m_endHandle(-1),
	m_startHandle(-1),
	m_cursorHandle(-1),
	m_backHandle(-1),
	m_animationPlayer(-1),
	m_titleW(0),
	m_titleH(0),
	m_startW(0),
	m_startH(0),
	m_endW(0),
	m_endH(0),
	m_titleX(0),
	m_titleY(0),
	m_startX(0),
	m_startY(0),
	m_endX(0),
	m_endY(0),
	m_cursorX(0),
	m_cursorY(0),
	m_playerX(0),
	m_playerY(0),
	m_startScale(1.0f),
	m_endScale(1.0f),
	m_scrollX(0),
	m_backW(0),
	m_backH(0),
	m_cursorCount(0),
	m_bgmHandle(-1),
	m_selectHandle(-1),
	m_startSeHandle(-1),
	m_cursorSinCount(0),
	m_cursorAnimationPosX(0),
	m_playerSinCount(0),
	m_playerAnimationPosY(0),
	m_fadeAlpha(0),
	m_animFrame(0)
{
}

TitleScene::~TitleScene()
{
	DeleteGraph(m_titleHandle);
	DeleteGraph(m_endHandle);
	DeleteGraph(m_startHandle);
	DeleteGraph(m_cursorHandle);
	DeleteGraph(m_backHandle);
}

void TitleScene::Init()
{
	m_titleHandle = LoadGraph(_T("data/Logo/TitleLogo.png"));
	m_endHandle = LoadGraph(_T("data/Logo/EndLogo.png"));
	m_startHandle = LoadGraph(_T("data/Logo/StartLogo.png"));
	m_cursorHandle = LoadGraph(_T("data/Logo/Cursor.png"));
	m_backHandle = LoadGraph(_T("data/Map/Stage_01Bg.png"));
	m_animationPlayer = LoadGraph(_T("data/Player/Hovering.png"));

	// BGMの読み込み
	m_bgmHandle = LoadSoundMem(_T("data/Sound/BGM/Title.ogg"));
	// BGMの音量設定
	ChangeVolumeSoundMem(150, m_bgmHandle);

	// SEの読み込み
	m_selectHandle = LoadSoundMem(_T("data/Sound/SE/Select.mp3"));
	m_startSeHandle = LoadSoundMem(_T("data/Sound/SE/GameStart.wav"));


	// BGMの再生
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP, true);

	GetGraphSize(m_titleHandle, &m_titleW, &m_titleH);
	GetGraphSize(m_startHandle, &m_startW, &m_startH);
	GetGraphSize(m_endHandle, &m_endW, &m_endH);

	m_isSceneEnd = false;

	m_titleX = kScreenWidht * 0.5f - (m_titleW * 0.5f);
	m_titleY = 25;
	m_startX = kScreenWidht * 0.5f + 50;
	m_startY = kScreenHeight * 0.5f + 70;
	m_endX = m_startX;
	m_endY = m_startY + 200;
	m_cursorX = m_startX - 500;
	m_cursorCount = 0;

	m_pPlayer->CompleteRecoveryLives();

}

void TitleScene::Update(Input& input)
{
	// スクロール
	m_scrollX += 2.0f;

	// カーソル移動
	if (input.IsTriggered("up"))
	{
		m_cursorCount--;
		PlaySoundMem(m_selectHandle, DX_PLAYTYPE_BACK, true);
	}
	if (input.IsTriggered("down"))
	{
		PlaySoundMem(m_selectHandle, DX_PLAYTYPE_BACK, true);
		m_cursorCount++;
	}

	if (m_cursorCount % 2 == 0)
	{
		m_cursorY = kScreenHeight * 0.5f + 55;
		m_startScale = 1.5f;
		m_endScale = 1.0f;

		if (input.IsTriggered("A") || input.IsTriggered("B"))
		{
			m_isSceneEnd = true;
			PlaySoundMem(m_startSeHandle, DX_PLAYTYPE_BACK, true);
		}
	}
	else if (m_cursorCount % 2 == 1 || m_cursorCount % 2 == -1)
	{
		m_cursorY = kScreenHeight * 0.5f + 255;
		m_startScale = 1.0f;
		m_endScale = 1.5f;
		if (input.IsTriggered("A") || input.IsTriggered("B"))
		{
			DxLib_End();
		}
	}

	// 場面転換
	if (m_isSceneEnd)
	{
		m_fadeAlpha += 8;
		if (m_fadeAlpha > 255)
		{
			m_fadeAlpha = 255;
		}
	}
	else
	{
		m_fadeAlpha -= 8;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
		}
	}

	// カーソルのアニメーション
	m_cursorSinCount += kCursorSinSpeed;

	m_cursorAnimationPosX = sinf(m_cursorSinCount) * kCursorAnimationSwing;

	// アニメーション
	m_animFrame++;
	if (m_animFrame >= kAnimFrameCycle)
	{
		m_animFrame = 0;
	}
	m_playerX += 3;
	if (m_playerX >= kScreenWidht + (kWidth + 0.5f))
	{
		m_playerX = 0 - (kWidth * 0.5f);
	}

	m_playerSinCount += kCursorSinSpeed;
	m_playerAnimationPosY = sinf(m_playerSinCount) * kPlayerAnimationSwing;
}

void TitleScene::Draw()
{
	// 背景の描画
	DrawBox(0, 0, kScreenWidht, kScreenHeight, 0x7fffd4, true);
	BackDraw();

	// プレイヤーの描画
	int animFrame = m_animFrame / kAnimFrameNum;
	int srcX = kUseFrame[animFrame] * kWidth;
	int srcY = 0;
	int playerY =static_cast<int>(m_playerY + m_playerAnimationPosY) + 1000;
	DrawRectRotaGraph(static_cast<int>(m_playerX), playerY, srcX, srcY, kWidth, kHeight, 1.5f, 0, m_animationPlayer, true);

	// ロゴの描画
	DrawGraph(static_cast<int>(m_titleX), static_cast<int>(m_titleY), m_titleHandle, true);
	DrawRotaGraph(static_cast<int>(m_startX), static_cast<int>(m_startY), m_startScale, 0.0, m_startHandle, true);
	DrawRotaGraph(static_cast<int>(m_endX), static_cast<int>(m_endY), m_endScale, 0.0, m_endHandle, true);

	// カーソルの描画
	int cursorX =static_cast<int>(m_cursorX + m_cursorAnimationPosX);
	DrawGraph(cursorX,static_cast<int>(m_cursorY), m_cursorHandle, true);

	// フェードの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);
	DrawBox(0, 0, kScreenWidht, kScreenHeight, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく


}

void TitleScene::BackDraw()
{
	GetGraphSize(m_backHandle, &m_backW, &m_backH);

	int scrollBack = static_cast<int>(m_scrollX) % m_backW;

	for (int index = 0; index < 3; ++index)
	{
		DrawGraph(-scrollBack + index * m_backW,
			kScreenHeight - m_backH,
			m_backHandle,
			true);
	}

}

void TitleScene::End()
{
	// BGMを止める
	StopSoundMem(m_bgmHandle);
	// BGMをメモリから削除する
	DeleteSoundMem(m_bgmHandle);
	// SEをメモリから削除する
	DeleteSoundMem(m_selectHandle);
	DeleteSoundMem(m_startSeHandle);
}

bool TitleScene::IsSceneEnd() const
{
	return m_isSceneEnd && (m_fadeAlpha >= 255);
}

