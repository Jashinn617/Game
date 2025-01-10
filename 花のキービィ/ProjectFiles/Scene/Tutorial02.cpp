#include "Tutorial02.h"
#include "../Game.h"
#include "../util/Input.h"
#include "../Player.h"
#include "../Camera.h"
#include "../UI.h"
#include "../Object/GoalFlower.h"
#include "../Enemy/StopEnemy.h"
#include "../Object/RecoveryItem.h"

#include "DxLib.h"

namespace
{
	// マップの広さ
	constexpr int kMapWidth = Game::kScreenWidth * 2;
	constexpr int kMapHeight = Game::kScreenHeight;

	// マップチップの情報
	constexpr int kChipWidth = 64;
	constexpr int kChipHeight = 64;

	// 背景の大きさ
	constexpr int kBackWidth = Game::kScreenWidth;
	constexpr int kBackHeight = Game::kScreenHeight;

	// チップを置く数
	constexpr int kChipNumX = Game::kScreenWidth / kChipWidth * 2;
	constexpr int kChipNumY = Game::kScreenHeight / kChipHeight;

	// プレイヤーの初期座標
	constexpr int kStartX = kChipWidth * 4;
	constexpr int kStartY = kChipHeight * 12;

	// 画面の大きさ
	constexpr int kScreenWidht = Game::kScreenWidth;
	constexpr int kScreenHeight = Game::kScreenHeight;

	// 矢印のアニメーション
	constexpr int kAnimFrame = 60;

	// エネミーの数
	constexpr int kStopEnemyNum = 30;

	// アイテムの数
	constexpr int kItemNum = 5;

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

	// SEの最大音量
	constexpr int kMaxSound = 255;

	// チュートリアルのウィンドウの数
	constexpr int kWindowNum = 21;

	// 移動イベントの初期座標
	constexpr int kEventX = kChipWidth * 15;
	constexpr int kEventY = kChipHeight * 11;

	// 花の初期位置
	constexpr int kFlowerX = kChipWidth * 18;
	constexpr int kFlowerY = kChipHeight * 5;

	// 花の拡大率
	constexpr float kFlowerRate = 1.5f;

	// ボタンの位置
	constexpr int kButtonX = 780;
	constexpr int kButtonY = 390;

	// ボタンの拡大率
	constexpr float kButtonRate = 2.0f;

	// 背景のスクロールの速さ
	constexpr int kScrollSpeed = 2;


	// 背景に重ねる色の濃さ
	constexpr int kBackOverlayAlpha = 50;

	// 矢印の点滅速度
	constexpr int kArrowBlinkingSpeed = 30;
	// 矢印の点滅頻度
	constexpr int kArrowBlinkingFrequency = 15;

}

