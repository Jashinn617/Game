#include "DxLib.h"
#include "Camera.h"
#include "Player.h"

#include "Util/Input.h"

Camera::Camera(shared_ptr<Player> player):
	m_pos{0,0,0},
	m_angle(-DX_PI_F/2),
	m_pPlayer(player)
{
	
}

Camera::~Camera()
{
	/*処理無し*/
}

void Camera::Update(Input& input, bool isOperate)
{
	// 操作を受け付けるときだけ、この中の処理をする
	if (isOperate)
	{
		// カメラの回転
		if (input.IsPressing("R"))
		{
			m_angle += kSpeed;
		}
		if (input.IsPressing("L"))
		{
			m_angle -= kSpeed;
		}
	}
	
	m_pPlayer->SetCameraAngle(m_angle);

	// カメラの描画範囲の設定
	SetCameraNearFar(kCameraNear, kCameraFar);
	VECTOR playerPos = m_pPlayer->GetPos();

	// カメラの回転処理
	m_pos.x = playerPos.x + cosf(m_angle) * kCameraDist;
	m_pos.y = kCameraHeight;
	m_pos.z = playerPos.z + sinf(m_angle) * kCameraDist;

	VECTOR targetPos = VGet(playerPos.x, 1.0f, playerPos.z);

	// カメラの位置設定
	SetCameraPositionAndTarget_UpVecY(m_pos, targetPos);
}
