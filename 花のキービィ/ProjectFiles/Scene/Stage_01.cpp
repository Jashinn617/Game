#include "Stage_01.h"
#include "../Game.h"
#include "../util/Input.h"
#include "DxLib.h"
#include "../Player.h"
#include "../Attack/Vomit.h"
#include "../Attack/AirBullet.h"
#include "../Attack/Inhale.h"
#include "../Camera.h"
#include "../Enemy/StopEnemy.h"
#include "../Enemy/WalkEnemy.h"
#include "../Enemy/FlyEnemy.h"
#include "../UI.h"
#include "../Object/Door.h"
#include "../Object//RecoveryItem.h"
#include "GameOverScene.h"

namespace
{
	// マップの広さ
	constexpr int kMapWidth = Game::kScreenWidth * 5;
	constexpr int kMapHeight = Game::kScreenHeight;

	// 背景の数
	constexpr int kBackBgNum = 5;

	// マップチップの情報
	constexpr int kChipWidth = 64;
	constexpr int kChipHeight = 64;

	// 背景の大きさ
	constexpr int kBackWidth = Game::kScreenWidth;
	constexpr int kBackHeight = Game::kScreenHeight;

	// チップを置く数
	constexpr int kChipNumX = (Game::kScreenWidth / kChipWidth) * 5;
	constexpr int kChipNumY = (Game::kScreenHeight / kChipHeight);

	// プレイヤーの初期座標
	constexpr int kStartX = 50;
	constexpr int kStartY = 300;

	// エネミーの数
	constexpr int kStopEnemyNum = 30;
	constexpr int kWalkEnemyNum = 15;
	constexpr int kFlyEnemyNum = 10;

	// アイテムの数
	constexpr int kItemNum = 5;

	// プレイヤーがミスをしたときの演出フレーム
	constexpr int kMissSe02Frame = 60;
	constexpr int kMissFrameMax = 60 * 2;
	constexpr float kMissFadeFrame = 60 * 2.2;
	constexpr float kContinueFrame = 60 * 2.6;

	// 画面の大きさ
	constexpr int kScreenWidht = Game::kScreenWidth;
	constexpr int kScreenHeight = Game::kScreenHeight;

	// ドアの位置
	constexpr float kDoorX = static_cast<float>(kChipWidth) * 149.0f - static_cast<float>(kChipWidth) * 0.5f;
	constexpr float kDoorY = static_cast<float>(kChipHeight) * 5.0f;

	// BGMの最大値
	constexpr int kBgmVolumeMax = 200;

	// フェードの速度
	constexpr int kMissFadeSpeed = 20;
	constexpr int kSceneChangeFadeSpeed = 12;
	// フェードの最大Alpha数
	constexpr int kFadeMaxAlpha = 255;

	// 画面の揺れ幅
	constexpr int kScreenShake = 16;

	// 画面が揺れる時間
	constexpr int kScreenShakeFrameMax = 8;
	// フェード時の色
	constexpr unsigned int kFadeColor = 0xffffff;
	// 背景にかぶせる色
	constexpr unsigned int kBackColor = 0xff0000;
}

