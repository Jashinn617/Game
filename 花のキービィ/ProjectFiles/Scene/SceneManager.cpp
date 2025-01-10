#include "SceneManager.h"
#include "TitleScene.h"
#include "StoryScene.h"
#include "Tutorial00.h"
#include "Tutorial01.h"
#include "Tutorial02.h"
#include "Stage_01.h"
#include "Stage_02.h"
#include "LastStage.h"
#include "ClearScene.h"
#include "GameOverScene.h"
#include "../Player.h"
#include "../Attack/AirBullet.h"
#include "../Attack/Inhale.h"
#include "../Attack/Vomit.h"
#include "../Enemy/Boss.h"
#include "../Enemy/StopEnemy.h"
#include "../Enemy/WalkEnemy.h"
#include "../Enemy/FlyEnemy.h"
#include "../Camera.h"
#include "../UI.h"
#include "../Object/Door.h"
#include "../Object/GoalFlower.h"
#include "../Object/Needle.h"
#include "../Object/RecoveryItem.h"

#include "DxLib.h"

namespace
{
	// エネミーの数
	constexpr int kStopEnemyNum = 30;
	constexpr int kWalkEnemyNum = 15;
	constexpr int kFlyEnemyNum = 10;

	// 針の数
	constexpr int kNeedleNum = 48;

	// アイテムの数
	constexpr int kItemNum = 5;
}

