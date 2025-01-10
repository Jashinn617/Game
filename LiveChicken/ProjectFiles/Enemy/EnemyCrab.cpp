#include "EnemyCrab.h"

EnemyCrab::EnemyCrab(int modelHandle):
	EnemyBase(modelHandle)
{
	/*ˆ—–³‚µ*/
}

EnemyCrab::~EnemyCrab()
{
	/*ˆ—–³‚µ*/
}

void EnemyCrab::Update()
{
	AdjustmentModelPos();
}