Tutorial02::Tutorial02():
	m_bgHandle(-1),
	m_fgHandle(-1),
	m_chipHandle(-1),
	m_buttonHandle(-1),
	m_evevtHandle(-1),
	m_windowTextHandle{-1},
	m_arrowHandle(-1),
	m_pushSeHandle(-1),
	m_moveSeHandle(-1),
	m_buttonAnimFrame(0),
	m_fadeAlpha(0),
	m_arrowAnimFrame(0),
	m_tutorialCount(0),
	m_scrollX(0),
	m_backW(0),
	m_backH(0),
	m_graphChipNumX(0),
	m_graphChipNumY(0),
	m_eventX(0),
	m_eventY(0),
	m_eventFrame(0),
	m_isSceneEnd(false),
	m_isOperation(false),
	m_isEnemy(false),
	m_isItem(false),
	m_isEvent(false)
{
	/*グラフィックのロード*/
	m_bgHandle = LoadGraph(_T("data/Map/TutorialBg.png"));
	m_fgHandle = LoadGraph(_T("data/Map/TutorialFg.png"));
	m_arrowHandle = LoadGraph(_T("data/Tutorial/Arrow.png"));
	m_buttonHandle = LoadGraph(_T("data/Object/Button.png"));
	m_evevtHandle = LoadGraph(_T("data/Event/MoveFlower.png"));
	m_chipHandle = LoadGraph("data/Map/map.png");
	m_windowTextHandle[0] = LoadGraph(_T("data/Tutorial/07.png"));
	m_windowTextHandle[1] = LoadGraph(_T("data/Tutorial/08.png"));
	m_windowTextHandle[2] = LoadGraph(_T("data/Tutorial/09.png"));
	m_windowTextHandle[3] = LoadGraph(_T("data/Tutorial/10.png"));
	m_windowTextHandle[4] = LoadGraph(_T("data/Tutorial/11.png"));
	m_windowTextHandle[5] = LoadGraph(_T("data/Tutorial/12.png"));
	m_windowTextHandle[6] = LoadGraph(_T("data/Tutorial/13.png"));
	m_windowTextHandle[7] = LoadGraph(_T("data/Tutorial/14.png"));
	m_windowTextHandle[8] = LoadGraph(_T("data/Tutorial/15.png"));
	m_windowTextHandle[9] = LoadGraph(_T("data/Tutorial/16.png"));
	m_windowTextHandle[10] = LoadGraph(_T("data/Tutorial/17.png"));
	m_windowTextHandle[11] = LoadGraph(_T("data/Tutorial/18.png"));
	m_windowTextHandle[12] = LoadGraph(_T("data/Tutorial/19.png"));
	m_windowTextHandle[13] = LoadGraph(_T("data/Tutorial/20.png"));
	m_windowTextHandle[14] = LoadGraph(_T("data/Tutorial/21.png"));
	m_windowTextHandle[15] = LoadGraph(_T("data/Tutorial/22.png"));
	m_windowTextHandle[16] = LoadGraph(_T("data/Tutorial/23.png"));
	m_windowTextHandle[17] = LoadGraph(_T("data/Tutorial/24.png"));
	m_windowTextHandle[18] = LoadGraph(_T("data/Tutorial/25.png"));
	m_windowTextHandle[19] = LoadGraph(_T("data/Tutorial/26.png"));
	m_windowTextHandle[20] = LoadGraph(_T("data/Tutorial/27.png"));
	

	m_pStopEnemy.resize(kStopEnemyNum);
	m_pItem.resize(kItemNum);
	// マップチップの配置データの初期化
	m_chipData = {
		{04,04,04,04,04,03,04,04,04,04,04,04,04,04,04,04,04,04,03,04,04,04,04,04,04,04,04,04,04,03,04,04,04,04,04,04,04,04,04,03,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,03,04,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04,03,04,04,03,04,04,03, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04,04,04,04,04,04,04,04,04,04,04,04,03,04,04,04,04,04,04,04,04,04,04,03,04,04,04,04,04,04,04,04,04,04,03,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,03,04,04,04,03,04,04},
		{04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04},
		{04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04},
		{04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04},
	};
}

Tutorial02::~Tutorial02()
{
	/*グラフィックのデリート*/
	DeleteGraph(m_bgHandle);
	DeleteGraph(m_fgHandle);
	DeleteGraph(m_chipHandle);
	DeleteGraph(m_arrowHandle);
	DeleteGraph(m_buttonHandle);
	DeleteGraph(m_evevtHandle);
	for (int i = 0; i < kWindowNum; i++)
	{
		DeleteGraph(m_windowTextHandle[i]);
	}
}

