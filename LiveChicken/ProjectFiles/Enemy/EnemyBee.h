#pragma once
#include "EnemyBase.h"

class EnemyBee : public EnemyBase
{
public:
	EnemyBee(int modelHandle);
	virtual ~EnemyBee();
	/// <summary>
	/// XV
	/// </summary>
	void Update() override;
};

