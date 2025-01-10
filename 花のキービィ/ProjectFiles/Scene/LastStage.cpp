#include "LastStage.h"
#include "../Game.h"
#include "../util/Input.h"
#include "../Player.h"
#include "../Camera.h"
#include "../UI.h"
#include "../Object/GoalFlower.h"
#include "../Enemy/Boss.h"

#include "DxLib.h"

namespace
{
	// マップの広さ
	constexpr int kMapWidth = Game::kScreenWidth;
	constexpr int kMapHeight = Game::kScreenHeight;

	// マップチップの情報
	constexpr int kChipWidth = 64;
	constexpr int kChipHeight = 64;

	// 背景の大きさ
	constexpr int kBackWidth = Game::kScreenWidth;
	constexpr int kBackHeight = Game::kScreenHeight;

	// チップを置く数
	constexpr int kChipNumX = Game::kScreenWidth / kChipWidth;
	constexpr int kChipNumY = Game::kScreenHeight / kChipHeight;

	// プレイヤーの初期座標
	constexpr int kStartX = kChipWidth * 4;
	constexpr int kStartY = kChipHeight * 12;

	// 画面の大きさ
	constexpr int kScreenWidht = Game::kScreenWidth;
	constexpr int kScreenHeight = Game::kScreenHeight;

	// ボスキャラクターのHP
	constexpr int kBossHp = 6;

	// プレイヤーがミスをしたときの演出フレーム
	constexpr int kMissSe02Frame = 60;
	constexpr int kMissFrameMax = 60 * 2;
	constexpr float kMissFadeFrame = 60 * 2.2;
	constexpr float kContinueFrame = 60 * 2.6;
}

LastStage::LastStage():
	m_bgHandle(-1),
	m_fgHandle(-1),
	m_chipHandle(-1),
	m_moveFlowerHandle(-1),
	m_bossHandle(-1),
	m_graphChipNumX(0),
	m_graphChipNumY(0),
	m_bgmHandle(-1),
	m_missSeHandle(-1),
	m_missSeHandle02(-1),
	m_flowerMoveSeHandle(-1),
	m_fadeAlpha(0),
	m_isMissFade(0),
	m_isBossExist(false),
	m_shakeFrame(0),
	m_nextScene(kClear),
	m_isMiss(false),
	m_scrollX(0),
	m_backW(0),
	m_backH(0),
	m_isClear(false),
	m_moveEventX(0),
	m_moveEventY(0),
	m_moveEventFrame(0),
	m_extRate(0),
	m_isSceneEnd(false)
{
	// ゲーム画面描画先の生成
	m_gameScreenHandle = MakeScreen(kScreenWidht, kScreenHeight, true);
	/*画像のロード*/
	m_bgHandle = LoadGraph(_T("data/Map/TutorialBg.png"));
	m_fgHandle = LoadGraph(_T("data/Map/TutorialFg.png"));
	m_chipHandle = LoadGraph("data/Map/map.png");
	m_moveFlowerHandle = LoadGraph(_T("data/Event/MoveFlowerColor.png"));

	/*SEのロード*/
	m_flowerMoveSeHandle = LoadSoundMem(_T("data/Sound/SE/FlowerMove.mp3"));

	// マップチップの配置データの初期化
	m_chipData = {
		{04,04,04,04,04,04,03,04,04,04,04,03,04,04,04,04,04,03,04,04,04,04,04,04,04,04,03,04,04,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04,04,04,04,04,04,04,03,04,04,04,04,04,03,04,04,04,04,03,04,04,04,04,04,04,04,03,04,04,04},
		{04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04},
		{04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04},
		{04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04}
	};

	// マップチップの数を数える
	int graphW = 0;
	int graphH = 0;
	GetGraphSize(m_chipHandle, &graphW, &graphH);

	m_graphChipNumX = graphW / kChipWidth;
	m_graphChipNumY = graphH / kChipHeight;
}

LastStage::~LastStage()
{
	/*グラフィックのデリート*/
	DeleteGraph(m_gameScreenHandle);
	DeleteGraph(m_bgHandle);
	DeleteGraph(m_fgHandle);
	DeleteGraph(m_chipHandle);
	DeleteGraph(m_moveFlowerHandle);

}

void LastStage::Init()
{
	m_pCamera->StartMixture();

	// BGMの読み込み
	m_bgmHandle = LoadSoundMem(_T("data/Sound/BGM/Boss.mp3"));
	// BGMの音量設定
	ChangeVolumeSoundMem(200, m_bgmHandle);

	/*SEのロード*/
	m_missSeHandle = LoadSoundMem(_T("data/Sound/SE/Miss.wav"));
	m_missSeHandle02 = LoadSoundMem(_T("data/Sound/SE/Miss02.mp3"));

	// 花に色を付ける
	m_pFlower->SetFlowerColor();

	RestartInit();	

	m_pFlower->Init(kMapWidth / 2, kMapHeight / 2);

	m_moveEventX = 880;
	m_moveEventY = 270;

	// シーンが終了しないようにする
	m_isSceneEnd = false;
	m_isClear = false;
	m_isBossExist = false;
	m_moveEventFrame = 0;
	m_isMiss = false;
}