SceneManager::SceneManager() :
	m_runScene(kSceneKindTitle)
{
	// ポインタの設定
	m_pTitle = std::make_shared<TitleScene>();
	m_pStory = std::make_shared<StoryScene>();
	m_pTutorial0 = std::make_shared<Tutorial00>();
	m_pTutorial1 = std::make_shared<Tutorial01>();
	m_pTutorial2 = std::make_shared<Tutorial02>();
	m_pStage_01 = std::make_shared<Stage_01>();
	m_pStage_02 = std::make_shared<Stage_02>();
	m_pLastStage = std::make_shared<LastStage>();
	m_pClear = std::make_shared<ClearScene>();
	m_pGameOver = std::make_shared<GameOverScene>();
	m_pPlayer = std::make_shared<Player>();
	m_pAirBullet = std::make_shared<AirBullet>();
	m_pInhale = std::make_shared<Inhale>();
	m_pVomit = std::make_shared<Vomit>();
	m_pCamera = std::make_shared<Camera>();
	m_pUI = std::make_shared<UI>();
	m_pDoor = std::make_shared<Door>();
	m_pGoalFlower = std::make_shared<GoalFlower>();
	m_pBoss = std::make_shared<Boss>();

	m_pStopEnemy.resize(kStopEnemyNum);
	for (int i = 0; i < m_pStopEnemy.size(); i++)
	{
		m_pStopEnemy[i] = std::make_shared<StopEnemy>();
	}

	m_pWalkEnemy.resize(kWalkEnemyNum);
	for (int i = 0; i < m_pWalkEnemy.size(); i++)
	{
		m_pWalkEnemy[i] = std::make_shared<WalkEnemy>();
	}

	m_pFlyEnemy.resize(kFlyEnemyNum);
	for (int i = 0; i < m_pFlyEnemy.size(); i++)
	{
		m_pFlyEnemy[i] = std::make_shared<FlyEnemy>();
	}

	m_pNeedle.resize(kNeedleNum);
	for (int i = 0; i < m_pNeedle.size(); i++)
	{
		m_pNeedle[i] = std::make_shared<Needle>();
	}

	m_pItem.resize(kItemNum);
	for (int i = 0; i < m_pItem.size(); i++)
	{
		m_pItem[i] = std::make_shared<RecoveryItem>();
	}

	// 情報を参照するためのポインタを渡す
	m_pPlayer->SetInhale(m_pInhale);
	m_pPlayer->SetVomit(m_pVomit);
	m_pPlayer->SetAirBullet(m_pAirBullet);
	m_pPlayer->SetCamera(m_pCamera);

	m_pVomit->SetCamera(m_pCamera);
	m_pVomit->SetPlayer(m_pPlayer);

	m_pAirBullet->SetCamera(m_pCamera);
	m_pAirBullet->SetPlayer(m_pPlayer);

	m_pInhale->SetCamera(m_pCamera);
	m_pInhale->SetPlayer(m_pPlayer);

	m_pUI->SetPlayer(m_pPlayer);
	
	m_pCamera->SetPlayer(m_pPlayer);

	m_pTutorial1->SetPlayer(m_pPlayer); 
	m_pTutorial1->SetCamera(m_pCamera);
	m_pTutorial1->SetUI(m_pUI);
	m_pTutorial1->SetDoor(m_pDoor);

	m_pTutorial2->SetPlayer(m_pPlayer);
	m_pTutorial2->SetCamera(m_pCamera);
	m_pTutorial2->SetUI(m_pUI);
	m_pTutorial2->SetFlower(m_pGoalFlower);
	
	m_pStage_01->SetPlayer(m_pPlayer);
	m_pStage_01->SetCamera(m_pCamera);
	m_pStage_01->SetUI(m_pUI);
	m_pStage_01->SetGameOver(m_pGameOver);
	m_pStage_01->SetDoor(m_pDoor);
	
	m_pStage_02->SetPlayer(m_pPlayer);
	m_pStage_02->SetCamera(m_pCamera);
	m_pStage_02->SetUI(m_pUI);
	m_pStage_02->SetDoor(m_pDoor);

	m_pLastStage->SetPlayer(m_pPlayer);
	m_pLastStage->SetCamera(m_pCamera);
	m_pLastStage->SetFlower(m_pGoalFlower);
	m_pLastStage->SetUI(m_pUI);
	m_pLastStage->SetBoss(m_pBoss);
	
	m_pGameOver->SetPlayer(m_pPlayer);
	
	m_pTitle->SetPlayer(m_pPlayer);
	
	m_pInhale->SetPlayer(m_pPlayer);
	
	m_pDoor->SetPlayer(m_pPlayer);
	m_pDoor->SetCamera(m_pCamera);

	m_pGoalFlower->SetPlayer(m_pPlayer);
	m_pGoalFlower->SetCamera(m_pCamera);

	m_pBoss->SetCamera(m_pCamera);
	m_pBoss->SetPlayer(m_pPlayer);
	m_pBoss->SetVomit(m_pVomit);
	m_pBoss->SetInhale(m_pInhale);

	for (int i = 0; i < m_pStopEnemy.size(); i++)
	{
		m_pStopEnemy[i]->SetCamera(m_pCamera);
		m_pStopEnemy[i]->SetPlayer(m_pPlayer);
		m_pStopEnemy[i]->SetInhale(m_pInhale);
		m_pStopEnemy[i]->SetVomit(m_pVomit);
		m_pStopEnemy[i]->SetAirBullet(m_pAirBullet);

		m_pTutorial2->SetStopEnemy(m_pStopEnemy[i], i);

		m_pStage_01->SetStopEnemy(m_pStopEnemy[i], i);

		m_pStage_02->SetStopEnemy(m_pStopEnemy[i], i);
	}
	for (int i = 0; i < m_pWalkEnemy.size(); i++)
	{
		m_pWalkEnemy[i]->SetCamera(m_pCamera);
		m_pWalkEnemy[i]->SetPlayer(m_pPlayer);
		m_pWalkEnemy[i]->SetInhale(m_pInhale);
		m_pWalkEnemy[i]->SetVomit(m_pVomit);
		m_pWalkEnemy[i]->SetAirBullet(m_pAirBullet);

		m_pStage_01->SetWalkEnemy(m_pWalkEnemy[i], i);

		m_pStage_02->SetWalkEnemy(m_pWalkEnemy[i], i);
	}
	for (int i = 0; i < m_pFlyEnemy.size(); i++)
	{
		m_pFlyEnemy[i]->SetCamera(m_pCamera);
		m_pFlyEnemy[i]->SetPlayer(m_pPlayer);
		m_pFlyEnemy[i]->SetInhale(m_pInhale);
		m_pFlyEnemy[i]->SetVomit(m_pVomit);
		m_pFlyEnemy[i]->SetAirBullet(m_pAirBullet);

		m_pStage_01->SetFlyEnemy(m_pFlyEnemy[i], i);

		m_pStage_02->SetFlyEnemy(m_pFlyEnemy[i], i);
	}
	for (int i = 0; i < m_pNeedle.size(); i++)
	{
		m_pNeedle[i]->SetCamera(m_pCamera);
		m_pNeedle[i]->SetPlayer(m_pPlayer);

		m_pStage_02->SetNeedle(m_pNeedle[i], i);
	}
	for (int i = 0; i < m_pItem.size(); i++)
	{
		m_pItem[i]->SetPlayer(m_pPlayer);
		m_pItem[i]->SetCamera(m_pCamera);
		m_pItem[i]->SetInhale(m_pInhale);

		m_pTutorial2->SetItem(m_pItem[i], i);

		m_pStage_01->SetItem(m_pItem[i], i);

		m_pStage_02->SetItem(m_pItem[i], i);
	}
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	// 初期化処理
	// 実行したいシーンの初期化を行う
	switch (m_runScene)
	{
	case kSceneKindTitle:
		m_pTitle->Init();
		break;
	case kSceneKindStory:
		m_pStory->Init();
		break;
	case kSceneKindTutorial0:
		m_pTutorial0->Init();
		break;
	case kSceneKindTutorial1:
		m_pTutorial1->Init();
		break;
	case kSceneKindTutorial2:
		m_pTutorial2->Init();
		break;
	case kSceneKindStage1:
		m_pStage_01->Init();
		break;
	case kSceneKindStage2:
		m_pStage_02->Init();
		break;
	case kSceneKindLastStage:
		m_pLastStage->Init();
		break;
	case kSceneKindClear:
		m_pClear->Init();
		break;
	case kSceneKindGameOver:
		m_pGameOver->Init();
		break;
	default:
		break;
	}
}

