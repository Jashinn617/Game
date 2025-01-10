#pragma once
#include "EnemyBase.h"
class EnemySkull : public EnemyBase
{
public:
	EnemySkull(int modelHandle);
	virtual ~EnemySkull();
	/// <summary>
	/// XV
	/// </summary>
	void Update();
};

