#include "Camera.h"
#include "Player.h"
#include "Map.h"
#include <vector>

Camera::Camera():
	m_pos{0,0,0}
{
	// 奥行0.1～1000までをカメラの描画範囲にする
	SetCameraNearFar(0.1f, 1000.0f);
}

Camera::~Camera()
{
	/*処理無し*/
}

void Camera::Update(const Player& player)
{
	// 大きさ0.5のマップチップを、課題当初のStageData1ColNum個(16)配置する
	// プレイヤーの地面のY位置を0としたいので、地上部のマップチップの数は二つ減らした位置で調整
	// その真ん中に表示するので半分を計算で出す
	// プレイヤーのX座標には追従したいのでplayerのXを使う
	VECTOR playerPos = player.GetPos();
	VECTOR cameraPos = VGet(playerPos.x, Map::kChipSize * (16 - 2) * 0.5f, playerPos.z - 10.0f);

	// 注視する視点はカメラと平行にまっすぐz = 0 地点
	VECTOR lookPos = VGet(cameraPos.x, cameraPos.y, 0.0f);

	m_pos = cameraPos;

	// カメラに位置を反映する
	SetCameraPositionAndTarget_UpVecY(m_pos, lookPos);
}
