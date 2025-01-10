#include "Stage_02.h"
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
#include "../Object/Needle.h"
#include "../Object/RecoveryItem.h"
#include "../Object/Door.h"
#include "GameOverScene.h"

#include <cmath>

namespace
{
	// 画面の大きさ
	constexpr int kScreenWidht = Game::kScreenWidth;
	constexpr int kScreenHeight = Game::kScreenHeight;

	// マップの広さ
	constexpr int kMapWidth = Game::kScreenWidth;
	constexpr int kMapHeight = Game::kScreenHeight * 5;

	// マップチップの情報
	constexpr int kChipWidth = 64;
	constexpr int kChipHeight = 64;

	// 背景の大きさ
	constexpr int kBackWidth = Game::kScreenWidth;
	constexpr int kBackHeight = Game::kScreenHeight;

	// チップを置く数
	constexpr int kChipNumX = (Game::kScreenWidth / kChipWidth);
	constexpr int kChipNumY = (Game::kScreenHeight / kChipHeight) * 5;

	// プレイヤーの初期座標
	constexpr float kStartX = static_cast<float>((kChipWidth * 3) - (kChipWidth * 0.5f));
	constexpr float kStartY = static_cast<float>((kChipHeight * 78));

	// ゴールの位置
	constexpr float kGoalPosX = static_cast<float>((kChipWidth * 15) - (kChipWidth * 0.5f));
	constexpr float kGoalPosY = static_cast<float>((kChipHeight * 4));

	// ダンサーの数
	constexpr int kDancerNum = 3;

	// ダンスの切り取りサイズ
	constexpr int kDancerWidth1 = 72;
	constexpr int kDanceHeight1 = 64;
	constexpr int kDancerWidth2 = 43;
	constexpr int kDanceHeight2 = 48;

	// ダンスの初期位置
	constexpr int kStartDancePosX = 500;
	constexpr int kStartDancePosY = kScreenHeight / 2;

	// 移動イベントの初期位置
	constexpr int kStartMoveEventX = 500;
	constexpr int kStartMoveEventY = 500;

	// ダンスのアニメーション
	constexpr int kUseFrame[]{ 0,1,0 };
	// 1コマのフレーム数
	constexpr int kAnimFrameNum = 12;
	// １サイクルのフレーム数
	constexpr int kAnimFrameCycle = _countof(kUseFrame) * kAnimFrameNum;

	// エネミーの数
	constexpr int kStopEnemyNum = 30;
	constexpr int kWalkEnemyNum = 15;
	constexpr int kFlyEnemyNum = 10;

	// 針の数
	constexpr int kNeedleNum = 48;

	// アイテムの数
	constexpr int kItemNum = 5;

	// プレイヤーがミスをしたときの演出フレーム
	constexpr int kMissSe02Frame = 60;
	constexpr int kMissFrameMax = 60 * 2;
	constexpr float kMissFadeFrame = 60 * 2.2;
	constexpr float kContinueFrame = 60 * 2.6;

	// BGMの最大値
	constexpr int kBgmVolumeMax = 200;

	// スクロール値
	constexpr float kScrollValue = 1.0f;

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
	
}

