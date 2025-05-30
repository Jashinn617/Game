#pragma once

#include <memory>
#include <vector>

class ObjectBase;

class Collision
{
public:
	Collision();
	~Collision();

	void Init();

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="my"></param>
	/// <param name="target"></param>
	void Update(ObjectBase* my, ObjectBase* target);
};

