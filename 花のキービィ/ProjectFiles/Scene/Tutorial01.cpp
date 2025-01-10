#include "Tutorial01.h"
#include "../Game.h"
#include "../util/Input.h"
#include "../Player.h"
#include "../Camera.h"
#include "../UI.h"
#include "../Object/Door.h"

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

	// 矢印のアニメーション
	constexpr int kAnimFrame = 60;

	// 矢印の点滅速度
	constexpr int kArrowBlinkingSpeed = 30;
	// 矢印の点滅頻度
	constexpr int kArrowBlinkingFrequency = 15;

	// ボタンの大きさ
	constexpr int kButtonWidth = 26;
	constexpr int kButtonHeight = 29;

	// ボタンのアニメーション
	constexpr int kUseFrame[] = { 0,1,0 };
	// アニメーション1コマのフレーム数
	constexpr int kAnimFrameNum = 12;
	// アニメーション１サイクルのフレーム数
	constexpr int kAnimFrameCycle = _countof(kUseFrame) * kAnimFrameNum;

	// フェードのスピード
	constexpr int kFadeSpeed = 8;
	// フェードの最大Alpha
	constexpr int kFadeMaxAlpha = 255;

	// ドアの位置
	constexpr int kDoorX = kChipWidth * 23;
	constexpr int kDoorY = kChipHeight * 12;

	// ボタンの位置
	constexpr int kButtonX = 780;
	constexpr int kButtonY = 390;

	// ボタンの拡大率
	constexpr float kButtonRate = 2.0f;

	// 背景のスクロールの速さ
	constexpr int kScrollSpeed = 2;

	// 背景に重ねる色の濃さ
	constexpr int kBackOverlayAlpha = 50;

	/* 矢印の位置*/
	// プレイヤーを指す矢印
	constexpr int kPlayerToPointArrowX = kChipWidth * 5;
	constexpr int kPlayerToPointArrowY = kChipHeight * 10;
	constexpr int kPlayerToPointArrowAngle = 43;
	// ドアを指す矢印
	constexpr int kDoorToPointArrowX = kChipWidth * 24;
	constexpr int kDoorToPointArrowY = kChipHeight * 9;
	constexpr int kDoorToPointArrowAngle = 43;

}

Tutorial01::Tutorial01() :
	m_fgHandle(-1),
	m_bgHandle(-1),
	m_chipHandle(-1),
	m_buttonHandle(-1),
	m_arrowHandle(-1),
	m_graphChipNumX(0),
	m_graphChipNumY(0),
	m_buttonAnimFrame(0),
	m_blinkingArrowAnimFrame(0),
	m_bgmHandle(-1),
	m_pushSeHandle(-1),
	m_moveSeHandle(-1),
	m_fadeAlpha(0),
	m_tutorialCount(0),
	m_scrollX(0),
	m_backW(0),
	m_backH(0),
	m_isOperation(false),
	m_isDoor(false),
	m_isSceneEnd(false)
{
	/*画像のロード*/
	m_bgHandle = LoadGraph(_T("data/Map/TutorialBg.png"));
	m_fgHandle = LoadGraph(_T("data/Map/TutorialFg.png"));
	m_arrowHandle = LoadGraph(_T("data/Tutorial/Arrow.png"));
	m_buttonHandle = LoadGraph(_T("data/Object/Button.png"));
	m_windowTextHandle[0] = LoadGraph(_T("data/Tutorial/00.png"));
	m_windowTextHandle[1] = LoadGraph(_T("data/Tutorial/01.png"));
	m_windowTextHandle[2] = LoadGraph(_T("data/Tutorial/02.png"));
	m_windowTextHandle[3] = LoadGraph(_T("data/Tutorial/04.png"));
	m_windowTextHandle[4] = LoadGraph(_T("data/Tutorial/04.png"));
	m_windowTextHandle[5] = LoadGraph(_T("data/Tutorial/05.png"));
	m_windowTextHandle[6] = LoadGraph(_T("data/Tutorial/06.png"));
	m_chipHandle = LoadGraph("data/Map/map.png");

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

Tutorial01::~Tutorial01()
{
	/*グラフィックのデリート*/
	DeleteGraph(m_bgHandle);
	DeleteGraph(m_fgHandle);
	DeleteGraph(m_chipHandle);
	DeleteGraph(m_arrowHandle);
	DeleteGraph(m_buttonHandle);
	for (int i = 0; i < 7; i++)
	{
		DeleteGraph(m_windowTextHandle[i]);
	}
}

void Tutorial01::Init()
{
	// マップの広さを決める
	m_pCamera->StartMixture();
	m_pCamera->Init();

	// プレイヤーのHPと残機を全回復する
	m_pPlayer->CompleteRecoveryHp();
	m_pPlayer->CompleteRecoveryLives();

	// BGMの読み込み
	m_bgmHandle = LoadSoundMem(_T("data/Sound/BGM/Tutorial.mp3"));
	// BGMの音量設定
	ChangeVolumeSoundMem(200, m_bgmHandle);

	// SEの読み込み
	m_moveSeHandle = LoadSoundMem(_T("data/Sound/SE/Move.wav"));
	m_pushSeHandle = LoadSoundMem(_T("data/Sound/SE/Push.wav"));

	// BGMの再生
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP, true);

	// プレイヤーの初期化
	m_pPlayer->Init();
	// 初期座標の設定
	m_pPlayer->SetStartPos(kStartX, kStartY);
	// ドアの初期化
	m_pDoor->Init(kDoorX, kDoorY);
	// UIの初期化
	m_pUI->Init();

	// シーンが終了しないようにする
	m_tutorialCount = 0;
	m_isOperation = false;
	m_isSceneEnd = false;
}