Stage_02::Stage_02() :
	m_gameScreenHandle(-1),
	m_background(-1),
	m_foreground(-1),
	m_chipHandle(-1),
	m_moveFlowerHandle(-1),
	m_graphChipNumX(0),
	m_graphChipNumY(0),
	m_bgmHandle(-1),
	m_missSeHandle(-1),
	m_missSeHandle02(-1),
	m_goalSeHandle(-1),
	m_moveSeHandle(-1),
	m_goalSeVolume(0),
	m_fadeAlpha(0),
	m_isMissFade(false),
	m_shakeFrame(0),
	m_nextScene(kLastStage),
	m_isMiss(false),
	m_scrollX(0),
	m_fgW(0),
	m_fgH(0),
	m_isGoal(false),
	m_ExtRate(1),
	m_moveEventX(kStartMoveEventX),
	m_moveEventY(kStartMoveEventY),
	m_moveEventFrame(0),
	m_isSceneEnd(false)
{
	// ゲーム画面描画先の生成
	m_gameScreenHandle = MakeScreen(kScreenWidht, kScreenHeight, true);
	/*画像のロード*/
	m_chipHandle = LoadGraph(_T("data/Map/map.png"));
	m_background = LoadGraph(_T("data/Map/Stage_02Bg.png"));
	m_foreground = LoadGraph(_T("data/Map/Stage_02Fg.png"));
	m_moveFlowerHandle = LoadGraph(_T("data/Event/MoveFlower.png"));

	/*敵の数を決める*/
	m_pStopEnemy.resize(kStopEnemyNum);
	m_pWalkEnemy.resize(kWalkEnemyNum);
	m_pFlyEnemy.resize(kFlyEnemyNum);
	// 針の数を決める
	m_pNeedle.resize(kNeedleNum);
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
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{04, 0, 0, 0, 0,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04,04, 0,04, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04,04,04,04, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0,04,04, 0,04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0,04,04,04,04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04, 0, 0, 0,04,04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04,04,04,04,04,04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0,04, 0,04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0,04,04,04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04, 0, 0,04,04, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04,04,04,04,04, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04,04,04,04,04,04,04,04,04,04,04,04, 0, 0, 0, 0, 0,04,04,04,04,04,04,04,04,04,04,04,04,04},
		{04,04,04,04,04,04,04,04, 0, 0, 0,04, 0, 0, 0, 0, 0,04,04,04,04,04,04,04,04,04,04,04,04,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04,04,04,04,04, 0, 0, 0,04,04,04,04,04, 0, 0, 0,04,04,04,04,04,04,04,04,04,04,04,04,04,04},
		{04,04,04,04,04, 0, 0, 0,04,04,04,04,04,04,04,04,04,04,04,04, 0, 0, 0,04,04,04,04,04,04,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04, 0, 0, 0,04,04,04,04,04,04,04},
		{04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04, 0, 0,04, 0, 0, 0,04,04,04,04,04,04,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0,04,04,04, 0, 0, 0, 0,04,04,04, 0, 0, 0, 0,04,04,04,04,04,04,04,04,04,04,04,04},
		{04, 0, 0, 0,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04, 0, 0, 0,04,04,04,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04, 0, 0, 0,04,04,04,04},
		{04,04,04,04,04,04, 0, 0, 0,04,04,04,04,04,04,04,04,04,04,04,04,04,04, 0, 0, 0,04,04,04,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04,04,04,04,04,04, 0, 0, 0, 0,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04},
		{04,04,04,04,04,04, 0, 0, 0, 0,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04,04,04,04,04,04, 0, 0, 0, 0,04,04,04,04,04, 0, 0, 0,04,04,04,04,04,04,04,04,04,04,04,04},
		{04, 0, 0, 0,04,04,04,04,04,04,04,04,04,04,04, 0, 0, 0,04,04,04,04,04,04,04,04,04,04,04,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04},
		{04, 0, 0, 0,04,04,04,04,04,04,04,04,04,04,04,04,04,04, 0, 0, 0,04,04,04,04,04, 0, 0,04,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04, 0, 0, 0,04},
		{04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,04},
		{04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04, 0, 0, 0,04},
		{04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04,04}
	};
}

Stage_02::~Stage_02()
{
	/*グラフィックのデリート*/
	DeleteGraph(m_gameScreenHandle);
	DeleteGraph(m_background);
	DeleteGraph(m_foreground);
	DeleteGraph(m_chipHandle);
	DeleteGraph(m_moveFlowerHandle);
}

void Stage_02::Init()
{

	// マップの広さを決める
	m_pCamera->StartVertical();

	// 初期化
	RestartInit();
	// ドアの初期化
	m_pDoor->Init(kGoalPosX, kGoalPosY);

	// 針の初期化
	NeedleInit();

	/*BGMのロード*/
	m_bgmHandle = LoadSoundMem(_T("data/Sound/BGM/Stage_02.mp3"));
	
	/*SEのロード*/
	m_missSeHandle = LoadSoundMem(_T("data/Sound/SE/Miss.wav"));
	m_missSeHandle02 = LoadSoundMem(_T("data/Sound/SE/Miss02.mp3"));
	m_goalSeHandle = LoadSoundMem(_T("data/Sound/SE/Goal.wav"));
	m_moveSeHandle = LoadSoundMem(_T("data/Sound/SE/Move.wav"));

	// BGMの再生
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP, true);

	// SEの再生
	PlaySoundMem(m_goalSeHandle, DX_PLAYTYPE_LOOP, true);

	// シーンが終了しないようにする
	m_isSceneEnd = false;
	m_isGoal = false;
	m_isMiss = false;
}

