#include "SceneTitle.h"
#include "MainScene.h"
#include "DxLib.h"

SceneTitle::SceneTitle():
	m_titleLogoHandle(-1),
	m_startLogoHandle(-1),
	m_endLogoHandle(-1),
	m_selectCursorHandle(-1),
	m_bgHandle(-1),
	m_bgmHandle(-1),
	m_cursorMoveSeHandle(-1),
	m_startSeHandle(-1),
	m_startExtRate(kSmallStartExtRate),
	m_endExtRate(kSmallEndExtRate),
	m_cursorSinPosX(0),
	m_cursorPosY(kStartPosY),
	m_cursorCount(0),
	m_cursorSinCount(0),
	m_fadeAlpha(0),
	m_isSceneEnd(false)
{
	/*画像のロード*/
	m_titleLogoHandle = LoadGraph("Data/Image/Logo/Title.png");
	m_startLogoHandle = LoadGraph("Data/Image/Logo/Start.png");
	m_endLogoHandle = LoadGraph("Data/Image/Logo/End.png");
	m_selectCursorHandle = LoadGraph("Data/Image/SelectCursor.png");
	m_bgHandle = LoadGraph("Data/Image/Background/TitleBg.jpg");
	/*BGM、SEのロード*/
	m_bgmHandle = LoadSoundMem("Data/Sound/BGM/Title.ogg");
	m_cursorMoveSeHandle = LoadSoundMem("Data/Sound/SE/CursorMove.mp3");
	m_startSeHandle = LoadSoundMem("Data/Sound/SE/Start.mp3");
}

SceneTitle::~SceneTitle()
{
	/*画像のデリート*/
	DeleteGraph(m_titleLogoHandle);
	DeleteGraph(m_startLogoHandle);
	DeleteGraph(m_endLogoHandle);
	DeleteGraph(m_selectCursorHandle);
	DeleteGraph(m_bgHandle);
	/*BGM、SEのデリート*/
	DeleteSoundMem(m_bgmHandle);
	DeleteSoundMem(m_cursorMoveSeHandle);
	DeleteSoundMem(m_startSeHandle);
}

void SceneTitle::Init()
{
	/*BGM、SEのボリューム調整*/
	ChangeVolumeSoundMem(kBgmVolume, m_bgmHandle);
	ChangeVolumeSoundMem(kSeVolume, m_cursorMoveSeHandle);
	ChangeVolumeSoundMem(kSeVolume, m_startSeHandle);
	// BGMの再生
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP);
}

shared_ptr<SceneBase> SceneTitle::Update(Input& input)
{
	// 場面転換
	if (m_isSceneEnd)
	{
		// フェードイン
		m_fadeAlpha += kFadeSpeed;

		// 完全にフェードをしきってから場面転換をする
		if (m_fadeAlpha > 255)
		{
			m_fadeAlpha = 255;
			return make_shared<MainScene>();
		}
	}
	else
	{
		// フェードアウト
		m_fadeAlpha -= kFadeSpeed;

		// カーソルの移動と処理
		CursorMove(input);

		// カーソルのアニメーション
		m_cursorSinCount += kCursorSinSpeed;
		m_cursorSinPosX = sinf(m_cursorSinCount) * kCursorAnimationSwing;

		// Aを押すとSEを鳴らしてメインシーンに遷移するかゲームを終了する
		if (input.IsTriggered("A"))
		{
			if (m_cursorCount % 2 == 0)
			{
				// BGMの停止
				StopSoundMem(m_bgmHandle);
				PlaySoundMem(m_startSeHandle, DX_PLAYTYPE_BACK);
				m_isSceneEnd = true;
			}
			else if (m_cursorCount % 2 == 1)
			{
				DxLib_End();
			}
		}

		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
		}
	}
	
	return shared_from_this();
}

void SceneTitle::Draw()
{
	// 背景
	DrawGraph(0, 0, m_bgHandle, true);
	// ロゴ
	DrawRotaGraphF(kTitlePosX, kTitlePosY, kTitleExtRate, 0.0, m_titleLogoHandle, true, false);
	DrawRotaGraphF(kStartPosX, kStartPosY, m_startExtRate, 0.0, m_startLogoHandle, true, false);
	DrawRotaGraphF(kEndPosX, kEndPosY, m_endExtRate, 0.0, m_endLogoHandle, true, false);
	// カーソル
	float curosrX = kCursorPosX + m_cursorSinPosX;
	DrawRotaGraphF(curosrX, m_cursorPosY, kCursorExtRate, 0.0, m_selectCursorHandle, true, false);

#ifdef _DEBUG
	DrawString(0, 0, "Title", 0xffffff);
	DrawFormatString(0, 50, 0xffffff, "Count：%d", m_cursorCount % 2);
#endif	

	// フェードの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);
	DrawBox(0, 0, kScreenWidth, kScreenHeight, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}

void SceneTitle::End()
{
	/*処理無し*/
}

void SceneTitle::CursorMove(Input& input)
{
	// カーソルを動かす際の処理
	if (input.IsTriggered("up"))
	{
		m_cursorCount--;
		PlaySoundMem(m_cursorMoveSeHandle, DX_PLAYTYPE_BACK);
	}
	else if (input.IsTriggered("down"))
	{
		m_cursorCount++;
		PlaySoundMem(m_cursorMoveSeHandle, DX_PLAYTYPE_BACK);
	}
	// カーソルの位置によってロゴの拡大率や、次のシーンが変わる
	if (m_cursorCount % 2 == 0)
	{
		m_cursorPosY = kStartPosY;
		m_startExtRate = kBigStartExtRate;
		m_endExtRate = kSmallEndExtRate;
	}
	else if (m_cursorCount % 2 == 1)
	{
		m_cursorPosY = kEndPosY;
		m_startExtRate = kSmallStartExtRate;
		m_endExtRate = kBigEndExtRate;
	}
}
