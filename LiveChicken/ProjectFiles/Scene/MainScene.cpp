#include "MainScene.h"
#include "../Player.h"
#include "../Map.h"
#include "../Camera.h"
#include "../Enemy/EnemyManager.h"

#include "SceneGameOver.h"
#include "SceneClear.h"

MainScene::MainScene():
	m_bgPos{kBgPosX,kBgPosY,kBgPosZ},
	m_noticeLogoHandle1(-1),
	m_noticeLogoHandle2(-1),
	m_noticeLogoHandle3(-1),
	m_noticeLogoHandle4(-1),
	m_tutorialHandle(-1),
	m_bgHandle(-1),
	m_bgmHandle(-1),
	m_clearSeHandle(-1),
	m_noticeSeHandle(-1),
	m_missSeHandle(-1),
	m_timeFontHandle(-1),
	m_timeBackFontHandle(-1),
	m_timeFrame(0),
	m_time(0),
	m_blinkingNoticeFrame(0),
	m_isNotice(false)
{
	/*画像のロード*/
	m_bgHandle = LoadGraph("Data/Image/Background/StageBg.png");
	m_noticeLogoHandle1 = LoadGraph("Data/Image/Logo/Notice1.png");
	m_noticeLogoHandle2 = LoadGraph("Data/Image/Logo/Notice2.png");
	m_noticeLogoHandle3 = LoadGraph("Data/Image/Logo/Notice3.png");
	m_noticeLogoHandle4 = LoadGraph("Data/Image/Logo/Notice4.png");
	m_tutorialHandle = LoadGraph("Data/Image/Logo/Tutorial.png");
	/*BGM、SEのロード*/
	m_bgmHandle = LoadSoundMem("Data/Sound/BGM/Main.ogg");
	m_clearSeHandle = LoadSoundMem("Data/Sound/SE/Clear.mp3");
	m_noticeSeHandle = LoadSoundMem("Data/Sound/SE/Notice.mp3");
	m_missSeHandle = LoadSoundMem("Data/Sound/SE/Miss.mp3");
	// 秒数表示用のフォント
	m_timeFontHandle = CreateFontToHandle("GN-キルゴUかなNA", 100, -1);
	m_timeBackFontHandle = CreateFontToHandle("GN-キルゴUかなNA", 110, -1);
	/*ポインタの生成*/
	m_pPlayer = make_shared<Player>();
	m_pCamera = make_shared<Camera>();
	m_pMap = make_shared<Map>();
	m_pEnemy = make_shared<EnemyManager>();
	m_pEnemy->GetPlayer(m_pPlayer);
}

MainScene::~MainScene()
{
	/*画像のデリート*/
	DeleteGraph(m_bgHandle);
	DeleteGraph(m_noticeLogoHandle1);
	DeleteGraph(m_noticeLogoHandle2);
	DeleteGraph(m_noticeLogoHandle3);
	DeleteGraph(m_noticeLogoHandle4);
	DeleteGraph(m_tutorialHandle);
	/*BGM、SEのデリート*/
	DeleteSoundMem(m_bgmHandle);
	DeleteSoundMem(m_clearSeHandle);
	DeleteSoundMem(m_noticeSeHandle);
	DeleteSoundMem(m_missSeHandle);
	/*フォントのデリート*/
	DeleteFontToHandle(m_timeFontHandle);
	DeleteFontToHandle(m_timeBackFontHandle);
}

void MainScene::Init()
{
	// マップのロード
	m_pMap->Load();
	// 敵の生成
	m_pEnemy->CreateEnemyes();

	/*BGM、SEのボリューム設定*/
	ChangeVolumeSoundMem(kBgmVolume, m_bgmHandle);
	ChangeVolumeSoundMem(kSeVolume, m_clearSeHandle);
	ChangeVolumeSoundMem(kSeVolume, m_noticeSeHandle);
	ChangeVolumeSoundMem(kSeVolume, m_missSeHandle);

	// BGMの再生
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP);
}

shared_ptr<SceneBase> MainScene::Update(Input& input)
{
	// 秒数
	m_timeFrame++;
	m_time = m_timeFrame / 60;
	// 点滅
	m_blinkingNoticeFrame++;

	m_pPlayer->Update(input);
	m_pMap->Update();
	m_pCamera->Update(*m_pPlayer);
	m_pEnemy->Update();
	NoticeSeRing();
	NoticeBlinking();

	// 背景のスクロール
	m_bgPos = VAdd(m_bgPos, VGet(kBgSpeed, 0.0f, 0.0f));

	// クリア時の処理
	if (m_time == kClearTime + 1)
	{
		if (!CheckSoundMem(m_clearSeHandle))
		{
			PlaySoundMem(m_clearSeHandle, DX_PLAYTYPE_BACK);
		}
	}
	if (m_time >= kClearTime + 3)
	{
		return make_shared<SceneClear>();
	}
	// ゲームオーバー時の処理
	if (m_pEnemy->CollisionPlayer())
	{
		StopSoundMem(m_bgmHandle);
		PlaySoundMem(m_missSeHandle, DX_PLAYTYPE_NORMAL);
		return make_shared<SceneGameOver>();
	}

#ifdef _DEBUG
	if (input.IsTriggered("Y"))
	{
		return make_shared<SceneGameOver>();
	}
	else if (input.IsTriggered("X"))
	{
		return make_shared<SceneClear>();
	}
#endif

	// シーン移動しないときは自身のポインタを返す
	return shared_from_this();
}

