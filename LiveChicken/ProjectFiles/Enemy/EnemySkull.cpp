#include "EnemySkull.h"

EnemySkull::EnemySkull(int modelHandle):
	EnemyBase(modelHandle)
{
	/*ˆ—–³‚µ*/
}

EnemySkull::~EnemySkull()
{
	/*ˆ—–³‚µ*/
}

void EnemySkull::Update()
{
	AdjustmentModelPos();
}