void Tutorial02::Init()
{
	// カメラの初期化
	m_pCamera->StartHorizon2();
	m_pCamera->Init();

	// SEの読み込み
	m_moveSeHandle = LoadSoundMem(_T("data/Sound/SE/FlowerMove.mp3"));
	m_pushSeHandle = LoadSoundMem(_T("data/Sound/SE/Push.wav"));

	// 音量設定
	ChangeVolumeSoundMem(kMaxSound, m_moveSeHandle);

	// 花の色をモノクロにする
	m_pFlower->SetFlowerMonochrome();

	// マップチップの数を数える
	int graphW = 0;
	int graphH = 0;
	GetGraphSize(m_chipHandle, &graphW, &graphH);
	m_graphChipNumX = graphW / kChipWidth;
	m_graphChipNumY = graphH / kChipHeight;

	// プレイヤーの初期化
	m_pPlayer->Init();
	// プレイヤーの初期位置の設定
	m_pPlayer->SetStartPos(kStartX, kStartY);
	// UIの初期化
	m_pUI->Init();
	// ゴール用の花の初期化と初期位置の設定
	m_pFlower->Init(kFlowerX, kFlowerY);
	// 移動イベント時の位置設定
	m_eventX = kEventX;
	m_eventY = kEventY;
	// 敵キャラクターの初期化
	EnemyInit();
	// アイテムの初期化
	ItemInit();
	

	// シーンが終了しないようにする
	m_eventFrame = 0;
	m_tutorialCount = 0;
	m_isOperation = false;
	m_isSceneEnd = false;
	m_isEvent = false;
}

