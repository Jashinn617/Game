#include "DxLib.h"
#include "RecoveredItem.h"
#include "WorldSprite.h"
#include "Player.h"
#include "Util/Game.h"
#include "Util/HandleManager.h"

#include <cassert>
#include <cmath>

RecoveredItem::RecoveredItem(shared_ptr<Player> pPlayer):
	m_pos{0,0,0},
	m_modelHandle(-1),
	m_w(0),
	m_h(0),
	m_isExist(true),
	m_sinCount(0),
	m_sinPosY(0),
	m_pPlayer(pPlayer)
{
	/*処理無し*/
}

RecoveredItem::~RecoveredItem()
{
	/*処理無し*/
}

void RecoveredItem::Init(float x, float z, HandleManager& handle)
{
	// 同じモデルで複数作るため、同じデータを使用するモデルハンドルを作成しておく
	m_modelHandle = MV1DuplicateModel(handle.GetModel("item"));

	int rand = GetRand(1);
	if (rand == 0)
	{
		m_pos = VGet(x, kDownPosY, z);
	}
	else if (rand == 1)
	{
		m_pos = VGet(x, kUpPosY, z);
	}
}

void RecoveredItem::Update(HandleManager& handle)
{
	if (!m_isExist) return;

	// モデルのスケールを設定する
	MV1SetScale(m_modelHandle, VGet(kScale, kScale, kScale));
	// 回転
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f,0.0f, 0.0f));

	// 当たり判定
	CollisionToPlayer(m_pPlayer->GetPos(), m_pPlayer->GetRadius(),handle);
	// 移動
	m_sinCount += kSinSpeed;
	m_sinPosY = sinf(m_sinCount) * kMoveSwing;
	m_pos.y += m_sinPosY;

	MV1SetPosition(m_modelHandle, m_pos);
}

void RecoveredItem::Draw(HandleManager& handle)
{
	if (!m_isExist) return;

	MV1DrawModel(m_modelHandle);

	// 当たり判定の表示
#ifdef _DEBUG
	VECTOR pos = m_pos;
	pos.y -= 0.1f;
	DrawSphere3D(pos, kRadius, 32, 0xff0000, 0xff0000, false);
#endif // _DEBUG
}

void RecoveredItem::CollisionToPlayer(VECTOR pPos, float pRad, HandleManager& handle)
{
	// プレイヤーとアイテムの半径の合計
	float rad = kRadius + pRad;
	// プレイヤーとアイテムの距離の計算
	VECTOR dirVec = VSub(m_pos, pPos);
	float dir = VSize(dirVec);
	
	// プレイヤーと当たっているかどうか
	if (dir < rad)
	{
		PlaySoundMem(handle.GetSound("itemChatchSE"), DX_PLAYTYPE_BACK);
		m_isExist = false;

		// デバック用の表示
#ifdef _DEBUG
		printfDx("当たった");
#endif // _DEBUG
		// 当たった時にSEを鳴らして消える
	}
}
