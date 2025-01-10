#include "EnemyBase.h"

EnemyBase::EnemyBase(int modelHandle):
	m_modelHandle(-1),
	m_pos{0,0,0},
	m_rad(kRadius)
{
	// モデルの読み込み
	m_modelHandle = MV1DuplicateModel(modelHandle);
}

EnemyBase::~EnemyBase()
{
	// モデルのデリート
	MV1DeleteModel(m_modelHandle);
}

void EnemyBase::Update()
{
	/*処理無し*/
}

void EnemyBase::Draw()
{
	// ３Ｄモデルの描画
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG

	VECTOR pos =VAdd(MV1GetPosition(m_modelHandle),VGet(0,0.3,0));

	DrawSphere3D(pos, kRadius, 32, 0xff0000, 0xff0000, false);

#endif // _DEBUG
}

const VECTOR& EnemyBase::GetPos() const
{
	return MV1GetPosition(m_modelHandle);
}

void EnemyBase::AdjustmentModelPos()
{
	// モデルのスケールを決定する
	MV1SetScale(m_modelHandle, VGet(kScale, kScale, kScale));

	// モデルの位置設定
	MV1SetPosition(m_modelHandle, m_pos);

	// 回転
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, kDirY, 0.0f));
}