void SceneManager::Update(Input& input)
{
	// 前のフレームの時点で処理が終わっていたらシーンの切り替え
	// シーンの切り替えを行う
	switch (m_runScene)
	{
	case kSceneKindTitle:
		// タイトル画面の終了チェック
		if (m_pTitle->IsSceneEnd())
		{
			m_pTitle->End();	// 実行していたシーンを終了

			// 次のシーンに移動する
			m_runScene = kSceneKindStory;	// 次のシーン
			m_pStory->Init();	// 次のシーンの初期化
		}
		break;
	case kSceneKindStory:
		if (m_pStory->IsSceneEnd())
		{
			m_pStory->End();

			m_runScene = kSceneKindTutorial0;
			m_pTutorial0->Init();
		}

		break;
	case kSceneKindTutorial0:
		if (m_pTutorial0->IsSceneEnd())
		{
			m_pTutorial0->End();

			if (m_pTutorial0->IsNextScene() == m_pTutorial0->GetStage01())
			{
				m_runScene = kSceneKindStage1;
				m_pStage_01->Init();
			}
			else if (m_pTutorial0->IsNextScene() == m_pTutorial0->GetTutorial1())
			{
				m_runScene = kSceneKindTutorial1;
				m_pTutorial1->Init();
			}
		}
		break;
	case kSceneKindTutorial1:
		if (m_pTutorial1->IsSceneEnd())
		{
			m_runScene = kSceneKindTutorial2;
			m_pTutorial2->Init();
		}
		break;
	case kSceneKindTutorial2:
		if (m_pTutorial2->IsSceneEnd())
		{
			m_pTutorial1->End();
			m_pTutorial2->End();

			m_runScene = kSceneKindStage1;
			m_pStage_01->Init();
		}
		break;
	case kSceneKindStage1:
		if (m_pStage_01->IsSceneEnd())
		{
			m_pStage_01->End();

			if (m_pStage_01->IsNextScene() == m_pStage_01->GetStage02())
			{
				m_runScene = kSceneKindStage2;
				m_pStage_02->Init();
			}
			else if (m_pStage_01->IsNextScene() == m_pStage_01->GetGameOver())
			{
				m_runScene = kSceneKindGameOver;
				m_pGameOver->Init();
			}
		}
		break;
	case kSceneKindStage2:
		if (m_pStage_02->IsSceneEnd())
		{
			m_pStage_02->End();
			if (m_pStage_02->IsNextScene() == m_pStage_02->GetLastStage())
			{
				m_runScene = kSceneKindLastStage;
				m_pLastStage->Init();
			}
			else if (m_pStage_02->IsNextScene() == m_pStage_02->GetGameOver())
			{
				m_runScene = kSceneKindGameOver;
				m_pGameOver->Init();
			}
		}
		break;
	case kSceneKindLastStage:
		if (m_pLastStage->IsSceneEnd())
		{
			m_pLastStage->End();
			if (m_pLastStage->IsNextScene() == m_pLastStage->GetClear())
			{
				m_runScene = kSceneKindClear;
				m_pClear->Init();
			}
			else if(m_pLastStage->IsNextScene() == m_pLastStage->GetGameOver())
			{
				m_runScene = kSceneKindGameOver;
				m_pGameOver->Init();
			}
		}
		break;
	case kSceneKindClear:
		if (m_pClear->IsSceneEnd())
		{
			m_pClear->End();

			m_runScene = kSceneKindTitle;
			m_pTitle->Init();
		}
		break;
	case kSceneKindGameOver:
		if (m_pGameOver->IsSceneEnd())
		{
			m_pGameOver->End();

			if (m_pGameOver->IsNextScene() == m_pGameOver->GetTitle())
			{
				m_runScene = kSceneKindTitle;
				m_pTitle->Init();
			}
			else if (m_pGameOver->IsNextScene() == m_pGameOver->GetStage01())
			{
				m_runScene = kSceneKindStage1;
				m_pStage_01->Init();
			}
		}
		break;
	default:
		break;
	}

	// 各シーンの更新を行う
	switch (m_runScene)
	{
	case kSceneKindTitle:
		m_pTitle->Update(input);
		break;
	case kSceneKindStory:
		m_pStory->Update(input);
		break;
	case kSceneKindTutorial0:
		m_pTutorial0->Update(input);
		break;
	case kSceneKindTutorial1:
		m_pTutorial1->Update(input);
		break;
	case kSceneKindTutorial2:
		m_pTutorial2->Update(input);
		break;
	case kSceneKindStage1:
		m_pStage_01->Update(input);
		break;
	case kSceneKindStage2:
		m_pStage_02->Update(input);
		break;
	case kSceneKindLastStage:
		m_pLastStage->Update(input);
		break;
	case kSceneKindClear:
		m_pClear->Update(input);
		break;
	case kSceneKindGameOver:
		m_pGameOver->Update(input);
		break;
	default:
		break;
	}
}