Stage_01::Stage_01() :
	m_gameScreenHandle(-1),
	m_backgroundHandle(-1),
	m_chipHandle(-1),
	m_graphChipNumX(0),
	m_graphChipNumY(0),
	m_bgmHandle(-1),
	m_missSeHandle(-1),
	m_moveSeHandle(-1),
	m_missSeHandle2(-1),
	m_fadeAlpha(255),
	m_backAlpha(50),
	m_ismissFade(false),
	m_shakeFrame(0),
	m_nextScene(kStage02),
	m_isMiss(false),
	m_isSceneEnd(false)
{
	// ゲーム画面描画先の生成
	m_gameScreenHandle = MakeScreen(kScreenWidht, kScreenHeight, true);
	/*画像のロード*/
	m_backgroundHandle = LoadGraph("data/Map/Stage_01Bg.png");
	m_chipHandle = LoadGraph("data/Map/map.png");

	/*敵の数を決める*/
	m_pStopEnemy.resize(kStopEnemyNum);
	m_pWalkEnemy.resize(kWalkEnemyNum);
	m_pFlyEnemy.resize(kFlyEnemyNum);
	/*アイテムの数を決める*/
	m_pRecoveryItem.resize(kItemNum);

	/*マップチップの数を数える*/
	int graphW = 0;
	int graphH = 0;
	GetGraphSize(m_chipHandle, &graphW, &graphH);
	m_graphChipNumX = graphW / kChipWidth;
	m_graphChipNumY = graphH / kChipHeight;

	// マップチップの配置データの初期化
	m_chipData = {
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,01,01, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,01,01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,01,01,01,01,01, 0, 0, 0, 0, 0, 0, 0,02,02, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,01,01,01,01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,01,01,01,01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,01, 0, 0, 0, 0, 0, 0, 0, 0, 0,01,01,01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,01,01, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,01,01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02, 0, 0,01,01,01,01},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,01,01, 0, 0,01,01, 0, 0, 0, 0, 0, 0, 0,02,02,02,02,01,01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02, 0, 0,02,02,02,02},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0,01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,01,01,01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02, 0, 0, 0, 0,01,01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,01,01,01,01,01, 0, 0,02,02,02, 0, 0, 0, 0, 0, 0, 0,01,01, 0, 0, 0, 0, 0, 0,01, 0, 0, 0, 0, 0, 0,02,02,02,02,02,02,01,01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,01,01,02,02, 0, 0,02,02,02,02},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02, 0, 0, 0, 0, 0,01,01,01, 0, 0, 0,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02, 0, 0, 0, 0,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,01, 0, 0, 0, 0, 0,02,02,02,02,02,02,02,02,01,01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02,02},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0,02, 0, 0,01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02, 0, 0, 0, 0, 0,02,02,02, 0, 0, 0,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02, 0, 0, 0, 0,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02,02,02,02,02,02,02,02,01,01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02,02},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0,02, 0, 0,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02, 0, 0, 0, 0, 0,02,02,02, 0, 0, 0,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02, 0, 0, 0, 0,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02,02,02,02,02,02,02,02,02,02,01,01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02,02},
		{01,01,01,01,01,01, 0, 0,01,02, 0, 0,02,01,01,01,01,01,01,01,01, 0, 0,01,01,01,01,01,01,02,02,02,01,01,01,01,01,02,02,02,01,01,01,02, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0,02,01,01,01,01,02,02, 0, 0, 0,01,01,01,01,01, 0, 0,01,01,01,01,01,01, 0, 0,01,01,01,01,01,01,01,01,01,02,02,02, 0, 0, 0, 0,01,01,01,01,01,01,01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02,02,02,02,02,02,02,02,02,02,02,02,01,01,01,01,01,01,01,01,01,01,01,01,01, 0, 0, 0, 0,02,02,02,02},
		{02,02,02,02,02,02, 0, 0,02,02, 0, 0,02,02,02,02,02,02,02,02,02, 0, 0,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02,02,02,02,02, 0, 0, 0,02,02,02,02,02, 0, 0,02,02,02,02,02,02, 0, 0,02,02,02,02,02,02,02,02,02,02,02,02, 0, 0, 0, 0,02,02,02,02,02,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02, 0, 0, 0, 0,02,02,02,02},
		{02,02,02,02,02,02, 0, 0,02,02, 0, 0,02,02,02,02,02,02,02,02,02, 0, 0,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02,02,02,02,02, 0, 0, 0,02,02,02,02,02, 0, 0,02,02,02,02,02,02, 0, 0,02,02,02,02,02,02,02,02,02,02,02,02, 0, 0, 0, 0,02,02,02,02,02,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02, 0, 0, 0, 0,02,02,02,02},
		{02,02,02,02,02,02, 0, 0,02,02, 0, 0,02,02,02,02,02,02,02,02,02, 0, 0,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02,02,02,02,02, 0, 0, 0,02,02,02,02,02, 0, 0,02,02,02,02,02,02, 0, 0,02,02,02,02,02,02,02,02,02,02,02,02, 0, 0, 0, 0,02,02,02,02,02,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02, 0, 0, 0, 0,02,02,02,02},
		{02,02,02,02,02,02, 0, 0,02,02, 0, 0,02,02,02,02,02,02,02,02,02, 0, 0,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02,02,02,02,02, 0, 0, 0,02,02,02,02,02, 0, 0,02,02,02,02,02,02, 0, 0,02,02,02,02,02,02,02,02,02,02,02,02, 0, 0, 0, 0,02,02,02,02,02,02,02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02, 0, 0, 0, 0,02,02,02,02},
	};	
}

