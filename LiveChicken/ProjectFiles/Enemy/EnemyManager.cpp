#include "EnemyManager.h"
#include "DxLib.h"
#include "EnemyBase.h"
#include "EnemyBee.h"
#include "EnemyCrab.h"
#include "EnemySkull.h"
#include "../Player.h"

EnemyManager::EnemyManager():
	m_beeModelHandle(-1),
	m_crabModelHandle(-1),
	m_skullModelHandle(-1)
{
	/*3Dモデルのロード*/
	m_beeModelHandle = MV1LoadModel("Data/Model/Enemy/BeeEnemy.mv1");
	m_crabModelHandle = MV1LoadModel("Data/Model/Enemy/CrabEnemy.mv1");
	m_skullModelHandle = MV1LoadModel("Data/Model/Enemy/SkullEnemy.mv1");

	// 敵の種類を決める
	m_pEnemy.resize(kEnemyNum);
	for (int i = 0; i < kEnemyChangeNum1; i++)// ハチ
	{
		m_pEnemy[i] = make_shared<EnemyBee>(m_beeModelHandle);
	}
	for (int i = kEnemyChangeNum1; i < kEnemyChangeNum2; i++)// カニ
	{
		m_pEnemy[i] = make_shared<EnemyCrab>(m_crabModelHandle);
	}
	for (int i = kEnemyChangeNum2; i < kEnemyChangeNum3; i++)// ホネ
	{
		m_pEnemy[i] = make_shared<EnemySkull>(m_skullModelHandle);
	}
}

EnemyManager::~EnemyManager()
{
	/*処理無し*/
}