void Tutorial01::Update(Input& input)
{
	if (!m_isSceneEnd)
	{
		// 背景をスクロールさせる
		m_scrollX += kScrollSpeed;

		m_pUI->Update();

		// 矢印の点滅
		m_blinkingArrowAnimFrame--;
		if (m_blinkingArrowAnimFrame < 0)m_blinkingArrowAnimFrame = kAnimFrame;

		// プレイヤーが動かせる状態かどうか
		if (m_isOperation)
		{
			m_pPlayer->Update(input, m_chipData, kChipNumX, kChipNumY, kChipWidth, kChipHeight);
		}
		// ドアが存在しているかどうか
		if (m_isDoor)
		{
			m_pDoor->Update();

			if (m_pDoor->IsColision())
			{
				if (input.IsTriggered("up"))
				{
					PlaySoundMem(m_moveSeHandle, DX_PLAYTYPE_BACK, true);
					m_isSceneEnd = true;
				}
			}
		}

		// 説明中の処理
		if (!m_isOperation)
		{
			// Aボタンを押すと次の説明に進み
			// チュートリアルのカウンタが一つ増える
			if (input.IsTriggered("A"))
			{
				PlaySoundMem(m_pushSeHandle, DX_PLAYTYPE_BACK, true);
				m_tutorialCount++;
			}
		}

		// チュートリアルのカウンタが3になるとドアが出現する
		if (m_tutorialCount == 3)
		{
			m_isDoor = true;
			m_tutorialCount++;
		}
		// チュートリアルのカウンタが6になるとプレイヤーが動かせるようになる
		if (m_tutorialCount >= 6)
		{
			m_isOperation = true;
		}

		// ボタンのアニメーション
		if (!m_isOperation)
		{	
			m_buttonAnimFrame++;
			if (m_buttonAnimFrame >= kAnimFrameCycle)
			{
				m_buttonAnimFrame = 0;
			}
		}

		// フェードイン
		m_fadeAlpha -= kFadeSpeed;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
		}
	}
	// 場面転換
	else
	{
		// フェードアウト
		m_fadeAlpha += kFadeSpeed;
		if (m_fadeAlpha > kFadeMaxAlpha)
		{
			m_fadeAlpha = kFadeMaxAlpha;
		}
	}
}

void Tutorial01::Draw()
{
	// 背景の表示
	BackDraw();

	// マップチップの描画
	ChipDraw();

	// ドアの描画
	if (m_isDoor)
	{
		m_pDoor->Draw();
	}

	// UIの描画
	m_pUI->Draw();
	// チュートリアルの説明の画像の描画
	DrawGraph(0, 0, m_windowTextHandle[m_tutorialCount], true);
	// プレイヤーの描画
	m_pPlayer->Draw();

	// ボタンの描画
	if (!m_isOperation)
	{
		// アニメーション
		int animFrame = m_buttonAnimFrame / kAnimFrameNum;
		int srcY = kUseFrame[animFrame] * kButtonHeight;

		DrawRectRotaGraph(kButtonX, kButtonY, 0, srcY, kButtonWidth, kButtonHeight, kButtonRate, 0.0, m_buttonHandle, true);
	}
	// 矢印の点滅
	if (m_blinkingArrowAnimFrame % kArrowBlinkingSpeed >= kArrowBlinkingFrequency)return;
	// 矢印の描画
	if (m_tutorialCount == 2)
	{
		DrawRotaGraph(kPlayerToPointArrowX, kPlayerToPointArrowY, 1.0, kPlayerToPointArrowAngle, m_arrowHandle, true);
	}
	if (m_tutorialCount >= 3)
	{
		DrawRotaGraph(kDoorToPointArrowX, kDoorToPointArrowY, 1.0, kDoorToPointArrowAngle, m_arrowHandle, true);
	}

	// フェードの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);
	DrawBox(0, 0, kScreenWidht, kScreenHeight, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}

void Tutorial01::ChipDraw()
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

void Tutorial01::BackDraw()
{
	// 自動スクロール
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

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kBackOverlayAlpha);
	DrawBox(0, 0, kScreenWidht, kScreenHeight, 0xb22222, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}

void Tutorial01::End()
{
	// BGMを止める
	StopSoundMem(m_bgmHandle);
	// BGMをメモリから削除する
	DeleteSoundMem(m_bgmHandle);
	// SEをメモリから削除する
	DeleteSoundMem(m_pushSeHandle);
	DeleteSoundMem(m_moveSeHandle);
}

bool Tutorial01::IsSceneEnd() const
{
	return m_isSceneEnd && (m_fadeAlpha >= kFadeMaxAlpha);
}
