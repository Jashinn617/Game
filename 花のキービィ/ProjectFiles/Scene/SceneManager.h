#pragma once
#include <memory>
#include <vector>

class Input;
class Player;
class AirBullet;
class Inhale;
class Vomit;
class Boss;;
class StopEnemy;
class WalkEnemy;
class FlyEnemy;
class Camera;
class UI;
class TitleScene;
class StoryScene;
class Tutorial00;
class Tutorial01;
class Tutorial02;
class Stage_01;
class Stage_02;
class LastStage;
class ClearScene;
class GameOverScene;
class Door;
class GoalFlower;
class Needle;
class RecoveryItem;
class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Init();
	void Update(Input& input);
	void Draw();
	void End();

private:
	enum SceneKind
	{
		kSceneKindTitle,
		kSceneKindStory,
		kSceneKindTutorial0,
		kSceneKindTutorial1,
		kSceneKindTutorial2,
		kSceneKindStage1,
		kSceneKindStage2,
		kSceneKindLastStage,
		kSceneKindClear,
		kSceneKindGameOver
	};

private:
	// 現在実行中のシーン
	SceneKind m_runScene;

	// SceneManagerで管理する各シーン
	std::shared_ptr<TitleScene> m_pTitle;
	std::shared_ptr<StoryScene> m_pStory;
	std::shared_ptr<Tutorial00> m_pTutorial0;
	std::shared_ptr<Tutorial01> m_pTutorial1;
	std::shared_ptr<Tutorial02> m_pTutorial2;
	std::shared_ptr<Stage_01> m_pStage_01;
	std::shared_ptr<Stage_02> m_pStage_02;
	std::shared_ptr<LastStage> m_pLastStage;
	std::shared_ptr<ClearScene> m_pClear;
	std::shared_ptr<GameOverScene> m_pGameOver;

	// 各ポインタ
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<AirBullet> m_pAirBullet;
	std::shared_ptr <Inhale> m_pInhale;
	std::shared_ptr<Vomit> m_pVomit;
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<UI> m_pUI;
	std::shared_ptr<Door> m_pDoor;
	std::shared_ptr<GoalFlower> m_pGoalFlower;
	std::shared_ptr<Boss> m_pBoss;

	std::vector<std::shared_ptr<Needle>> m_pNeedle;
	std::vector<std::shared_ptr<StopEnemy>> m_pStopEnemy;
	std::vector<std::shared_ptr<WalkEnemy>> m_pWalkEnemy;
	std::vector<std::shared_ptr<FlyEnemy>> m_pFlyEnemy;
	std::vector<std::shared_ptr<RecoveryItem>> m_pItem;
};