void Stage_02::Update(Input& input)
{
	// スクロール
	m_scrollX += kScrollValue;

	VolumeSetting();

	if (!m_isSceneEnd)
	{
		// プレイヤーがミスをした場合はプレイヤー以外の処理を止める
		if (!m_isMiss || !m_isGoal)
		{
			m_pDoor->Update();
			EnemyUpdate();
			NeedleUpdate();
			ItemUpdate();
			m_pUI->Update();
		}

		if (!m_isGoal)
		{
			// プレイヤーにマップチップのデータを渡しておく
			m_pPlayer->Update(input, m_chipData,
				kChipNumX, kChipNumY,
				kChipWidth, kChipHeight);
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
			m_fadeAlpha += kMissFadeSpeed;
			if (m_fadeAlpha > kFadeMaxAlpha)
			{
				m_fadeAlpha = kFadeMaxAlpha;
				m_isMissFade = false;
			}
		}
		else
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
				m_nextScene = kLastStage;
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
	if (input.IsTriggered("S"))
	{
		m_nextScene = kGameOver;
		m_isSceneEnd = true;
	}
	else if (input.IsTriggered("P"))
	{
		m_nextScene = kLastStage;
		m_isSceneEnd = true;
	}
#endif
}

void Stage_02::Draw()
{
	// 自分で生成したグラフィックデータに対して書き込みを行う
	SetDrawScreen(m_gameScreenHandle);

	// 描画先スクリーンをクリアする
	ClearDrawScreen();

	// 背景の描画
	BackDraw();
	// マップチップの描画
	ChipDraw();
	

	// 針の描画
	NeedleDraw();

	// 敵の描画
	EnemyDraw();
	
	// アイテムの描画
	ItemDraw();	

	// ドアの描画
	m_pDoor->Draw();

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

void Stage_02::End()
{
	// BGMを止める
	StopSoundMem(m_bgmHandle);
	// BGMをメモリから削除する
	DeleteSoundMem(m_bgmHandle);
	// SEをメモリから削除する
	DeleteSoundMem(m_missSeHandle);
	DeleteSoundMem(m_missSeHandle02);
	DeleteSoundMem(m_goalSeHandle);
	DeleteSoundMem(m_moveSeHandle);
}

bool Stage_02::IsSceneEnd() const
{
	// フェードがおわってからシーンを移動する
	return m_isSceneEnd && (m_fadeAlpha >= kFadeMaxAlpha);
}

void Stage_02::RestartInit()
{
	/*プレイヤーの初期化*/
	m_pPlayer->Init();
	// 初期座標を設定
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

void Stage_02::EnemyInit()
{
	/*初期化と座標の設定*/

		// 止まっている敵
		{
			m_pStopEnemy[0]->Init(static_cast<float>((kChipWidth * 15) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 78)));
			m_pStopEnemy[1]->Init(static_cast<float>((kChipWidth * 23) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 78)));
			m_pStopEnemy[2]->Init(static_cast<float>((kChipWidth * 25) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 78)));
			m_pStopEnemy[3]->Init(static_cast<float>((kChipWidth * 8) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 73)));
			m_pStopEnemy[4]->Init(static_cast<float>((kChipWidth * 16) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 73)));
			m_pStopEnemy[5]->Init(static_cast<float>((kChipWidth * 9) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 73)));
			m_pStopEnemy[6]->Init(static_cast<float>((kChipWidth * 19) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 73)));
			m_pStopEnemy[7]->Init(static_cast<float>((kChipWidth * 20) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 73)));
			m_pStopEnemy[8]->Init(static_cast<float>((kChipWidth * 16) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 67)));
			m_pStopEnemy[9]->Init(static_cast<float>((kChipWidth * 8) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 67)));
			m_pStopEnemy[10]->Init(static_cast<float>((kChipWidth * 28) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 67)));
			m_pStopEnemy[11]->Init(static_cast<float>((kChipWidth * 25) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 63)));
			m_pStopEnemy[12]->Init(static_cast<float>((kChipWidth * 3) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 63)));
			m_pStopEnemy[13]->Init(static_cast<float>((kChipWidth * 5) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 63)));
			m_pStopEnemy[14]->Init(static_cast<float>((kChipWidth * 12) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 58)));
			m_pStopEnemy[15]->Init(static_cast<float>((kChipWidth * 4) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 58)));
			m_pStopEnemy[16]->Init(static_cast<float>((kChipWidth * 19) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 58)));
			m_pStopEnemy[17]->Init(static_cast<float>((kChipWidth * 27) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 54)));
			m_pStopEnemy[18]->Init(static_cast<float>((kChipWidth * 6) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 48)));
			m_pStopEnemy[19]->Init(static_cast<float>((kChipWidth * 19) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 48)));
			m_pStopEnemy[20]->Init(static_cast<float>((kChipWidth * 25) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 48)));
			m_pStopEnemy[21]->Init(static_cast<float>((kChipWidth * 4) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 42)));
			m_pStopEnemy[22]->Init(static_cast<float>((kChipWidth * 15) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 42)));
			m_pStopEnemy[23]->Init(static_cast<float>((kChipWidth * 26) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 42)));
			m_pStopEnemy[24]->Init(static_cast<float>((kChipWidth * 19) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 37)));
			m_pStopEnemy[25]->Init(static_cast<float>((kChipWidth * 17) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 37)));
			m_pStopEnemy[26]->Init(static_cast<float>((kChipWidth * 20) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 37)));
			m_pStopEnemy[27]->Init(static_cast<float>((kChipWidth * 9) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 30)));
			m_pStopEnemy[28]->Init(static_cast<float>((kChipWidth * 26) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 30)));
			m_pStopEnemy[29]->Init(static_cast<float>((kChipWidth * 20) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 25)));
		}

		// 歩く敵
		{
			m_pWalkEnemy[0]->Init(static_cast<float>((kChipWidth * 19) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 63)), 300);
			m_pWalkEnemy[1]->Init(static_cast<float>((kChipWidth * 11) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 58)), 500);
			m_pWalkEnemy[2]->Init(static_cast<float>((kChipWidth * 15) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 54)), 100);
			m_pWalkEnemy[4]->Init(static_cast<float>((kChipWidth * 20) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 48)), 200);
			m_pWalkEnemy[5]->Init(static_cast<float>((kChipWidth * 24) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 42)), 30);
			m_pWalkEnemy[6]->Init(static_cast<float>((kChipWidth * 3) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 42)), 550);
			m_pWalkEnemy[7]->Init(static_cast<float>((kChipWidth * 25) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 37)), 50);
			m_pWalkEnemy[8]->Init(static_cast<float>((kChipWidth * 4) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 37)), 30);
			m_pWalkEnemy[9]->Init(static_cast<float>((kChipWidth * 5) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 30)), 80);
			m_pWalkEnemy[10]->Init(static_cast<float>((kChipWidth * 18) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 30)), 200);
			m_pWalkEnemy[11]->Init(static_cast<float>((kChipWidth * 23) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 25)), 15);
			m_pWalkEnemy[12]->Init(static_cast<float>((kChipWidth * 4) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 22)), 8);
			m_pWalkEnemy[13]->Init(static_cast<float>((kChipWidth * 18) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 17)), 30);
			m_pWalkEnemy[14]->Init(static_cast<float>((kChipWidth * 24) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 9)), 30);
		}

		// 飛ぶ敵
		{
			m_pFlyEnemy[0]->Init(static_cast<float>((kChipWidth * 4) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 9), 300);
			m_pFlyEnemy[1]->Init(static_cast<float>((kChipWidth * 6) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 12), 800);
			m_pFlyEnemy[2]->Init(static_cast<float>((kChipWidth * 8) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 15), 500);
			m_pFlyEnemy[3]->Init(static_cast<float>((kChipWidth * 8) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 20), 700);
			m_pFlyEnemy[4]->Init(static_cast<float>((kChipWidth * 6) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 28), 610);
			m_pFlyEnemy[5]->Init(static_cast<float>((kChipWidth * 16) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 22), 200);
			m_pFlyEnemy[6]->Init(static_cast<float>((kChipWidth * 9) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 17), 460);
			m_pFlyEnemy[7]->Init(static_cast<float>((kChipWidth * 15) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 25), 50);
			m_pFlyEnemy[8]->Init(static_cast<float>((kChipWidth * 20) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 15), 300);
			m_pFlyEnemy[9]->Init(static_cast<float>((kChipWidth * 15) - (kChipWidth * 0.5f)), static_cast<float>(kChipHeight * 19), 650);
		}
}