void MainScene::Draw()
{
	BackDraw();
	m_pMap->Draw();	
	m_pPlayer->Draw();
	m_pEnemy->Draw();
	TimeDraw();
	NoticeDraw();
	
	// デバッグ描画
#ifdef _DEBUG
		// XYZ軸
	float lineSize = 300.0f;
	DrawLine3D(VGet(-lineSize, 0, 0), VGet(lineSize, 0, 0), GetColor(255, 0, 0));
	DrawLine3D(VGet(0, -lineSize, 0), VGet(0, lineSize, 0), GetColor(0, 255, 0));
	DrawLine3D(VGet(0, 0, -lineSize), VGet(0, 0, lineSize), GetColor(0, 0, 255));
	DrawString(8, 8, "MainScene", 0xffffff);
	DrawFormatString(100, 100, 0xffffff, "%d", m_timeFrame / 60);
#endif // DEBUG
	
}

void MainScene::End()
{
	// BGMの停止
	StopSoundMem(m_bgmHandle);
}

void MainScene::NoticeSeRing()
{
	// 秒数経過のSE
	if (m_time == kTimeInterval1 || m_time == kTimeInterval2 || m_time == kTimeInterval3 || m_time == kTimeInterval4)
	{
		if (!CheckSoundMem(m_noticeSeHandle))
		{
			PlaySoundMem(m_noticeSeHandle, DX_PLAYTYPE_BACK);
		}
	}
}

void MainScene::NoticeBlinking()
{
	if (m_blinkingNoticeFrame % kNoticeBlinkingSpeed >= kNoticeBlinkingFrequency)
	{
		m_isNotice = true;
	}
	else
	{
		m_isNotice = false;
	}
}

void MainScene::TimeDraw()
{
	// 秒数の描画
	if (m_time <= kClearTime)
	{
		DrawFormatStringToHandle(kTimePosX, kTimePosY, 0x00008b, m_timeBackFontHandle, "%d", m_time);
		DrawFormatStringToHandle(kTimePosX, kTimePosY, 0xff7f50, m_timeFontHandle, "%d", m_time);
		DrawRotaGraphF(1250, 170, 0.35, 0.0, m_tutorialHandle, true, false);
	}
}

void MainScene::NoticeDraw()
{
	if (m_isNotice)
	{
		// 20秒ごとの通知の描画
		if (m_time >= kTimeInterval1 && m_time <= kTimeInterval1 + kNoticeDisplayTime)
		{
			DrawRotaGraphF(kNoticePosX, kNoticePosY, kNoticeExtRate, 0.0, m_noticeLogoHandle1, true, false);
		}
		else if (m_time >= kTimeInterval2 && m_time <= kTimeInterval2 + kNoticeDisplayTime)
		{
			DrawRotaGraphF(kNoticePosX, kNoticePosY, kNoticeExtRate, 0.0, m_noticeLogoHandle2, true, false);
		}
		else if (m_time >= kTimeInterval3 && m_time <= kTimeInterval3 + kNoticeDisplayTime)
		{
			DrawRotaGraphF(kNoticePosX, kNoticePosY, kNoticeExtRate, 0.0, m_noticeLogoHandle3, true, false);
		}
		else if (m_time >= kTimeInterval4 && m_time <= kTimeInterval4 + kNoticeDisplayTime)
		{
			DrawRotaGraphF(kNoticePosX, kNoticePosY, kNoticeExtRate, 0.0, m_noticeLogoHandle4, true, false);
		}
	}
}

void MainScene::BackDraw()
{
	// 画像サイズの取得
	BgSize bgSize;
	GetGraphSize(m_bgHandle, &bgSize.width, &bgSize.height);

	// スクロール量の計算
	int scrollBg = static_cast<int>(m_bgPos.x * 0.3f) % static_cast<int>(bgSize.width * kBgScale);

	// 背景の描画
	for (int i = 0; i < 2; i++)
	{
		DrawRotaGraph3D(scrollBg + i * bgSize.width * kBgScale,
			m_bgPos.y,
			m_bgPos.z,
			kBgScale,
			0.0f,
			m_bgHandle,
			true);
	}
}