void EnemyManager::CreateEnemyes()
{
	// 敵の生成と位置の設定
	m_pEnemy[0]->SetPos(VGet(kPosX0, kPosYDown, kPosZ));
	m_pEnemy[1]->SetPos(VGet(kPosX1, kPosYDown, kPosZ));
	m_pEnemy[2]->SetPos(VGet(kPosX2, kPosYDown, kPosZ));
	m_pEnemy[3]->SetPos(VGet(kPosX3, kPosYDown, kPosZ));
	m_pEnemy[4]->SetPos(VGet(kPosX4, kPosYDown, kPosZ));
	m_pEnemy[5]->SetPos(VGet(kPosX5, kPosYDown, kPosZ));
	m_pEnemy[6]->SetPos(VGet(kPosX6, kPosYDown, kPosZ));
	m_pEnemy[7]->SetPos(VGet(kPosX7, kPosYUp, kPosZ));
	m_pEnemy[8]->SetPos(VGet(kPosX8, kPosYUp, kPosZ));
	m_pEnemy[9]->SetPos(VGet(kPosX9, kPosYUp, kPosZ));
	m_pEnemy[10]->SetPos(VGet(kPosX10, kPosYUp, kPosZ));
	m_pEnemy[11]->SetPos(VGet(kPosX11, kPosYUp, kPosZ));
	m_pEnemy[12]->SetPos(VGet(kPosX12, kPosYUp, kPosZ));
	m_pEnemy[13]->SetPos(VGet(kPosX13, kPosYDown, kPosZ));
	m_pEnemy[14]->SetPos(VGet(kPosX14, kPosYUp, kPosZ));
	m_pEnemy[15]->SetPos(VGet(kPosX15, kPosYDown, kPosZ));
	m_pEnemy[16]->SetPos(VGet(kPosX16, kPosYDown, kPosZ));
	m_pEnemy[17]->SetPos(VGet(kPosX17, kPosYDown, kPosZ));
	m_pEnemy[18]->SetPos(VGet(kPosX18, kPosYDown, kPosZ));
	m_pEnemy[19]->SetPos(VGet(kPosX19, kPosYDown, kPosZ));
	m_pEnemy[20]->SetPos(VGet(kPosX20, kPosYDown, kPosZ));
	m_pEnemy[21]->SetPos(VGet(kPosX21, kPosYDown, kPosZ));
	m_pEnemy[22]->SetPos(VGet(kPosX22, kPosYDown, kPosZ));
	m_pEnemy[23]->SetPos(VGet(kPosX23, kPosYDown, kPosZ));
	m_pEnemy[24]->SetPos(VGet(kPosX24, kPosYUp, kPosZ));
	m_pEnemy[25]->SetPos(VGet(kPosX25, kPosYDown, kPosZ));
	m_pEnemy[26]->SetPos(VGet(kPosX26, kPosYUp, kPosZ));
	m_pEnemy[27]->SetPos(VGet(kPosX27, kPosYDown, kPosZ));
	m_pEnemy[28]->SetPos(VGet(kPosX28, kPosYUp, kPosZ));
	m_pEnemy[29]->SetPos(VGet(kPosX29, kPosYDown, kPosZ));
	m_pEnemy[30]->SetPos(VGet(kPosX30, kPosYUp, kPosZ));
	m_pEnemy[31]->SetPos(VGet(kPosX31, kPosYDown, kPosZ));
	m_pEnemy[32]->SetPos(VGet(kPosX32, kPosYUp, kPosZ));
	m_pEnemy[33]->SetPos(VGet(kPosX33, kPosYDown, kPosZ));
	m_pEnemy[34]->SetPos(VGet(kPosX34, kPosYUp, kPosZ));
	m_pEnemy[35]->SetPos(VGet(kPosX35, kPosYUp, kPosZ));
	m_pEnemy[36]->SetPos(VGet(kPosX36, kPosYDown, kPosZ));
	m_pEnemy[37]->SetPos(VGet(kPosX37, kPosYUp, kPosZ));
	m_pEnemy[38]->SetPos(VGet(kPosX38, kPosYDown, kPosZ));
	m_pEnemy[39]->SetPos(VGet(kPosX39, kPosYUp, kPosZ));
	m_pEnemy[40]->SetPos(VGet(kPosX40, kPosYDown, kPosZ));
	m_pEnemy[41]->SetPos(VGet(kPosX41, kPosYDown, kPosZ));
	m_pEnemy[42]->SetPos(VGet(kPosX42, kPosYUp, kPosZ));
	m_pEnemy[43]->SetPos(VGet(kPosX43, kPosYUp, kPosZ));
	m_pEnemy[44]->SetPos(VGet(kPosX44, kPosYDown, kPosZ));
	m_pEnemy[45]->SetPos(VGet(kPosX45, kPosYDown, kPosZ));
	m_pEnemy[46]->SetPos(VGet(kPosX46, kPosYUp, kPosZ));
	m_pEnemy[47]->SetPos(VGet(kPosX47, kPosYDown, kPosZ));
	m_pEnemy[48]->SetPos(VGet(kPosX48, kPosYDown, kPosZ));
	m_pEnemy[49]->SetPos(VGet(kPosX49, kPosYUp, kPosZ));
	m_pEnemy[50]->SetPos(VGet(kPosX50, kPosYDown, kPosZ));
	m_pEnemy[51]->SetPos(VGet(kPosX51, kPosYUp, kPosZ));
	m_pEnemy[52]->SetPos(VGet(kPosX52, kPosYDown, kPosZ));
	m_pEnemy[53]->SetPos(VGet(kPosX53, kPosYUp, kPosZ));
	m_pEnemy[54]->SetPos(VGet(kPosX54, kPosYDown, kPosZ));
	m_pEnemy[55]->SetPos(VGet(kPosX55, kPosYUp, kPosZ));
	m_pEnemy[56]->SetPos(VGet(kPosX56, kPosYDown, kPosZ));
	m_pEnemy[57]->SetPos(VGet(kPosX57, kPosYUp, kPosZ));
	m_pEnemy[58]->SetPos(VGet(kPosX58, kPosYDown, kPosZ));
	m_pEnemy[59]->SetPos(VGet(kPosX59, kPosYUp, kPosZ));
	m_pEnemy[60]->SetPos(VGet(kPosX60, kPosYDown, kPosZ));
	m_pEnemy[61]->SetPos(VGet(kPosX61, kPosYUp, kPosZ));
	m_pEnemy[62]->SetPos(VGet(kPosX62, kPosYDown, kPosZ));
	m_pEnemy[63]->SetPos(VGet(kPosX63, kPosYUp, kPosZ));
	m_pEnemy[64]->SetPos(VGet(kPosX64, kPosYDown, kPosZ));
	m_pEnemy[65]->SetPos(VGet(kPosX65, kPosYUp, kPosZ));
	m_pEnemy[66]->SetPos(VGet(kPosX66, kPosYDown, kPosZ));
	m_pEnemy[67]->SetPos(VGet(kPosX67, kPosYUp, kPosZ));
	m_pEnemy[68]->SetPos(VGet(kPosX68, kPosYDown, kPosZ));
	m_pEnemy[69]->SetPos(VGet(kPosX69, kPosYUp, kPosZ));
	m_pEnemy[70]->SetPos(VGet(kPosX70, kPosYDown, kPosZ));
	m_pEnemy[71]->SetPos(VGet(kPosX71, kPosYDown, kPosZ));
	m_pEnemy[72]->SetPos(VGet(kPosX72, kPosYDown, kPosZ));
	m_pEnemy[73]->SetPos(VGet(kPosX73, kPosYDown, kPosZ));
	m_pEnemy[74]->SetPos(VGet(kPosX74, kPosYUp, kPosZ));
	m_pEnemy[75]->SetPos(VGet(kPosX75, kPosYDown, kPosZ));
	m_pEnemy[76]->SetPos(VGet(kPosX76, kPosYUp, kPosZ));
	m_pEnemy[77]->SetPos(VGet(kPosX77, kPosYDown, kPosZ));
	m_pEnemy[78]->SetPos(VGet(kPosX78, kPosYDown, kPosZ));
	m_pEnemy[79]->SetPos(VGet(kPosX79, kPosYDown, kPosZ));
	m_pEnemy[80]->SetPos(VGet(kPosX80, kPosYDown, kPosZ));
	m_pEnemy[81]->SetPos(VGet(kPosX81, kPosYDown, kPosZ));
	m_pEnemy[82]->SetPos(VGet(kPosX82, kPosYDown, kPosZ));
	m_pEnemy[83]->SetPos(VGet(kPosX83, kPosYDown, kPosZ));
	m_pEnemy[84]->SetPos(VGet(kPosX84, kPosYDown, kPosZ));
	m_pEnemy[85]->SetPos(VGet(kPosX85, kPosYDown, kPosZ));
	m_pEnemy[86]->SetPos(VGet(kPosX86, kPosYDown, kPosZ));
	m_pEnemy[87]->SetPos(VGet(kPosX87, kPosYDown, kPosZ));
	m_pEnemy[88]->SetPos(VGet(kPosX88, kPosYDown, kPosZ));
	m_pEnemy[89]->SetPos(VGet(kPosX89, kPosYDown, kPosZ));
}