void Stage_02::NeedleInit()
{
	/*初期化と座標の設定*/

		m_pNeedle[0]->Init(static_cast<float>((kChipWidth * 29) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 79)), 0);
		m_pNeedle[1]->Init(static_cast<float>((kChipWidth * 28) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 79)), 0);
		m_pNeedle[2]->Init(static_cast<float>((kChipWidth * 27) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 79)), 0);
		m_pNeedle[3]->Init(static_cast<float>((kChipWidth * 28) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 69)), 110);
		m_pNeedle[4]->Init(static_cast<float>((kChipWidth * 27) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 69)), 110);
		m_pNeedle[5]->Init(static_cast<float>((kChipWidth * 21) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 69)), 110);
		m_pNeedle[6]->Init(static_cast<float>((kChipWidth * 20) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 69)), 110);
		m_pNeedle[7]->Init(static_cast<float>((kChipWidth * 19) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 69)), 110);
		m_pNeedle[8]->Init(static_cast<float>((kChipWidth * 2) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 65)), 110);
		m_pNeedle[9]->Init(static_cast<float>((kChipWidth * 3) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 65)), 110);
		m_pNeedle[10]->Init(static_cast<float>((kChipWidth * 4) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 65)), 110);
		m_pNeedle[11]->Init(static_cast<float>((kChipWidth * 7) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 64)), 0);
		m_pNeedle[12]->Init(static_cast<float>((kChipWidth * 8) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 64)), 0);
		m_pNeedle[13]->Init(static_cast<float>((kChipWidth * 9) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 64)), 0);
		m_pNeedle[14]->Init(static_cast<float>((kChipWidth * 10) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 64)), 0);
		m_pNeedle[15]->Init(static_cast<float>((kChipWidth * 7) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 56)), 110);
		m_pNeedle[16]->Init(static_cast<float>((kChipWidth * 8) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 56)), 110);
		m_pNeedle[17]->Init(static_cast<float>((kChipWidth * 9) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 56)), 110);
		m_pNeedle[18]->Init(static_cast<float>((kChipWidth * 24) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 50)), 110);
		m_pNeedle[19]->Init(static_cast<float>((kChipWidth * 25) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 50)), 110);
		m_pNeedle[20]->Init(static_cast<float>((kChipWidth * 26) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 50)), 110);
		m_pNeedle[21]->Init(static_cast<float>((kChipWidth * 8) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 49)), 0);
		m_pNeedle[22]->Init(static_cast<float>((kChipWidth * 9) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 49)), 0);
		m_pNeedle[23]->Init(static_cast<float>((kChipWidth * 10) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 49)), 0);
		m_pNeedle[24]->Init(static_cast<float>((kChipWidth * 11) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 49)), 0);
		m_pNeedle[25]->Init(static_cast<float>((kChipWidth * 15) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 49)), 0);
		m_pNeedle[26]->Init(static_cast<float>((kChipWidth * 16) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 49)), 0);
		m_pNeedle[27]->Init(static_cast<float>((kChipWidth * 17) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 49)), 0);
		m_pNeedle[28]->Init(static_cast<float>((kChipWidth * 18) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 49)), 0);
		m_pNeedle[29]->Init(static_cast<float>((kChipWidth * 18) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 44)), 110);
		m_pNeedle[30]->Init(static_cast<float>((kChipWidth * 19) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 44)), 110);
		m_pNeedle[31]->Init(static_cast<float>((kChipWidth * 21) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 39)), 110);
		m_pNeedle[32]->Init(static_cast<float>((kChipWidth * 22) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 39)), 110);
		m_pNeedle[33]->Init(static_cast<float>((kChipWidth * 23) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 39)), 110);
		m_pNeedle[34]->Init(static_cast<float>((kChipWidth * 14) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 38)), 0);
		m_pNeedle[35]->Init(static_cast<float>((kChipWidth * 15) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 38)), 0);
		m_pNeedle[36]->Init(static_cast<float>((kChipWidth * 16) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 38)), 0);
		m_pNeedle[37]->Init(static_cast<float>((kChipWidth * 9) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 32)), 110);
		m_pNeedle[38]->Init(static_cast<float>((kChipWidth * 10) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 32)), 110);
		m_pNeedle[39]->Init(static_cast<float>((kChipWidth * 11) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 32)), 110);
		m_pNeedle[40]->Init(static_cast<float>((kChipWidth * 21) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 26)), 0);
		m_pNeedle[41]->Init(static_cast<float>((kChipWidth * 22) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 26)), 0);
		m_pNeedle[42]->Init(static_cast<float>((kChipWidth * 5) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 23)), 0);
		m_pNeedle[43]->Init(static_cast<float>((kChipWidth * 15) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 18)), 0);
		m_pNeedle[44]->Init(static_cast<float>((kChipWidth * 16) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 18)), 0);
		m_pNeedle[45]->Init(static_cast<float>((kChipWidth * 17) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 18)), 0);
		m_pNeedle[46]->Init(static_cast<float>((kChipWidth * 11) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 12)), 0);
		m_pNeedle[47]->Init(static_cast<float>((kChipWidth * 26) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 10)), 0);
}