Stage_01::~Stage_01()
{
	/*グラフィックのデリート*/
	DeleteGraph(m_gameScreenHandle);
	DeleteGraph(m_backgroundHandle);
	DeleteGraph(m_chipHandle);
}

void Stage_01::Init()
{
	// マップの広さを決める
	m_pCamera->StartHorizon();
	m_pCamera->Init();

	// プレイヤーのHPと残機を全回復する
	m_pPlayer->CompleteRecoveryHp();
	m_pPlayer->CompleteRecoveryLives();

	// 初期化
	RestartInit();
	// ドアの位置を設定する
	m_pDoor->Init(kDoorX, kDoorY);
	// ゲームオーバーシーンのコンテニュー時に戻るシーンを設定する
	m_pGameOver->ChangeNextScene(m_pGameOver->GetStage01());

	/*BGMのロード*/
	m_bgmHandle = LoadSoundMem(_T("data/Sound/BGM/Stage_01.wav"));
	// BGMの音量設定
	ChangeVolumeSoundMem(kBgmVolumeMax, m_bgmHandle);

	/*SEのロード*/
	m_missSeHandle = LoadSoundMem(_T("data/Sound/SE/Miss.wav"));
	m_missSeHandle2 = LoadSoundMem(_T("data/Sound/SE/Miss02.mp3"));
	m_moveSeHandle = LoadSoundMem(_T("data/Sound/SE/Move.wav"));

	// BGMの再生
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP, true);

	// シーンが終了しないようにする
	m_isSceneEnd = false;
}

void Stage_01::Update(Input& input)
{
	if (!m_isSceneEnd)
	{
		// プレイヤーがミスをした際はプレイヤー以外が止まる
		if (!m_isMiss)
		{
			EnemyUpdate();
			ItemUpdate();
			m_pDoor->Update();
			m_pUI->Update();
		}
		// プレイヤーにマップチップのデータを渡しておく
		m_pPlayer->Update(input, m_chipData, kChipNumX, kChipNumY, kChipWidth, kChipHeight);

		// 画面揺れの処理
		m_shakeFrame--;
		if (m_shakeFrame <= 0) m_shakeFrame = 0;

		// ミス
		OnMiss();

		// 残機が0になったらゲームオーバー
		if (m_pPlayer->GetRemainingLives() < 0)
		{
			// 次のシーンをゲームオーバーシーンにする
			m_nextScene = kGameOver;
			m_isSceneEnd = true;
		}

		// プレイヤーがミスをした時のフェード処理
		if (m_ismissFade)// フェードイン
		{
			m_fadeAlpha += kMissFadeSpeed;
			if (m_fadeAlpha > kFadeMaxAlpha)
			{
				m_fadeAlpha = kFadeMaxAlpha;
				m_ismissFade = false;
			}
		}
		else // フェードアウト
		{
			m_fadeAlpha -= kMissFadeSpeed;
			if (m_fadeAlpha < 0)
			{
				m_fadeAlpha = 0;
			}
		}

		// プレイヤーが扉に入ったら次のシーンに移行する
		if (m_pDoor->IsColision())
		{
			if (input.IsTriggered("up"))
			{
				// SEを鳴らす
				PlaySoundMem(m_moveSeHandle, DX_PLAYTYPE_BACK, true);
				// 次のシーンをステージ2にする
				m_nextScene = kStage02;
				m_isSceneEnd = true;
			}
		}

		// フェードアウト
		m_fadeAlpha -= kSceneChangeFadeSpeed;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
		}
	}
	// 場面転換のフェード
	else // フェードイン
	{
		m_fadeAlpha += kSceneChangeFadeSpeed;
		if (m_fadeAlpha > kFadeMaxAlpha)
		{
			m_fadeAlpha = kFadeMaxAlpha;
		}
	}

#ifdef _DEBUG
	// Sキーでゲームオーバーに飛ぶ
	if (input.IsTriggered("S"))
	{
		m_nextScene = kGameOver;
		m_isSceneEnd = true;
	}
	// Pキーで次のシーンに飛ぶ
	else if (input.IsTriggered("P"))
	{
		PlaySoundMem(m_moveSeHandle, DX_PLAYTYPE_BACK, true);
		m_nextScene = kStage02;
		m_isSceneEnd = true;
	}
#endif
}

