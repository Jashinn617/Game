#pragma once
#include <memory>
#include "../Input.h"
#include <math.h>

using namespace std;

class SceneBase:public enable_shared_from_this<SceneBase>
{
public:
	SceneBase(){}
	virtual ~SceneBase() {};

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力</param>
	/// <returns>現在のシーン</returns>
	virtual shared_ptr<SceneBase> Update(Input& input) = 0;
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;
	/// <summary>
	/// シーン終了時の処理
	/// </summary>
	virtual void End() = 0;
};

