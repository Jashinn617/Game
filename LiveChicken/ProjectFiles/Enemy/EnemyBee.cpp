#include "EnemyBee.h"



EnemyBee::EnemyBee(int modelHandle):
	EnemyBase(modelHandle)
{
	/*ˆ—–³‚µ*/
}

EnemyBee::~EnemyBee()
{
	/*ˆ—–³‚µ*/
}

void EnemyBee::Update()
{
	AdjustmentModelPos();
}