void Stage_01::Draw()
{
	// 自分で生成したグラフィックデータに対して書き込みを行う
	SetDrawScreen(m_gameScreenHandle);

	// 描画先スクリーンをクリアする
	ClearDrawScreen();

	// 背景の描画
	BackDraw();
	// マップチップの描画
	ChipDraw();
	
	// ドアの描画
	m_pDoor->Draw();

	// 敵キャラクターの描画
	EnemyDraw();

	// アイテムの描画
	ItemDraw();

	// プレイヤーの描画
	m_pPlayer->Draw();

	// UIの描画
	m_pUI->Draw();

	// バックバッファに書き込む設定に戻しておく
	SetDrawScreen(DX_SCREEN_BACK);

	// ゲーム画面をバックバッファに描画する
	int screenX = 0;
	int screenY = 0;
	if (m_shakeFrame > 0)
	{
		// 画面を揺らす
		screenX = GetRand(kScreenShake) - (kScreenShake / 2);
		screenY = GetRand(kScreenShake) - (kScreenShake / 2);
	}

	// スクリーンの描画
	DrawGraph(screenX, screenY, m_gameScreenHandle, true);

	// フェードの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);
	DrawBox(0, 0, kScreenWidht, kScreenHeight, kFadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}

void Stage_01::End()
{
	// BGMを止める
	StopSoundMem(m_bgmHandle);
	// BGMをメモリから削除する
	DeleteSoundMem(m_bgmHandle);
	// SEをメモリから削除する
	DeleteSoundMem(m_missSeHandle);
	DeleteSoundMem(m_missSeHandle2);
	DeleteSoundMem(m_moveSeHandle);
}

bool Stage_01::IsSceneEnd() const
{
	// フェードがおわってからシーンを移動する
	return m_isSceneEnd && (m_fadeAlpha >= kFadeMaxAlpha);
}

void Stage_01::RestartInit()
{
	/*プレイヤーの初期化*/
	m_pPlayer->Init();
	// 初期座標の設定
	m_pPlayer->SetStartPos(kStartX, kStartY);
	// 重力の初期化
	m_pPlayer->GravityReset();

	m_pCamera->Init();
	m_pUI->Init();

	// 敵キャラクターの初期化
	EnemyInit();
	// アイテムの初期化
	ItemInit();

	// BGMの再生
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP, true);
	// 画面揺れのフレームをゼロに戻す
	m_shakeFrame = 0;
}