void EnemyManager::DestroyEnemyes()
{
	/*3Dモデルのデリーtp*/
	MV1DeleteModel(m_beeModelHandle);
	MV1DeleteModel(m_crabModelHandle);
	MV1DeleteModel(m_skullModelHandle);
}

void EnemyManager::Update()
{
	for (auto& enemy : m_pEnemy)
	{
		if (enemy != NULL)
		{
			enemy->Update();
		}
	}
}

void EnemyManager::Draw()
{
	for (auto& enemy : m_pEnemy)
	{
		if (enemy != NULL)
		{
			enemy->Draw();

		}
	}
}

bool EnemyManager::CollisionPlayer()
{
	// 円での当たり判定をとっている
	// 当たっていればtrue、そうでなければfalseを返す
	VECTOR playerPos = m_pPlayer->GetPos();
	playerPos.x -= 0.1f;
	float playerRad = m_pPlayer->GetRad();
	for (auto& enemy : m_pEnemy)
	{
		VECTOR enemyPos=enemy->GetPos();
		float enemyRad=enemy->GetRad();

		float a = playerPos.x - enemyPos.x;
		float b = playerPos.y - enemyPos.y;
		float c = sqrtf((a * a) + (b * b));

		if (c <= playerRad + enemyRad)
		{
			return true;
		}
	}

	return false;
}