void Stage_02::ItemInit()
{
	/*初期化と座標の設定*/

	// 回復アイテム
		m_pRecoveryItem[0]->Init(static_cast<float>((kChipWidth * 14) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 4)));
		m_pRecoveryItem[1]->Init(static_cast<float>((kChipWidth * 7) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 67)));
		m_pRecoveryItem[2]->Init(static_cast<float>((kChipWidth * 26) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 48)));
		m_pRecoveryItem[3]->Init(static_cast<float>((kChipWidth * 6) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 22)));
		m_pRecoveryItem[4]->Init(static_cast<float>((kChipWidth * 25) - (kChipWidth * 0.5f)), static_cast<float>((kChipHeight * 42)));
}

void Stage_02::EnemyUpdate()
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

void Stage_02::NeedleUpdate()
{
	for (int i = 0; i < m_pNeedle.size(); i++)
	{
		m_pNeedle[i]->Update();
	}
}

void Stage_02::ItemUpdate()
{
	// 回復アイテム
	for (int i = 0; i < m_pRecoveryItem.size(); i++)
	{
		m_pRecoveryItem[i]->Update();
	}
}

void Stage_02::BackDraw()
{
	// 前景のサイズの取得
	GetGraphSize(m_foreground, &m_fgW, &m_fgH);

	int scrollFore = static_cast<int>(m_scrollX) % m_fgW;

	// プレイヤーの位置に応じたスクロール量を決定する
	int scrollX = m_pCamera->GetScrollX();
	int scrollY = m_pCamera->GetScrollY();

	int posX = scrollX;
	int posY = kScreenHeight - scrollY - kScreenHeight;

	DrawGraph(posX, posY, m_background, true);

	for (int index = 0; index < 2; ++index)
	{
		DrawGraph(-scrollFore + index * m_fgW,
			posY,
			m_foreground,
			true);
	}
}