void Stage_01::EnemyInit()
{
	/*初期化と座標の設定*/

	// 止まっている敵
	{
		m_pStopEnemy[0]->Init(static_cast<float>((kChipWidth * 6) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 11)));
		m_pStopEnemy[1]->Init(static_cast<float>((kChipWidth * 10) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 7)));
		m_pStopEnemy[2]->Init(static_cast<float>((kChipWidth * 13) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 9)));
		m_pStopEnemy[3]->Init(static_cast<float>((kChipWidth * 16) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 11)));
		m_pStopEnemy[4]->Init(static_cast<float>((kChipWidth * 17) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 11)));
		m_pStopEnemy[5]->Init(static_cast<float>((kChipWidth * 17) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 5)));
		m_pStopEnemy[6]->Init(static_cast<float>((kChipWidth * 19) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 5)));
		m_pStopEnemy[7]->Init(static_cast<float>((kChipWidth * 32) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 7)));
		m_pStopEnemy[8]->Init(static_cast<float>((kChipWidth * 34) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 11)));
		m_pStopEnemy[9]->Init(static_cast<float>((kChipWidth * 36) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 11)));
		m_pStopEnemy[10]->Init(static_cast<float>((kChipWidth * 38) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 8)));
		m_pStopEnemy[11]->Init(static_cast<float>((kChipWidth * 40) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 8)));
		m_pStopEnemy[12]->Init(static_cast<float>((kChipWidth * 44) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 7)));
		m_pStopEnemy[13]->Init(static_cast<float>((kChipWidth * 57) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 5)));
		m_pStopEnemy[14]->Init(static_cast<float>((kChipWidth * 63) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 7)));
		m_pStopEnemy[15]->Init(static_cast<float>((kChipWidth * 68) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 5)));
		m_pStopEnemy[16]->Init(static_cast<float>((kChipWidth * 85) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 7)));
		m_pStopEnemy[17]->Init(static_cast<float>((kChipWidth * 87) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 7)));
		m_pStopEnemy[18]->Init(static_cast<float>((kChipWidth * 91) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 7)));
		m_pStopEnemy[19]->Init(static_cast<float>((kChipWidth * 93) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 7)));
		m_pStopEnemy[20]->Init(static_cast<float>((kChipWidth * 104) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 6)));
		m_pStopEnemy[21]->Init(static_cast<float>((kChipWidth * 106) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 5)));
		m_pStopEnemy[22]->Init(static_cast<float>((kChipWidth * 108) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 6)));
		m_pStopEnemy[23]->Init(static_cast<float>((kChipWidth * 116) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 4)));
		m_pStopEnemy[24]->Init(static_cast<float>((kChipWidth * 120) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 6)));
		m_pStopEnemy[25]->Init(static_cast<float>((kChipWidth * 133) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 4)));
		m_pStopEnemy[26]->Init(static_cast<float>((kChipWidth * 141) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 7)));
		m_pStopEnemy[27]->Init(static_cast<float>((kChipWidth * 142) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 11)));
		m_pStopEnemy[28]->Init(static_cast<float>((kChipWidth * 143) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 3)));
		m_pStopEnemy[29]->Init(static_cast<float>((kChipWidth * 147) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 5)));
	}
	// 歩く敵
	{
		m_pWalkEnemy[0]->Init(static_cast<float>((kChipWidth * 14) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 11)), 220);
		m_pWalkEnemy[1]->Init(static_cast<float>((kChipWidth * 24) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 11)), 150);
		m_pWalkEnemy[2]->Init(static_cast<float>((kChipWidth * 35) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 5)), 100);
		m_pWalkEnemy[4]->Init(static_cast<float>((kChipWidth * 41) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 11)), 60);
		m_pWalkEnemy[5]->Init(static_cast<float>((kChipWidth * 58) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 11)), 95);
		m_pWalkEnemy[6]->Init(static_cast<float>((kChipWidth * 67) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 11)), 120);
		m_pWalkEnemy[7]->Init(static_cast<float>((kChipWidth * 74) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 11)), 170);
		m_pWalkEnemy[8]->Init(static_cast<float>((kChipWidth * 82) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 11)), 250);
		m_pWalkEnemy[9]->Init(static_cast<float>((kChipWidth * 98) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 11)), 190);
		m_pWalkEnemy[10]->Init(static_cast<float>((kChipWidth * 130) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 11)), 190);
		m_pWalkEnemy[11]->Init(static_cast<float>((kChipWidth * 133) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 11)), 190);
		m_pWalkEnemy[12]->Init(static_cast<float>((kChipWidth * 136) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 11)), 190);
		m_pWalkEnemy[13]->Init(static_cast<float>((kChipWidth * 131) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 4)), 140);
		m_pWalkEnemy[14]->Init(static_cast<float>((kChipWidth * 143) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 3)), 30);
	}
	// 飛ぶ敵
	{
		m_pFlyEnemy[0]->Init(static_cast<float>((kChipWidth * 10) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 2), 90);
		m_pFlyEnemy[1]->Init(static_cast<float>((kChipWidth * 30) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 3), 500);
		m_pFlyEnemy[2]->Init(static_cast<float>((kChipWidth * 60) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 3), 200);
		m_pFlyEnemy[3]->Init(static_cast<float>((kChipWidth * 80) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 4), 70);
		m_pFlyEnemy[4]->Init(static_cast<float>((kChipWidth * 110) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 4), 300);
		m_pFlyEnemy[5]->Init(static_cast<float>((kChipWidth * 132) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 3), 260);
		m_pFlyEnemy[6]->Init(static_cast<float>((kChipWidth * 70) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 2), 30);
		m_pFlyEnemy[7]->Init(static_cast<float>((kChipWidth * 105) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 4), 500);
		m_pFlyEnemy[8]->Init(static_cast<float>((kChipWidth * 55) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 6), 50);
		m_pFlyEnemy[9]->Init(static_cast<float>((kChipWidth * 30) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 3), 200);
	}
}

void Stage_01::ItemInit()
{
	/*初期化と座標の設定*/

	// 回復アイテム
	{
		m_pRecoveryItem[0]->Init(static_cast<float>((kChipWidth * 5) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 11));
		m_pRecoveryItem[1]->Init(static_cast<float>((kChipWidth * 58) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 11));
		m_pRecoveryItem[2]->Init(static_cast<float>((kChipWidth * 103) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 11));
		m_pRecoveryItem[3]->Init(static_cast<float>((kChipWidth * 77) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 11));
		m_pRecoveryItem[4]->Init(static_cast<float>((kChipWidth * 135) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 11));
	}
}

