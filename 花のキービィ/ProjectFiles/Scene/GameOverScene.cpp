#include "GameOverScene.h"
#include "../Player.h"
#include "../Game.h"
#include "DxLib.h"

namespace
{
	constexpr int kScreenWidht = Game::kScreenWidth;
	constexpr int kScreenHeight = Game::kScreenHeight;
}

GameOverScene::GameOverScene() :
	m_gameOverBg(-1),
	m_gameEndLogo(-1),
	m_gameOverLogo(-1),
	m_continueLogo(-1),
	m_cursor(-1),
	m_gameOverChara(-1),
	m_bgmHandle(-1),
	m_gameEndSeHandle(-1),
	m_continueSeHandle(-1),
	m_endW(0),
	m_endH(0),
	m_continueW(0),
	m_continueH(0),
	m_gameOverW(0),
	m_gameOverH(0),
	m_gameOverX(0),
	m_gameOverY(0),
	m_continueX(0),
	m_continueY(0),
	m_endX(0),
	m_endY(0),
	m_cursorX(0),
	m_cursorY(0),
	m_continueScale(1.0f),
	m_endScale(1.0f),
	m_cursorCount(0),
	m_fadeAlpha(255),
	m_nextScene(kTitle),
	m_isSceneEnd(false)
{
	/*グラフィックロード*/
	m_gameOverBg = LoadGraph(_T("data/Map/GameOverBg.png"));
	m_gameEndLogo = LoadGraph(_T("data/Logo/GameEndLogo.png"));
	m_gameOverLogo = LoadGraph(_T("data/Logo/GameOverLogo.png"));
	m_continueLogo = LoadGraph(_T("data/Logo/ContinueLogo.png"));
	m_cursor = LoadGraph(_T("data/Logo/Cursor.png"));
	m_gameOverChara = LoadGraph(_T("data/Logo/GameOverCharacter.png"));
}

GameOverScene::~GameOverScene()
{
	/*グラフィックのデリート*/
	DeleteGraph(m_gameOverBg);
	DeleteGraph(m_gameEndLogo);
	DeleteGraph(m_gameOverLogo);
	DeleteGraph(m_continueLogo);
	DeleteGraph(m_cursor);
	DeleteGraph(m_gameOverChara);

}

void GameOverScene::Init()
{
	/*BGMのロード*/
	m_bgmHandle = LoadSoundMem(_T("data/Sound/BGM/GameOver.wav"));
	// BGMの音量設定
	ChangeVolumeSoundMem(200, m_bgmHandle);
	// SEの読み込み
	m_gameEndSeHandle = LoadSoundMem(_T("data/Sound/SE/GameEnd.wav"));
	m_continueSeHandle = LoadSoundMem(_T("data/Sound/SE/Continue.wav"));
	ChangeVolumeSoundMem(150, m_gameEndSeHandle);

	// BGMの再生
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP, true);

	GetGraphSize(m_continueLogo, &m_continueW, &m_continueH);
	GetGraphSize(m_gameEndLogo, &m_endW, &m_endH);
	GetGraphSize(m_gameOverLogo, &m_gameOverW, &m_gameOverH);

	m_isSceneEnd = false;

	m_continueX = kScreenWidht * 0.5f - 300;
	m_continueY = kScreenHeight * 0.5f - 50;
	m_endX = m_continueX;
	m_endY = m_continueY + 300;
	m_gameOverX = kScreenWidht * 0.5f - 650;
	m_gameOverY = 25;
	m_cursorX = m_continueX - 600;

	m_pPlayer->CompleteRecoveryLives();
}

void GameOverScene::Update(Input& input)
{
	// カーソル移動
	if (input.IsTriggered("up"))
	{
		m_cursorCount--;
	}
	if (input.IsTriggered("down"))
	{
		m_cursorCount++;
	}

	// コンテニュー
	if (m_cursorCount % 2 == 0)
	{
		m_cursorY = m_continueY - 30;
		m_continueScale = 1.3f;
		m_endScale = 1.0f;

		if (input.IsTriggered("A") || input.IsTriggered("B"))
		{
			StopSoundMem(m_bgmHandle);
			PlaySoundMem(m_continueSeHandle, DX_PLAYTYPE_BACK, true);
			m_isSceneEnd = true;
		}
	}
	// ゲームエンド
	else if (m_cursorCount % 2 == 1 || m_cursorCount % 2 == -1)
	{
		m_cursorY = m_endY - 30;
		m_continueScale = 1.0f;
		m_endScale = 1.3f;
		if (input.IsTriggered("A") || input.IsTriggered("B"))
		{
			// BGMを止める
			StopSoundMem(m_bgmHandle);
			PlaySoundMem(m_gameEndSeHandle, DX_PLAYTYPE_NORMAL, true);
			m_nextScene = kTitle;
			m_isSceneEnd = true;
		}
	}

	if (m_isSceneEnd)
	{
		m_fadeAlpha += 4;
		if (m_fadeAlpha > 255)
		{
			m_fadeAlpha = 255;
		}
	}
	else
	{
		m_fadeAlpha -= 4;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
		}
	}
}

void GameOverScene::Draw()
{
	// 背景の描画
	DrawGraph(0, 0, m_gameOverBg, true);
	DrawGraph(0, 0, m_gameOverChara, true);

	// ロゴの描画
	DrawGraph(static_cast<int>(m_gameOverX), static_cast<int>(m_gameOverY), m_gameOverLogo, true);
	DrawRotaGraph(static_cast<int>(m_continueX), static_cast<int>(m_continueY), m_continueScale, 0.0, m_continueLogo, true);
	DrawRotaGraph(static_cast<int>(m_endX), static_cast<int>(m_endY), m_endScale, 0.0, m_gameEndLogo, true);
	
	// カーソルの描画
	DrawGraph(static_cast<int>(m_cursorX), static_cast<int>(m_cursorY), m_cursor, true);

	// フェードの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);
	DrawBox(0, 0, kScreenWidht, kScreenHeight, GetColor(255, 255, 255), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}

void GameOverScene::End()
{
	// BGMを止める
	StopSoundMem(m_bgmHandle);
	// BGMをメモリから削除する
	DeleteSoundMem(m_bgmHandle);
	DeleteSoundMem(m_gameEndSeHandle);
	DeleteSoundMem(m_continueSeHandle);
}

bool GameOverScene::IsSceneEnd()
{
	return m_isSceneEnd && (m_fadeAlpha >= 255);
}