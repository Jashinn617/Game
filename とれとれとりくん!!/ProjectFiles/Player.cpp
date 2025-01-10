#include "DxLib.h"
#include "Player.h"
#include "Util/Input.h"
#include "Util/HandleManager.h"


Player::Player():
	m_pos{0,0,-kWallZ},
	m_dir(0),
	m_jumpPower(0.0f),
	m_isJump(false),
	m_cameraAngle(0.0f)
{
	/*処理無し*/
}

Player::~Player()
{
	/*処理無し*/
}

void Player::Init(HandleManager& handle)
{
	// モデルを初期位置に戻す
	MV1SetPosition(handle.GetModel("player"), m_pos);

	// モデルの初期の向きを設定する
	MV1SetRotationXYZ(handle.GetModel("player"), VGet(0.0f, kStartModelDir, 0.0f));
}

void Player::Update(Input& input, HandleManager& handle, bool isOperate)
{
	// モデルのスケールを設定する
	MV1SetScale(handle.GetModel("player"), VGet(kScale,kScale,kScale));

	// 操作を受け付けるときだけ、この中の処理をする
	if (isOperate)
	{
		// 回転
		MV1SetRotationXYZ(handle.GetModel("player"), VGet(0.0f, m_dir, 0.0f));
		
		// 移動
		Move(input);
		
		// ジャンプ処理
		Jump(input, handle);
	}
	
	// 重力と地面の処理
	GravityAndGround();

	// 壁の処理
	Wall();

	// モデルの位置設定
	MV1SetPosition(handle.GetModel("player"), m_pos);
}

void Player::Draw(HandleManager& handle) const
{
	// モデルの描画
	MV1DrawModel(handle.GetModel("player"));

	// 当たり判定の表示
#ifdef _DEBUG
	VECTOR pos = MV1GetPosition(handle.GetModel("player"));
	pos.y += 0.1f;
	DrawSphere3D(pos, kRadius, 32, 0x0000ff, 0x0000ff, false);
	// プレイヤーの位置
	DrawFormatString(0, 0, 0xffffff, "(%.2f,%.2f,%.2f)\n", m_pos.x, m_pos.y, m_pos.z);		
#endif // _DEBUG
}

void Player::Move(Input& input)
{
	// アナログスティックを使っての移動
	int analogX = 0;
	int analogY = 0;
	// コントローラーのアナログスティックの入力値を取得する
	GetJoypadAnalogInput(&analogX, &analogY, DX_INPUT_PAD1);

	// アナログスティックの入力の10～80%の範囲を使用する
	VECTOR move = VGet(static_cast<float>(analogX), 0.0f, static_cast<float>(-analogY));
	// ベクトルの長さを取得する
	float len = VSize(move);

	/* ベクトルの長さを0.0～1.0の割合に変換する*/
	float rate = len / kAnalogInputMax;
	// アナログスティックが無効になる範囲を除外する
	rate = (rate - kAnalogRangeMim) / (kAnalogInputMax - kAnalogRangeMim);
	rate = min(rate, 1.0f);
	rate = max(rate, 0.0f);

	// 速度を決定し、移動ベクトルに反映する
	move = VNorm(move);
	float speed = kMaxSpeed * rate;
	move = VScale(move, speed);

	// カメラのいる場所(角度)からコントローラーによる移動方向を決定する
	MATRIX mtx = MGetRotY(-m_cameraAngle - DX_PI_F / 2);
	move = VTransform(move, mtx);

	// 移動方向からプレイヤーの向く方向を決定する
	// 移動していない場合(0ベクトルの場合)は変更しない
	if (VSquareSize(move) > 0.0f)
	{
		m_dir = -atan2f(move.z, move.x) - DX_PI_F;
	}

	// 位置の更新
	m_pos = VAdd(m_pos, move);
}


void Player::Jump(Input& input, HandleManager& handle)
{
	// ジャンプ処理
	if (input.IsTriggered("A") && !m_isJump)
	{
		PlaySoundMem(handle.GetSound("jumpSE"), DX_PLAYTYPE_BACK);
		m_jumpPower = kJumpPower;
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

void Player::Wall()
{
	// 後ろ
	if (m_pos.z <= -kWallZ)
	{
		m_pos.z = -kWallZ;
	}
	// 前
	if (m_pos.z >= kWallZ)
	{
		m_pos.z = kWallZ;
	}
	// 右
	if (m_pos.x >= kWallX)
	{
		m_pos.x = kWallX;
	}
	// 左
	if (m_pos.x <= -kWallX)
	{
		m_pos.x = -kWallX;
	}
}