#pragma once
#include "EnemyBase.h"
class EnemyCrab : public EnemyBase
{
public:
	EnemyCrab(int modelHandle);
	virtual ~EnemyCrab();
	/// <summary>
	/// XV
	/// </summary>
	void Update() override;
};

