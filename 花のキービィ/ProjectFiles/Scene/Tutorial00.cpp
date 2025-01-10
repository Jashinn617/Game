
#include "Tutorial00.h"
#include "DxLib.h"
#include "../Game.h"
#include "../util/Input.h"

namespace
{
	constexpr int kLogoSpeed = 30;

	constexpr int kScreenWidth = Game::kScreenWidth;
	constexpr int kScreenHeight = Game::kScreenHeight;
}

Tutorial00::Tutorial00() :
	m_cursorCount(0),
	m_cursorX(0),
	m_cursorY(0),
	m_cursorMove(0),
	m_logoY(0),
	m_selectSeHandle(-1),
	m_startSeHandle(-1),
	m_landingLogoSeHandle(-1),
	m_fadeAlpha(255),
	m_isOperation(false),
	m_isSceneEnd(false),
	m_isRingSe(false),
	m_nextScene(kTutorial1)
{
	m_logo01 = LoadGraph(_T("data/Logo/tutorialLogo0.png"));
	m_logo02 = LoadGraph(_T("data/Logo/tutorialLogo1.png"));
	m_logo03 = LoadGraph(_T("data/Logo/tutorialLogo2.png"));
	m_cursor = LoadGraph(_T("data/Logo/Cursor.png"));
	
}

Tutorial00::~Tutorial00()
{
	DeleteGraph(m_logo01);
	DeleteGraph(m_logo02);
	DeleteGraph(m_logo03);
	DeleteGraph(m_cursor);
}

void Tutorial00::Init()
{

	// SEの読み込み
	m_selectSeHandle = LoadSoundMem(_T("data/Sound/SE/Select.mp3"));
	m_startSeHandle = LoadSoundMem(_T("data/Sound/SE/GameStart.wav"));
	m_landingLogoSeHandle = LoadSoundMem(_T("data/Sound/SE/LandingLogo.wav"));

	// カーソルの位置
	m_cursorX = 350;
	m_cursorY = 825;
	m_cursorCount = 0;

	// ロゴの初期位置
	m_logoY = -kScreenHeight;

	m_isSceneEnd = false;
	m_isOperation = false;
	m_isRingSe = false;
}

void Tutorial00::Update(Input& input)
{

	if (m_isOperation)
	{
		// カーソル移動
		if (input.IsTriggered("right"))
		{
			m_cursorCount--;

			PlaySoundMem(m_selectSeHandle, DX_PLAYTYPE_BACK, true);
		}
		if (input.IsTriggered("left"))
		{
			m_cursorCount++;
			PlaySoundMem(m_selectSeHandle, DX_PLAYTYPE_BACK, true);
		}

		if (m_cursorCount % 2 == 0)
		{
			m_cursorMove = 0;
			if (input.IsTriggered("A") || input.IsTriggered("B"))
			{
				m_isSceneEnd = true;
				m_nextScene = kTutorial1;

				PlaySoundMem(m_startSeHandle, DX_PLAYTYPE_BACK, true);
			}
		}
		else if (m_cursorCount % 2 == 1 || m_cursorCount % 2 == -1)
		{
			m_cursorMove = 700;
			if (input.IsTriggered("A") || input.IsTriggered("B"))
			{
				m_isSceneEnd = true;
				m_nextScene = kStage01;

				PlaySoundMem(m_startSeHandle, DX_PLAYTYPE_BACK, true);
			}
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

	m_logoY += kLogoSpeed;

	if (m_logoY >= -50)
	{
		if (!m_isRingSe)
		{
			PlaySoundMem(m_landingLogoSeHandle, DX_PLAYTYPE_BACK, true);
			m_isRingSe = true;
		}
	}

	if (m_logoY >= 0)
	{
		m_logoY = 0;
		m_isOperation = true;
	}

}

void Tutorial00::Draw()
{
	DrawGraph(0, m_logoY, m_logo01, true);
	DrawGraph(0, m_logoY, m_logo02, true);
	DrawGraph(0, m_logoY, m_logo03, true);


	if (m_isOperation)
	{
		DrawGraph(m_cursorX + m_cursorMove, m_cursorY, m_cursor, true);
	}

	

	// フェードの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);
	DrawBox(0, 0, kScreenWidth, kScreenHeight, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}

void Tutorial00::End()
{
	// SEをメモリから削除する
	DeleteSoundMem(m_selectSeHandle);
	DeleteSoundMem(m_startSeHandle);
	DeleteSoundMem(m_landingLogoSeHandle);
}

bool Tutorial00::IsSceneEnd() const
{
	return m_isSceneEnd && (m_fadeAlpha >= 255);
}