void SceneManager::Draw()
{
	switch (m_runScene)
	{
	case kSceneKindTitle:
		m_pTitle->Draw();
		break;
	case kSceneKindStory:
		m_pStory->Draw();
		break;
	case kSceneKindTutorial0:
		m_pTutorial0->Draw();
		break;
	case kSceneKindTutorial1:
		m_pTutorial1->Draw();
		break;
	case kSceneKindTutorial2:
		m_pTutorial2->Draw();
		break;
	case kSceneKindStage1:
		m_pStage_01->Draw();
		break;
	case kSceneKindStage2:
		m_pStage_02->Draw();
		break;
	case kSceneKindLastStage:
		m_pLastStage->Draw();
		break;
	case kSceneKindClear:
		m_pClear->Draw();
		break;
	case kSceneKindGameOver:
		m_pGameOver->Draw();
		break;
	default:
		break;
	}
}

void SceneManager::End()
{
	switch (m_runScene)
	{
	case kSceneKindTitle:
		m_pTitle->End();
		break;
	case kSceneKindStory:
		m_pStory->End();
		break;
	case kSceneKindTutorial0:
		m_pTutorial0->End();
		break;
	case kSceneKindTutorial1:
		m_pTutorial1->End();
		break;
	case kSceneKindTutorial2:
		m_pTutorial2->End();
		break;
	case kSceneKindStage1:
		m_pStage_01->End();
		break;
	case kSceneKindStage2:
		m_pStage_02->End();
		break;
	case kSceneKindLastStage:
		m_pLastStage->End();
		break;
	case kSceneKindClear:
		m_pClear->End();
		break;
	case kSceneKindGameOver:
		m_pGameOver->End();
		break;
	default:
		break;
	}
}