void Stage_02::ChipDraw()
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

			DrawRectGraph(posX, posY, 
				srcX, srcY,
				kChipWidth, kChipHeight,
				m_chipHandle, true);
		}
	}
}

void Stage_02::EnemyDraw()
{
	// 止まっている敵
	for (int i = 0; i < m_pStopEnemy.size(); i++)
	{
		m_pStopEnemy[i]->Draw();
	}
	// 歩く敵
	for (int i = 0; i < m_pWalkEnemy.size(); i++)
	{
		m_pWalkEnemy[i]->Draw();
	}
	// 飛ぶ敵
	for (int i = 0; i < m_pFlyEnemy.size(); i++)
	{
		m_pFlyEnemy[i]->Draw();
	}
}

void Stage_02::NeedleDraw()
{
	for (int i = 0; i < m_pNeedle.size(); i++)
	{
		m_pNeedle[i]->Draw();
	}
}

void Stage_02::ItemDraw()
{
	// 回復アイテム
	for (int i = 0; i < m_pRecoveryItem.size(); i++)
	{
		m_pRecoveryItem[i]->Draw();
	}
}

void Stage_02::VolumeSetting()
{
	// プレイヤーがゴールに近づくとSEの音量が上がり、BGMの音量が小さくなる
	int goalToPlayer = static_cast<int>(kGoalPosY - m_pPlayer->GetTop());
	if (goalToPlayer >= 0)
	{
		goalToPlayer = 0;
	}
	// SEの音量は200以上にはならないようにする
	m_goalSeVolume = goalToPlayer + kBgmVolumeMax;
	if (m_goalSeVolume >= kBgmVolumeMax)
	{
		m_goalSeVolume = kBgmVolumeMax;
	}
	else if (m_goalSeVolume <= 0)
	{
		m_goalSeVolume = 0;
	}
	// BGMの音量設定
	ChangeVolumeSoundMem(kBgmVolumeMax - (m_goalSeVolume / 2), m_bgmHandle);
	// SEの音量設定
	ChangeVolumeSoundMem(m_goalSeVolume, m_goalSeHandle);
}

void Stage_02::OnMiss()
{
	// プレイヤーがミスをした場合は残機を減らしてHPを全回復して初期位置に戻す
	// このステージでプレイヤーが落ちることはない為、HPがゼロになった場合のミスだけ処理を書いておく
	if (m_pPlayer->GetHp() <= 0)
	{
		// 画面を揺らす
		if (!m_isMiss)
		{
			m_shakeFrame = kScreenShakeFrameMax;
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