void LastStage::Update(Input& input)
{
	// スクロール
	m_scrollX += 2;

	if (!m_isSceneEnd)
	{
		// プレイヤーがミスをした場合はプレイヤー以外の処理を止める
		if (!m_isMiss || !m_isClear)
		{
			m_pFlower->Update();
			m_pUI->Update();
			m_pBoss->Update();
		}

		if (!m_isClear)
		{
			// プレイヤーにマップチップのデータを渡しておく
			m_pPlayer->Update(input, m_chipData, kChipNumX, kChipNumY, kChipWidth, kChipHeight);
		}

		// 画面揺れの処理
		m_shakeFrame--;
		if (m_shakeFrame < 0) m_shakeFrame = 0;

		// ミス
		OnMiss();

		// 残機が0になったらゲームオーバー
		if (m_pPlayer->GetRemainingLives() < 0)
		{
			m_nextScene = kGameOver;
			m_isSceneEnd = true;
		}


		// プレイヤーがミスをした時のフェード処理
		if (m_isMissFade)
		{
			m_fadeAlpha += 20;
			if (m_fadeAlpha > 255)
			{
				m_fadeAlpha = 255;
				m_isMissFade = false;
			}
		}
		else
		{
			m_fadeAlpha -= 20;
			if (m_fadeAlpha < 0)
			{
				m_fadeAlpha = 0;
			}
		}

		// ボスのHPが0になったらBGMを止める
		if (m_pBoss->IsBossEnd())
		{
			StopSoundMem(m_bgmHandle);
		}

		// ボスが消えたら花が出現する
		if (!m_pBoss->IsExist())
		{
			// プレイヤーが花に触れたら次のシーンに移動する
			// ゴール
			if (m_pFlower->IsColision())
			{
				StopSoundMem(m_bgmHandle);
				m_nextScene = kClear;
				m_isClear = true;

			}
		}
		

		if (m_isClear)
		{
			// クリア演出
			ClearPerformance();
		}

		// フェードアウト
		m_fadeAlpha -= 12;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
		}

	}
	// 場面転換のフェード
	else // フェードイン
	{
		m_fadeAlpha += 12;
		if (m_fadeAlpha > 255)
		{
			m_fadeAlpha = 255;
		}
	}	

#ifdef _DEBUG
	if (input.IsTriggered("S"))
	{
		m_nextScene = kGameOver;
		m_isSceneEnd = true;
	}
	else if (input.IsTriggered("P"))
	{
		m_nextScene = kClear;
		m_isSceneEnd = true;
	}
#endif
}

