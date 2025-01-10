#include "SceneManager.h"
#include "SceneMain.h"
#include "../Util/Input.h"
#include "../Util/HandleManager.h"

SceneManager::SceneManager() :
	m_pScene(nullptr)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init(HandleManager& handle)
{
	// 最初のシーンのメモリを確保する
	m_pScene = make_shared<SceneMain>();
	m_pScene->Init(handle);
}

void SceneManager::Update(Input& input, HandleManager& handle)
{
	shared_ptr<SceneBase> pNext = m_pScene->Update(input,handle);
	if (pNext != m_pScene)
	{
		// 現在処理中のシーンの終了処理
		m_pScene->End();

		// Updateが返した新しいシーンの開始処理を行う
		m_pScene = pNext;
		m_pScene->Init(handle);
	}
}

void SceneManager::Draw(HandleManager& handle)
{
	m_pScene->Draw(handle);
}

void SceneManager::End()
{
	m_pScene->End();
}
