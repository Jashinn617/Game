#include "Player.h"
#include "Input.h"

Player::Player() :
	m_pos{ -10,kGroundPosY,0 },
	m_move{ 0,0,0 },
	m_modelHandle(-1),
	m_rad(kRadius),
	m_jumpPower(0.0f),
	m_isJump(false),
	m_jumpSe(-1)
{
	/*3Dモデルのロード*/
	m_modelHandle = MV1LoadModel("Data/Model/chicken.mv1");
	/*SEのロード*/
	m_jumpSe = LoadSoundMem("Data/Sound/SE/Jump.mp3");
}

Player::~Player()
{
	/*モデルのデリート*/
	MV1DeleteModel(m_modelHandle);
	/*SEのデリート*/
	DeleteSoundMem(m_jumpSe);
}

void Player::Update(Input& input)
{
	// 移動値の初期化
	m_move = VGet(0, 0, 0);

	// モデルのスケールを決定する
	MV1SetScale(m_modelHandle, VGet(kScale, kScale, kScale));
	// 回転
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, kDirY, 0.0f));

	// 右移動
	m_move = VAdd(m_move, VGet(0.05f, 0, 0));

	// 重力と地面の処理
	GravityAndGround();

	// ジャンプ処理
	Jump(input);

	// 位置の更新
	m_pos = VAdd(m_pos, m_move);

	// モデルの位置設定
	MV1SetPosition(m_modelHandle, m_pos);
}

void Player::Draw() const
{
	// 3Dモデルの描画
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG
	VECTOR pos = MV1GetPosition(m_modelHandle);
	pos.x -= 0.1f;
	DrawSphere3D(pos, kRadius, 32, 0x0000ff, 0x0000ff, false);
#endif // _DEBUG
}

const VECTOR& Player::GetPos() const
{
	return MV1GetPosition(m_modelHandle);
}

void Player::Jump(Input& input)
{
	// ジャンプ処理
	if (input.IsTriggered("A") && !m_isJump)
	{
		PlaySoundMem(m_jumpSe, DX_PLAYTYPE_BACK);
		m_jumpPower = kJumpHeight;
		m_isJump = true;
	}
}

void Player::GravityAndGround()
{
	// 重力
	m_pos = VAdd(m_pos, VGet(0.0f, m_jumpPower, 0.0f));
	m_jumpPower -= kGravity;

	// 地面
	if (m_pos.y <= kGroundPosY)
	{
		m_pos.y = kGroundPosY;
		m_jumpPower = 0;
		m_isJump = false;
	}
}
