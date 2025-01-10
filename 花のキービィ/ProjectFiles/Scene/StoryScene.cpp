#include "StoryScene.h"
#include"DxLib.h"
#include "../Game.h"
#include "../util/Input.h"

namespace
{
	// ボタンの大きさ
	constexpr int kButtonWidth = 26;
	constexpr int kButtonHeight = 29;

	// 画像切り替えのフレーム数
	constexpr int kStoryFrame = 300;

	// フェード時の速さ
	constexpr int kMissFadeSpeed = 12;
}

StoryScene::StoryScene() :
	m_bgmHandle(-1),
	m_skipHandle(-1),
	m_buttonHandle(-1),
	m_storyFrame(0),
	m_storyCount(0),
	m_fadeAlpha(255),
	m_isSceneEnd(false)
{
	/*グラフィックのロード*/
	m_storyHandle[0] = LoadGraph(_T("data/Story/00.png"));
	m_storyHandle[1] = LoadGraph(_T("data/Story/01.png"));
	m_storyHandle[2] = LoadGraph(_T("data/Story/02.png"));
	m_storyHandle[3] = LoadGraph(_T("data/Story/03.png"));
	m_storyHandle[4] = LoadGraph(_T("data/Story/04.png"));
	m_storyHandle[5] = LoadGraph(_T("data/Story/05.png"));
	m_storyHandle[6] = LoadGraph(_T("data/Story/06.png"));
	m_storyHandle[7] = LoadGraph(_T("data/Story/07.png"));
	m_skipHandle = LoadGraph(_T("data/Story/skip.png"));
	m_buttonHandle = LoadGraph(_T("data/Object/Button.png"));
}

StoryScene::~StoryScene()
{
	/*グラフィックのデリート*/
	for (int i = 0; i < 8; i++)
	{
		DeleteGraph(m_storyHandle[i]);
	}
	DeleteGraph(m_skipHandle);
	DeleteGraph(m_buttonHandle);
}

void StoryScene::Init()
{
	/*BGMのロード*/
	m_bgmHandle = LoadSoundMem(_T("data/Sound/BGM/Story.mp3"));
	/*BGMの再生*/
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP, true);
	// シーンが終了しないようにする
	m_isSceneEnd = false;
	m_storyFrame = 0;
}

void StoryScene::Update(Input& input)
{
	if (!m_isSceneEnd)
	{
		m_storyFrame++;

		// Aボタンを押したらスキップができる
		if (input.IsTriggered("A"))
		{
			m_isSceneEnd = true;
		}

		if (m_storyFrame >= kStoryFrame)
		{
			m_storyCount++;
			m_storyFrame = 0;
		}

		// ストーリーが終わるとシーンが終了する
		if (m_storyCount>=_countof(m_storyHandle))
		{
			m_isSceneEnd = true;
		}

		// フェードアウト
		m_fadeAlpha -= kMissFadeSpeed;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
		}
	}
	// 場面転換のフェード
	else // フェードイン
	{
		m_fadeAlpha += kMissFadeSpeed;
		if (m_fadeAlpha > 255)
		{
			m_fadeAlpha = 255;
		}
	}
}

void StoryScene::Draw()
{
	// ストーリー画像の表示
	DrawGraph(0, 0, m_storyHandle[m_storyCount], true);
	// スキップ画像の表示
	DrawGraph(0, 0, m_skipHandle, true);
	DrawRectRotaGraph(1850, 30,
		0, 0, 
		kButtonWidth, kButtonHeight,
		2.0, 0.0, m_buttonHandle, true);

	// フェードの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}

void StoryScene::End()
{
	/*BGMを止める*/
	StopSoundMem(m_bgmHandle);
	/*BGMの再生*/
	DeleteSoundMem(m_bgmHandle);
}

bool StoryScene::IsSceneEnd()
{
	return m_isSceneEnd && (m_fadeAlpha >= 255);
}