void LastStage::Draw()
{
	// 自分で生成したグラフィックデータに対して書き込みを行う
	SetDrawScreen(m_gameScreenHandle);

	// 描画先スクリーンをクリアする
	ClearDrawScreen();

	// 背景の描画
	BackDraw();

	// マップチップの描画
	ChipDraw();

	if (!m_pBoss->IsExist() && !m_pPlayer->IsFlowerMove())
	{
		// ゴールの花の描画
		m_pFlower->Draw();
	}
	
	// ボスの描画
	m_pBoss->Draw();

	if (!m_pPlayer->IsFlowerMove())
	{
		// プレイヤーの描画
		m_pPlayer->Draw();
	}
	

	// UIの描画
	m_pUI->Draw();

	// 花に乗った移動
	if (m_pPlayer->IsFlowerMove())
	{
		DrawRotaGraph(m_moveEventX, m_moveEventY, m_extRate, 0.0, m_moveFlowerHandle, true);
	}

	// バックバッファに書き込む設定に戻しておく
	SetDrawScreen(DX_SCREEN_BACK);

	// ゲーム画面をバックバッファに描画する
	int screenX = 0;
	int screenY = 0;
	if (m_shakeFrame > 0)
	{
		// 画面を揺らす
		screenX = GetRand(16) - 8;
		screenY = GetRand(16) - 8;
	}

	// スクリーンの描画
	DrawGraph(screenX, screenY, m_gameScreenHandle, true);

	// フェードの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);
	DrawBox(0, 0, kScreenWidht, kScreenHeight, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}

void LastStage::BackDraw()
{
	GetGraphSize(m_fgHandle, &m_backW, &m_backH);

	DrawGraph(0, 0, m_bgHandle, true);
	int scrollBack = static_cast<int>(m_scrollX) % m_backW;

	for (int index = 0; index < 2; ++index)
	{
		DrawGraph(-scrollBack + index * m_backW,
			kScreenHeight - m_backH,
			m_fgHandle,
			true);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50);
	DrawBox(0, 0, kScreenWidht, kScreenHeight, 0xb22222, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}

void LastStage::ChipDraw()
{
	// プレイヤーの位置に応じたスクロール量を決定する
	int scrollX = m_pCamera->GetScrollX();
	int scrollY = m_pCamera->GetScrollY();

	// マップチップを敷き詰める
	for (int x = 0; x < kChipNumX; x++)
	{
		for (int y = 0; y < kChipNumY; y++)
		{
			int posX = kChipWidth * x - scrollX;
			int posY = kChipHeight * y - scrollY;

			// 画面外を描画しない
			if (posX < 0 - kChipWidth)        continue;
			if (posX > Game::kScreenWidth)    continue;
			if (posY < 0 - kChipHeight)        continue;
			if (posY > Game::kScreenHeight)    continue;

			int chipNo = m_chipData[y][x];

			// マップチップのグラフィックの切り出し座標
			int srcX = kChipWidth * (chipNo % m_graphChipNumX);
			int srcY = kChipHeight * (chipNo / m_graphChipNumX);

			DrawRectGraph(posX, posY, srcX, srcY, kChipWidth, kChipHeight, m_chipHandle, true);
		}
	}
}

void LastStage::OnMiss()
{
	// プレイヤーがミスをした場合は残機を減らしてHPを全回復して初期位置に戻す
	// このステージでプレイヤーが落ちることはない為、HPがゼロになった場合のミスだけ処理を書いておく
	if (m_pPlayer->GetHp() <= 0)
	{
		// 画面を揺らす
		if (!m_isMiss)
		{
			m_shakeFrame = 8;
			m_pPlayer->DamageMiss();
		}

		// BGMを止める
		StopSoundMem(m_bgmHandle);

		// SEを鳴らす
		if (!CheckSoundMem(m_missSeHandle02) && !m_isMiss)
		{
			PlaySoundMem(m_missSeHandle02, DX_PLAYTYPE_BACK, true);
		}
		m_pPlayer->OnMiss();

		// 画面を止める
		m_isMiss = true;

	}
	// ミスをして少したってからミス演出用のSEを鳴らす
	if (m_pPlayer->GetMissFrame() >= kMissSe02Frame)
	{
		if (!CheckSoundMem(m_missSeHandle))
		{
			PlaySoundMem(m_missSeHandle, DX_PLAYTYPE_BACK, true);
		}
	}
	// フェード演出
	if (m_pPlayer->GetMissFrame() >= kMissFrameMax)
	{
		m_isMissFade = true;
	}
	// リスタート
	if (m_pPlayer->GetMissFrame() >= kContinueFrame)
	{
		// プレイヤーの残機を減らしてHPは全回復する
		// プレイヤーやエネミーの位置を初期化する
		m_pPlayer->DeleteLives();
		m_pPlayer->CompleteRecoveryHp();
		RestartInit();
		m_pPlayer->GravityReset();
		m_isMiss = false;

	}
}

void LastStage::ClearPerformance()
{
	m_pPlayer->OnDance();

	// 移動演出
	if (m_pPlayer->IsFlowerMove())
	{
		m_moveEventFrame++;
		m_extRate += 0.1f;


		if (m_extRate >= 2.0)
		{
			m_extRate = 2.0;

			if (!CheckSoundMem(m_flowerMoveSeHandle))
			{
				PlaySoundMem(m_flowerMoveSeHandle, DX_PLAYTYPE_LOOP, true);
			}

			if (m_moveEventFrame >= 50 && m_moveEventFrame < 120)
			{
				m_moveEventX -= 12;
				m_moveEventY += 8;
			}
			if (m_moveEventFrame >= 120)
			{
				m_moveEventX += 18;
				m_moveEventY -= 10;
			}

			if (m_moveEventX >= kScreenWidht)
			{
				m_isSceneEnd = true;
			}
		}

	}
}

void LastStage::End()
{
	// BGMを止める
	StopSoundMem(m_bgmHandle);
	// BGMをメモリから削除する
	DeleteSoundMem(m_bgmHandle);
	// SEを止める
	StopSoundMem(m_flowerMoveSeHandle);
	// SEをメモリから削除する
	DeleteSoundMem(m_flowerMoveSeHandle);
	DeleteSoundMem(m_missSeHandle);
	DeleteSoundMem(m_missSeHandle02);
}

bool LastStage::IsSceneEnd() const
{
	return m_isSceneEnd && (m_fadeAlpha >= 255);
}

void LastStage::RestartInit()
{
	/*プレイヤーの初期化*/
	m_pPlayer->Init();
	// 初期化座標を設定
	m_pPlayer->SetStartPos(kStartX, kStartY);
	// 重力の初期化
	m_pPlayer->GravityReset();

	// ボスの初期化
	m_pBoss->Init();

	m_pCamera->Init();
	m_pUI->Init();

	// BGMの再生
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP, true);

	// 画面揺れのフレームをゼロに戻す
	m_shakeFrame = 0;

	m_isMiss = false;
	m_isBossExist = false;
}