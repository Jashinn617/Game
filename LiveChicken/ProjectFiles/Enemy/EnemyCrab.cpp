#include "EnemyCrab.h"

EnemyCrab::EnemyCrab(int modelHandle):
	EnemyBase(modelHandle)
{
	/*処理無し*/
}

EnemyCrab::~EnemyCrab()
{
	/*処理無し*/
}

void EnemyCrab::Update()
{
	AdjustmentModelPos();
}