void Stage_01::EnemyUpdate()
{
	// 止まっている敵
	for (int i = 0; i < m_pStopEnemy.size(); i++)
	{
		m_pStopEnemy[i]->Update();
	}
	// 歩いている敵
	for (int i = 0; i < m_pWalkEnemy.size(); i++)
	{
		m_pWalkEnemy[i]->Update();
	}
	// 飛んでいる敵
	for (int i = 0; i < m_pFlyEnemy.size(); i++)
	{
		m_pFlyEnemy[i]->Update();
	}
}

void Stage_01::ItemUpdate()
{
	// 回復アイテム
	for (int i = 0; i < m_pRecoveryItem.size(); i++)
	{
		m_pRecoveryItem[i]->Update();
	}
}

void Stage_01::BackDraw()
{
	// プレイヤーの位置に応じたスクロール量を決定する
	int scrollX = m_pCamera->GetScrollX();
	int scrollY = m_pCamera->GetScrollY();

	// 背景の描画
	DrawBox(0, 0, kMapWidth, kMapHeight, 0xffffbc, true);

	// 背景のスクロール
	for (int i = 0; i < kBackBgNum; i++)
	{
		int posX = kBackWidth * i - scrollX;
		int posY = scrollY;

		DrawGraph(posX, posY, m_backgroundHandle, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_backAlpha);
	DrawBox(0, 0, kMapWidth, kMapHeight, kBackColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}

void Stage_01::ChipDraw()
{
	// プレイヤーの位置に応じたスクロール量を決定する
	int scrollX = m_pCamera->GetScrollX();
	int scrollY = m_pCamera->GetScrollY();

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

			DrawRectGraph(posX, posY,
				srcX, srcY,
				kChipWidth, kChipHeight,
				m_chipHandle, true);
		}
	}
}

void Stage_01::EnemyDraw()
{
	// 止まっている敵
	for (int i = 0; i < m_pStopEnemy.size(); i++)
	{
		m_pStopEnemy[i]->Draw();
	}
	// 歩いている敵
	for (int i = 0; i < m_pWalkEnemy.size(); i++)
	{
		m_pWalkEnemy[i]->Draw();
	}
	// 飛んでいる敵
	for (int i = 0; i < m_pFlyEnemy.size(); i++)
	{
		m_pFlyEnemy[i]->Draw();
	}
}

void Stage_01::ItemDraw()
{
	// 回復アイテム
	for (int i = 0; i < m_pRecoveryItem.size(); i++)
	{
		m_pRecoveryItem[i]->Draw();
	}
}

void Stage_01::OnMiss()
{
	// プレイヤーがミスをした場合は残機を減らしてHPを全回復して初期位置に戻す
	// 落ちた場合とHPがゼロになった場合で演出が違うため別々で処理を作る
	// 落ちた場合の処理
	if ((m_pPlayer->GetBottom() >= kMapHeight) || (m_pPlayer->GetHp() <= 0))
	{
		// 落ちた場合
		if ((m_pPlayer->GetBottom() >= kMapHeight) && !m_isMiss)
		{
			// 画面を揺らす
			// 一度画面が揺れたらもう一度揺れないようにする
			m_shakeFrame = kScreenShakeFrameMax;
			m_pPlayer->FellMiss();
		}
		else if ((m_pPlayer->GetHp() <= 0) && !m_isMiss)// HPがゼロになった場合
		{
			m_shakeFrame = kScreenShakeFrameMax;
			m_pPlayer->DamageMiss();
		}

		// BGMを止める
		StopSoundMem(m_bgmHandle);

		// SEを鳴らす
		if (!CheckSoundMem(m_missSeHandle2) && !m_isMiss)
		{
			PlaySoundMem(m_missSeHandle2, DX_PLAYTYPE_BACK, true);
		}
		m_pPlayer->OnMiss();

		// 画面を止める
		m_isMiss = true;

	}
	// ミスをして少し経ってからミス演出用のSEを鳴らす
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
		m_ismissFade = true;
	}
	// リスタート
	if (m_pPlayer->GetMissFrame() >= kContinueFrame)
	{
		// プレイヤーの残機を減らしてHPは全回復する
		// プレイヤーやエネミーの位置を初期化する
		m_pPlayer->DeleteLives();
		m_pPlayer->CompleteRecoveryHp();
		RestartInit();
		m_isMiss = false;
	}
}

