#include "ClearScene.h"
#include "DxLib.h"
#include "../Game.h"
#include "../util/Input.h"

namespace
{
	constexpr float kForegroundScale = 2.0f;
	constexpr int kScreenWidth = Game::kScreenWidth;
	constexpr int kScreenHeight = Game::kScreenHeight;
}

ClearScene::ClearScene() :
	m_bgHandle(-1),
	m_fgHandle(-1),
	m_objHandle0(-1),
	m_objHandle1(-1),
	m_objHandle2(-1),
	m_objHandle3(-1),
	m_bgmHandle(-1),
	m_scrollX(0),
	m_fgW(0),
	m_fgH(0),
	m_isSceneEnd(false)
{
	/*グラフィックのロード*/
	m_bgHandle = LoadGraph(_T("data/Map/ClearBg.png"));
	m_fgHandle = LoadGraph(_T("data/Map/ClearFg0.png"));
	m_objHandle0 = LoadGraph(_T("data/Object/ClearObject0.png"));
	m_objHandle1 = LoadGraph(_T("data/Object/ClearObject1.png"));
	m_objHandle2 = LoadGraph(_T("data/Object/ClearObject2.png"));
	m_objHandle3 = LoadGraph(_T("data/Object/ClearObject3.png"));

	// 前景のサイズを取得する
	GetGraphSize(m_fgHandle, &m_fgW, &m_fgH);
}

ClearScene::~ClearScene()
{
	/*グラフィックのデリート*/
	DeleteGraph(m_bgHandle);
	DeleteGraph(m_fgHandle);
	DeleteGraph(m_objHandle0);
	DeleteGraph(m_objHandle1);
	DeleteGraph(m_objHandle2);
	DeleteGraph(m_objHandle3);
}

void ClearScene::Init()
{
	// シーンが終了しないようにする
	m_isSceneEnd = false;
	// BGMのロード
	m_bgmHandle = LoadSoundMem(_T("data/Sound/BGM/Clear.ogg"));
	// BGMの音量設定
	ChangeVolumeSoundMem(200, m_bgmHandle);
	// BGMの再生
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP, true);
}

void ClearScene::Update(Input& input)
{
	// 前景のスクロール
	m_scrollX += 2.0f;

	// Aボタンでタイトルに戻る
	if (input.IsTriggered("A"))
	{
		m_isSceneEnd = true;
	}
}

void ClearScene::Draw()
{
	// 背景の描画
	DrawGraph(0, 0, m_bgHandle, true);
	// 前景の描画
	ForeDraw();
	// オブジェクトの描画
	DrawGraph(0, 0, m_objHandle0, true);
	DrawGraph(0, 0, m_objHandle1, true);
	DrawGraph(0, 0, m_objHandle2, true);
	DrawGraph(0, 0, m_objHandle3, true);
}

void ClearScene::ForeDraw()
{
	// 前景はスクロールさせる
	int scrollFg = static_cast<int>(m_scrollX) % static_cast<int>(m_fgW);
	for (int index = 0; index < 2; index++)
	{
		DrawGraph(-scrollFg + index * m_fgW,
			kScreenHeight - m_fgH,
			m_fgHandle,
			true);
	}
}

void ClearScene::End()
{
	// BGMを止める
	StopSoundMem(m_bgmHandle);
	// BGMのデリート
	DeleteSoundMem(m_bgmHandle);
}

bool ClearScene::IsSceneEnd()
{
	return m_isSceneEnd;
}