void Tutorial02::Update(Input& input)
{
	if (!m_isSceneEnd)
	{
		// 背景のスクロール
		m_scrollX += kScrollSpeed;

		// 敵キャラクターの更新
		if (m_isEnemy)
		{
			for (int i = 0; i < m_pStopEnemy.size(); i++)
			{
				m_pStopEnemy[i]->Update();
			}
		}
		// アイテムの更新
		if (m_isItem)
		{
			for (int i = 0; i < m_pItem.size(); i++)
			{
				m_pItem[i]->Update();
			}
		}
		// UIの更新
		m_pUI->Update();

		// 矢印のアニメーション
		m_arrowAnimFrame--;
		if (m_arrowAnimFrame < 0)m_arrowAnimFrame = kAnimFrame;

		// プレイヤーが動かせる状態で移動イベント中ではないときはプレイヤーの更新をする
		if (m_isOperation && !m_isEvent)
		{
			m_pPlayer->Update(input, m_chipData, kChipNumX, kChipNumY, kChipWidth, kChipHeight);
		}

		// 説明中の処理
		if (!m_isOperation && !m_isEvent)
		{
			if (input.IsTriggered("A"))
			{
				PlaySoundMem(m_pushSeHandle, DX_PLAYTYPE_BACK, true);
				m_tutorialCount++;
			}
		}

		// チュートリアルのカウント数による処理
		CountUpdate();

		// 説明中はボタンが表示される
		if (!m_isOperation)
		{
			// ボタンのアニメーション
			m_buttonAnimFrame++;
			if (m_buttonAnimFrame >= kAnimFrameCycle)
			{
				m_buttonAnimFrame = 0;
			}
		}

		// 移動のイベント
		MoveEvent();

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

void Tutorial02::Draw()
{
	// 背景の描画
	BackDraw();

	// マップチップの描画
	ChipDraw();

	// プレイヤーの位置に応じたスクロール量を決定する
	int scrollX = m_pCamera->GetScrollX();
	int scrollY = m_pCamera->GetScrollY();

	// 敵キャラクターの描画
	if (m_isEnemy)
	{
		for (int i = 0; i < m_pStopEnemy.size(); i++)
		{
			m_pStopEnemy[i]->Draw();
		}
	}
	
	// アイテムの描画
	if (m_isItem)
	{
		for (int i = 0; i < m_pItem.size(); i++)
		{
			m_pItem[i]->Draw();
		}
	}

	// 移動のイベント
	if (m_isEvent)
	{
		DrawRotaGraph(m_eventX, m_eventY, kFlowerRate, 0.0, m_evevtHandle, true);
	}
	
	// UIの描画
	m_pUI->Draw();

	// 説明ウィンドウの描画
	DrawGraph(0, 0, m_windowTextHandle[m_tutorialCount], true);

	// 花の描画
	if (m_isOperation)
	{
		m_pFlower->Draw();
	}
	
	// プレイヤーの描画
	if (!m_isEvent)
	{
		m_pPlayer->Draw();
	}

	// ボタン
	if (!m_isOperation && !m_isEvent)
	{
		// アニメーション
		int animFrame = m_buttonAnimFrame / kAnimFrameNum;
		int srcY = kUseFrame[animFrame] * kButtonHeight;

		DrawRectRotaGraph(kButtonX, kButtonY, 0, srcY, kButtonWidth, kButtonHeight, kButtonRate, 0.0, m_buttonHandle, true);
	}

	if (m_arrowAnimFrame % kArrowBlinkingSpeed >= kArrowBlinkingFrequency)return;

	// 矢印の描画
	if (m_tutorialCount == 4)
	{
		DrawRotaGraph(kChipWidth * 19 - scrollX, kChipHeight * 3 - scrollY, 1.0, 43, m_arrowHandle, true);
	}
	if (m_tutorialCount == 7)
	{
		DrawRotaGraph(kChipWidth * 24 - scrollX, kChipHeight * 9 - scrollY, 3.0, 54, m_arrowHandle, true);
	}
	if (m_tutorialCount == 9 || m_tutorialCount == 10)
	{
		DrawRotaGraph(kChipWidth * 17, kChipHeight * 12, 2.0, 43, m_arrowHandle, true);
		DrawRotaGraph(kChipWidth * 26, kChipHeight * 12, 2.0, 43, m_arrowHandle, true);
	}
	if (m_tutorialCount == 11)
	{
		DrawRotaGraph(kChipWidth * 24 - scrollX, kChipHeight * 10 - scrollY, 2.0, 43, m_arrowHandle, true);
	}

	// フェードの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);
	DrawBox(0, 0, kScreenWidht, kScreenHeight, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}

void Tutorial02::CountUpdate()
{
	// チュートリアルのカウンタが4になるとプレイヤーが動かせる様になる
	if (m_tutorialCount == 4)
	{
		m_isOperation = true;
		m_pFlower->Update();

		// 花に触れると再度説明中になる
		if (m_pFlower->IsColision())
		{
			m_isOperation = false;
			m_tutorialCount++;
		}
	}
	// チュートリアルのカウンタが7になると敵キャラクターが現れる
	if (m_tutorialCount == 7)
	{
		m_isEnemy = true;
	}
	// チュートリアルのカウンタが11になるとアイテムが現れる
	if (m_tutorialCount == 11)
	{
		m_isItem = true;
	}
	// チュートリアルのカウンタが16になると再度プレイヤーが動かせるようになる
	if (m_tutorialCount == 16)
	{
		m_isOperation = true;
		m_pFlower->RePosition(kChipWidth * 55, kChipHeight * 10);
		m_pFlower->Update();

		// 花に触れると説明中になる
		if (m_pFlower->IsColision())
		{
			m_isOperation = false;
			m_isEnemy = false;
			m_isItem = false;
			m_tutorialCount++;
		}
	}
	// チュートリアルのカウンタが20になると再度プレイヤーが動かせるようになる
	if (m_tutorialCount >= 20)
	{
		m_isOperation = true;
		m_pFlower->RePosition(kChipWidth * 40, kChipHeight * 12);
		m_pFlower->Update();

		// 花に触れると移動イベントに移る
		if (m_pFlower->IsColision())
		{
			m_isOperation = false;
			m_isEvent = true;
		}
	}

}

void Tutorial02::MoveEvent()
{
	if (!m_isEvent)return;
		if (!CheckSoundMem(m_moveSeHandle))
		{
			PlaySoundMem(m_moveSeHandle, DX_PLAYTYPE_LOOP, true);
		}

		m_eventFrame++;

		if (m_eventFrame >= 30 && m_eventFrame < 60)
		{
			m_eventX -= 6;
			m_eventY += 2;
		}

		if (m_eventFrame >= 60)
		{
			m_eventX += 12;
			m_eventY -= 10;
		}

		if (m_eventX >= kScreenWidht)
		{
			m_isSceneEnd = true;
		}
}

void Tutorial02::ChipDraw()
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

void Tutorial02::BackDraw()
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

void Tutorial02::End()
{
	// SEを止める
	StopSoundMem(m_moveSeHandle);

	// SEをメモリから削除する
	DeleteSoundMem(m_pushSeHandle);
	DeleteSoundMem(m_moveSeHandle);
}

bool Tutorial02::IsSceneEnd() const
{
	return m_isSceneEnd && (m_fadeAlpha >= kFadeMaxAlpha);
}

void Tutorial02::EnemyInit()
{
	// 敵キャラクターの初期化と位置の設定
		m_pStopEnemy[0]->Init(static_cast<float>((kChipWidth * 25) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[1]->Init(static_cast<float>((kChipWidth * 26) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[2]->Init(static_cast<float>((kChipWidth * 51) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[3]->Init(static_cast<float>((kChipWidth * 52) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[4]->Init(static_cast<float>((kChipWidth * 53) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[5]->Init(static_cast<float>((kChipWidth * 30) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[6]->Init(static_cast<float>((kChipWidth * 31) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[7]->Init(static_cast<float>((kChipWidth * 32) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[8]->Init(static_cast<float>((kChipWidth * 33) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[9]->Init(static_cast<float>((kChipWidth * 34) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[10]->Init(static_cast<float>((kChipWidth * 35) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[11]->Init(static_cast<float>((kChipWidth * 36) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[12]->Init(static_cast<float>((kChipWidth * 37) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[13]->Init(static_cast<float>((kChipWidth * 38) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[14]->Init(static_cast<float>((kChipWidth * 39) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[15]->Init(static_cast<float>((kChipWidth * 40) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[16]->Init(static_cast<float>((kChipWidth * 41) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[17]->Init(static_cast<float>((kChipWidth * 42) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[18]->Init(static_cast<float>((kChipWidth * 43) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[19]->Init(static_cast<float>((kChipWidth * 44) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[20]->Init(static_cast<float>((kChipWidth * 45) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[21]->Init(static_cast<float>((kChipWidth * 46) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[22]->Init(static_cast<float>((kChipWidth * 47) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[23]->Init(static_cast<float>((kChipWidth * 48) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[24]->Init(static_cast<float>((kChipWidth * 49) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[25]->Init(static_cast<float>((kChipWidth * 50) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pStopEnemy[26]->Init(static_cast<float>((kChipWidth * 31) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 6)));
		m_pStopEnemy[27]->Init(static_cast<float>((kChipWidth * 33) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 6)));
		m_pStopEnemy[28]->Init(static_cast<float>((kChipWidth * 35) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 6)));
		m_pStopEnemy[29]->Init(static_cast<float>((kChipWidth * 37) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 6)));

}

void Tutorial02::ItemInit()
{
	// アイテムの初期化と位置の設定
		m_pItem[0]->Init(static_cast<float>((kChipWidth * 32) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 6)));
		m_pItem[1]->Init(static_cast<float>((kChipWidth * 34) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 6)));
		m_pItem[2]->Init(static_cast<float>((kChipWidth * 36) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 6)));
		m_pItem[3]->Init(static_cast<float>((kChipWidth * 23) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
		m_pItem[4]->Init(static_cast<float>((kChipWidth * 29) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)));
}

void Tutorial02::SetStopEnemy(std::shared_ptr<StopEnemy> pStopEnemy, int i)
{
	m_pStopEnemy[i] = pStopEnemy;
}

void Tutorial02::SetItem(std::shared_ptr<RecoveryItem> pItem, int i)
{
	m_pItem[i] = pItem;
}
