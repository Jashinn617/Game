#include "Boss.h"
#include "../Player.h"
#include "../Attack/Vomit.h"
#include "../Attack/AirBullet.h"
#include "../Attack/Inhale.h"
#include "../Camera.h"
#include "../Game.h"
#include "DxLib.h"

#include <cmath>

namespace
{
	// アニメーションスピード
	constexpr float kSinSpeed = 0.25f;
	// アニメーションの振幅
	constexpr float kAnimationSize = 6.0f;

	// ボスのサイズ
	constexpr float kWidth = 128;
	constexpr float kHeight = 128;

	// 弾のサイズ
	constexpr float kCannonWidth = 32;
	constexpr float kCannonHeight = 32;

	// 花のサイズ
	constexpr float kFlowerWidth = 64;
	constexpr float kFlowerHeight = 64;

	// HP
	constexpr int kHp = 5;

	// 定位置
	constexpr float kFixedPositionX = 1780;
	constexpr float kFixedPositionY = 770;

	// 攻撃間隔
	constexpr float kAttackInterval = 120;

	// 花が吸い込まれる速度
	constexpr float kFlowerInhaleSpeed = 5.5f;

	// 花が消えるフレーム数
	constexpr float kFloerInterval = 120;
}


Boss::Boss() :
	m_pos(0, 0),
	m_x(0),
	m_y(0),
	m_leftFlowerPos(0, 0),
	m_leftFlowerX(0),
	m_leftFlowerY(0),
	m_rightFlowerPos(0, 0),
	m_rightFlowerX(0),
	m_rightFlowerY(0),
	m_w(0),
	m_h(0),
	m_normalHandle(-1),
	m_preparationHandle(-1),
	m_damageHandle(-1),
	m_flowerHandle(-1),
	m_hitSeHandle(-1),
	m_landingSeHandle(-1),
	m_fellSeHandle(-1),
	m_preparationSeHandle(-1),
	m_endSeHandle(-1),
	m_sinCount(0),
	m_animationHeight(0),
	m_isStandBy(true),
	m_attackIntervalFrame(0),
	m_isCenterJump(false),
	m_isLeftJump(false),
	m_isRightJump(false),
	m_jumpFrame(0),
	m_isJumpPreparation(false),
	m_isJumpRise(false),
	m_isJumpFell(false),
	m_isJumpFixedPos(false),
	m_isJumpFixedEndPos(false),
	m_isLeftFlower(false),
	m_isRightFlower(false),
	m_flowerFrame(0),
	m_isFlowerInhale(false),
	m_flowerRotate(0),
	m_isDamage(false),
	m_damageFrame(0),
	m_hp(0),
	m_isBossEnd(false),
	m_bossRotate(0),
	m_BossEndFrame(0),
	m_isEndSe(false),
	m_isExist(false),
	m_Color(0xffffff)
{
	/*グラフィックのロード*/
	m_normalHandle = LoadGraph(_T("data/Enemy/BossNormal.png"));
	m_preparationHandle = LoadGraph(_T("data/Enemy/BossPreparation.png"));
	m_damageHandle = LoadGraph(_T("data/Enemy/BossDamage.png"));
	m_flowerHandle = LoadGraph(_T("data/Object/colorFlower.png"));
	/*SEのロード*/
	m_hitSeHandle = LoadSoundMem(_T("data/Sound/SE/Hit.wav"));
	m_landingSeHandle = LoadSoundMem(_T("data/Sound/SE/BossLanding.wav"));
	m_fellSeHandle = LoadSoundMem(_T("data/Sound/SE/BossFell.wav"));
	m_preparationSeHandle = LoadSoundMem(_T("data/Sound/SE/BossPreparation.wav"));
	m_endSeHandle = LoadSoundMem(_T("data/Sound/SE/BossEnd.wav"));
}

Boss::~Boss()
{
	/*グラフィックのデリート*/
	DeleteGraph(m_normalHandle);
	DeleteGraph(m_preparationHandle);
	DeleteGraph(m_damageHandle);
	DeleteGraph(m_flowerHandle);
	/*SEのデリート*/
	DeleteSoundMem(m_hitSeHandle);
	DeleteSoundMem(m_landingSeHandle);
	DeleteSoundMem(m_fellSeHandle);
	DeleteSoundMem(m_preparationSeHandle);
	DeleteSoundMem(m_endSeHandle);
}

void Boss::Init()
{
	// 初期位置の設定
	m_pos.x = kFixedPositionX;
	m_pos.y = kFixedPositionY;

	// サイズの設定
	m_w = kWidth;
	m_h = kHeight;

	m_leftFlowerPos.y = kFixedPositionY;
	m_rightFlowerPos.y = kFixedPositionY;

	// リスタート用の初期化
	m_damageFrame = 0;
	m_BossEndFrame = 0;
	m_isCenterJump = false;
	m_isLeftJump = false;
	m_isRightJump = false;
	m_isDamage = false;
	m_isExist = true;
	m_isEndSe = false;
	m_isBossEnd = false;
	m_isStandBy = true;
	m_isLeftFlower = false;
	m_isRightFlower = false;
	m_hp = kHp;
	m_bossRotate = 0;
}

void Boss::Update()
{
	// 存在しないエネミーの処理はしない
	if (!m_isExist)return;

	// HPが0になったら消える
	if (m_hp <= 0)
	{
		m_isBossEnd = true;
		m_isDamage = true;
	}
		
	if (m_isBossEnd)
	{
		m_BossEndFrame++;
		m_bossRotate += 0.1f;

		if (m_BossEndFrame >= 30 && m_BossEndFrame < 60)
		{
			if (!CheckSoundMem(m_landingSeHandle))
			{
				PlaySoundMem(m_landingSeHandle, DX_PLAYTYPE_BACK, true);
			}
		}
		if (m_BossEndFrame >= 60 && m_BossEndFrame < 180)
		{
			if (!CheckSoundMem(m_endSeHandle)&&!m_isEndSe)
			{
				PlaySoundMem(m_endSeHandle, DX_PLAYTYPE_BACK, true);
				m_isEndSe = true;
			}
			m_pos.y -= 10;
			m_pos.x += 10;
		}
		if (m_BossEndFrame >= 180)
		{
			m_isExist = false;
		}
	}
	else if (!m_isBossEnd)
	{
		// 画面外に出ないようにする
		if (m_pos.x <= 100)
		{
			m_pos.x = 100;
		}
		if (m_pos.x >= 1800)
		{
			m_pos.x = 1800;
		}
		if (m_isStandBy)
		{
			m_attackIntervalFrame++;
			m_jumpFrame = 0;
			m_Color = 0xffffff;
		}

		if (!m_isLeftFlower && !m_isRightFlower)
		{
			m_flowerFrame = 0;
		}

		// 花が消えるまでのフレーム
		if (m_isLeftFlower || m_isRightFlower)
		{
			m_flowerFrame++;
		}
		// 一定時間すると花が消えるが、吸い込まれている途中は消えない
		if (m_flowerFrame > kFloerInterval && !m_isFlowerInhale)
		{
			m_isLeftFlower = false;
			m_isRightFlower = false;
		}

		// 攻撃はランダムで決定する
		if (m_attackIntervalFrame >= kAttackInterval)
		{
			m_isStandBy = false;
			int attackPattern = GetRand(2);

			if (attackPattern == 0)
			{
				m_isCenterJump = true;
			}
			else if (attackPattern == 1)
			{
				m_isLeftJump = true;
			}
			else if (attackPattern == 2)
			{
				m_isRightJump = true;
			}
			m_attackIntervalFrame = 0;
		}

		if (m_isCenterJump)
		{
			m_jumpFrame++;
			JumpCenterFellAttack();
		}
		else if (m_isLeftJump)
		{
			m_jumpFrame++;
			JumpLeftFellAttack();
		}
		else if (m_isRightJump)
		{
			m_jumpFrame++;
			JumpRightFellAttack();
		}

		m_isFlowerInhale = false;
		// 当たり判定の更新
		GetRect();
		GetLeftFlowerRect();
		GetRightFlowerRect();
		PlayerCollision();
		VomitCollision();
		InhaleToLeftFlowerCollision();
		InhaleToRightFlowerCollision();
		PlayerToLeftFlowerCollision();
		PlayerToRightFlowerCollision();

		// アニメーション
		m_sinCount += kSinSpeed;
		m_animationHeight = sinf(m_sinCount) * kAnimationSize;

		// 花の回転
		m_flowerRotate += 0.05f;

		// ダメージを受けたときの処理
		if (m_isDamage)
		{
			m_damageFrame++;

			if (m_damageFrame >= 30)
			{
				m_isDamage = false;
				m_damageFrame = 0;
			}
		}
	}


}

void Boss::Draw()
{
	// 存在しないエネミーの描画はしない
	if (!m_isExist)return;

	// 足元中央の座標を左上座標に変換する
	m_x = m_pos.x - kWidth * 0.5f;
	m_y = m_pos.y - kHeight;
	// スクロール量を反映する
	m_x -= m_pCamera->GetScrollX();
	m_y -= m_pCamera->GetScrollY();

	if (m_isJumpPreparation)
	{
		DrawGraph(static_cast<int>(m_x),static_cast<int>(m_y), m_preparationHandle, true);
	}
	else if (m_isDamage)
	{
		DrawRotaGraph(static_cast<int>(m_x + (m_w / 2)), static_cast<int>(m_y + (m_h / 2)), 1.0, m_bossRotate, m_damageHandle, true);
	}
	else
	{
		DrawGraph(static_cast<int>(m_x), static_cast<int>(m_y), m_normalHandle, true);
	}

	FlowerDraw();
}

Rect Boss::GetRect()
{
	m_colRect.top = GetTop();
	m_colRect.left = GetLeft();
	m_colRect.bottom = GetBottom();
	m_colRect.right = GetRight();

	return m_colRect;
}

Rect Boss::GetLeftFlowerRect()
{
	m_leftFlowerColRect.top = GetLeftFlowerTop();
	m_leftFlowerColRect.left = GetLeftFlowerLeft();
	m_leftFlowerColRect.bottom = GetLeftFlowerBottom();
	m_leftFlowerColRect.right = GetLeftFlowerRight();

	return m_leftFlowerColRect;
}

Rect Boss::GetRightFlowerRect()
{
	m_rightFlowerColRect.top = GetRightFlowerTop();
	m_rightFlowerColRect.left = GetRightFlowerLeft();
	m_rightFlowerColRect.bottom = GetRightFlowerBottom();
	m_rightFlowerColRect.right = GetRightFlowerRight();

	return m_rightFlowerColRect;
}

float Boss::GetLeft() const
{
	float x = m_pos.x - m_w * 0.5f;
	x -= m_pCamera->GetScrollX();

	return x;
}

float Boss::GetRight() const
{
	float x = m_pos.x - m_w * 0.5f;
	x -= m_pCamera->GetScrollX();

	return x + m_w;
}

float Boss::GetTop() const
{
	float y = m_pos.y - m_h;
	y -= m_pCamera->GetScrollY();

	return y;
}

float Boss::GetBottom() const
{
	float y = m_pos.y - m_h;
	y -= m_pCamera->GetScrollY();

	return y + m_h;
}

float Boss::GetLeftFlowerLeft() const
{
	float x = m_leftFlowerPos.x - kFlowerWidth * 0.5f;
	x -= m_pCamera->GetScrollX();

	return x;
}

float Boss::GetLeftFlowerRight() const
{
	float x = m_leftFlowerPos.x - kFlowerWidth * 0.5f;
	x -= m_pCamera->GetScrollX();

	return x + kFlowerWidth;
}

float Boss::GetLeftFlowerTop() const
{
	float y = m_leftFlowerPos.y - kFlowerHeight;
	y -= m_pCamera->GetScrollY();

	return y;
}

float Boss::GetLeftFlowerBottom() const
{
	float y = m_leftFlowerPos.y - kFlowerHeight;
	y -= m_pCamera->GetScrollY();

	return y + kFlowerHeight;
}

float Boss::GetRightFlowerLeft() const
{
	float x = m_rightFlowerPos.x - kFlowerWidth * 0.5f;
	x -= m_pCamera->GetScrollX();

	return x;
}

float Boss::GetRightFlowerRight() const
{
	float x = m_rightFlowerPos.x - kFlowerWidth * 0.5f;
	x -= m_pCamera->GetScrollX();

	return x + kFlowerWidth;
}

float Boss::GetRightFlowerTop() const
{
	float y = m_rightFlowerPos.y - kFlowerHeight;
	y -= m_pCamera->GetScrollY();

	return y;
}

float Boss::GetRightFlowerBottom() const
{
	float y = m_rightFlowerPos.y - kFlowerHeight;
	y -= m_pCamera->GetScrollY();

	return y + kFlowerHeight;

}

void Boss::PlayerCollision()
{
	if (m_pPlayer->IsMiss())return;
	// プレイヤーに当たった場合は、プレイヤーにダメージを与える
	if (m_colRect.isCol(m_pPlayer->GetColRect()))
	{
		m_pPlayer->OnDamage();
	}
}

void Boss::PlayerToLeftFlowerCollision()
{
	if (!m_isLeftFlower)return;
	if (!m_isFlowerInhale)return;
	if (m_leftFlowerColRect.isCol(m_pPlayer->GetColRect()))
	{
		m_pPlayer->OnGlup();
		m_isLeftFlower = false;
	}
}

void Boss::PlayerToRightFlowerCollision()
{
	if (!m_isRightFlower)return;
	if (!m_isFlowerInhale)return;
	if (m_rightFlowerColRect.isCol(m_pPlayer->GetColRect()))
	{
		m_pPlayer->OnGlup();
		m_isRightFlower = false;
	}
}

void Boss::VomitCollision()
{
	if (!m_pVomit->IsExsist())return;
	if (m_isDamage)return;
	if (m_colRect.isCol(m_pVomit->GetRect()))
	{
		// SEを鳴らす
		PlaySoundMem(m_hitSeHandle, DX_PLAYTYPE_BACK, true);
		// Hpを減らす
		m_isDamage = true;
		m_hp--;
		// 弾を消す
		m_pVomit->FalseExist();
	}
}

void Boss::InhaleToLeftFlowerCollision()
{
	if (!m_isLeftFlower)return;
	if (!m_pInhale->IsExsist())return;
	if (m_leftFlowerColRect.isCol(m_pInhale->GetRect()))
	{
		// 吸い込み処理
		Vec2 F = m_leftFlowerPos;
		Vec2 P = Vec2(m_pPlayer->GetX(), m_pPlayer->GetY());
		Vec2 FP = P.operator-=(F);
		Vec2 Direction = FP.GetNormalize();
		Vec2 Velocity = Direction.operator*=(kFlowerInhaleSpeed);

		F.operator+=(Velocity);

		m_leftFlowerPos = F;
		m_isFlowerInhale = true;
	}
}

void Boss::InhaleToRightFlowerCollision()
{
	if (!m_isRightFlower)return;
	if (!m_pInhale->IsExsist())return;
	if (m_rightFlowerColRect.isCol(m_pInhale->GetRect()))
	{
		// 吸い込み処理
		Vec2 F = m_rightFlowerPos;
		Vec2 P = Vec2(m_pPlayer->GetX(), m_pPlayer->GetY());
		Vec2 FP = P.operator-=(F);
		Vec2 Direction = FP.GetNormalize();
		Vec2 Velocity = Direction.operator*=(kFlowerInhaleSpeed);

		F.operator+=(Velocity);

		m_rightFlowerPos = F;
		m_isFlowerInhale = true;
	}
}

void Boss::FlowerDraw()
{
	if (m_isLeftFlower)
	{
		// 足元中央を左上座標に変換する
		float leftX = m_leftFlowerPos.x - kFlowerWidth * 0.5f;
		float leftY = m_leftFlowerPos.y - kFlowerHeight;
		// スクロール量を反映させる
		leftX -= m_pCamera->GetScrollX();
		leftY -= m_pCamera->GetScrollY();

		DrawRotaGraph(static_cast<int>(leftX), static_cast<int>(leftY), 1.0, m_flowerRotate, m_flowerHandle, true);
	}
	if (m_isRightFlower)
	{

		// 足元中央を左上座標に変換する
		float rightX = m_rightFlowerPos.x - kFlowerWidth * 0.5f;
		float rightY = m_rightFlowerPos.y - kFlowerHeight;
		// スクロール量を反映させる
		rightX -= m_pCamera->GetScrollX();
		rightY -= m_pCamera->GetScrollY();

		DrawRotaGraph(static_cast<int>(rightX), static_cast<int>(rightY), 1.0, m_flowerRotate, m_flowerHandle, true);
	}
}

/*攻撃*/
void Boss::JumpCenterFellAttack()
{
	// 中央に飛んで落ちていく
	// 地面についたら周りに吸い込める花を二つ落とす
	m_isJumpPreparation = true;

	// 準備
	if (m_isJumpPreparation && m_jumpFrame >= 0 && m_jumpFrame < 30)
	{
		if (!CheckSoundMem(m_preparationSeHandle))
		{
			PlaySoundMem(m_preparationSeHandle, DX_PLAYTYPE_BACK, true);
		}
		m_Color = 0xff0000;
		m_isJumpRise = true;
	}
	// 飛ぶ
	if (m_isJumpRise && m_jumpFrame >= 30)
	{
		m_isJumpPreparation = false;
		m_Color = 0x4069e1;
		m_pos.x -= 15;
		m_pos.y -= 10;
		if (m_isJumpRise && m_pos.y <= 200)
		{
			StopSoundMem(m_preparationSeHandle);
			m_pos.y = 200;
			m_isJumpFell = true;
		}
	}
	// 落ちる
	if (m_isJumpFell && m_pos.y <= kFixedPositionY)
	{
		if (!CheckSoundMem(m_fellSeHandle))
		{
			PlaySoundMem(m_fellSeHandle, DX_PLAYTYPE_BACK, true);
		}
		m_isJumpPreparation = false;
		m_isJumpRise = false;
		m_pos.y += 20;
		m_isJumpFixedPos = true;
		m_jumpFrame = 30;
		if (m_pos.y >= kFixedPositionY)
		{
			StopSoundMem(m_fellSeHandle);
			if (!CheckSoundMem(m_landingSeHandle))
			{
				PlaySoundMem(m_landingSeHandle, DX_PLAYTYPE_BACK, true);
			}
			m_pos.y = kFixedPositionY + 1;

			// 花の位置の調整
		// 足元中央を左上座標に変換する
			float x = m_pos.x - m_w * 0.5f;
			float y = m_pos.y - m_h;
			// スクロール量を反映させる
			x -= m_pCamera->GetScrollX();
			y -= m_pCamera->GetScrollY();

			m_leftFlowerPos.x = x - kFlowerWidth;
			m_rightFlowerPos.x = x + kWidth + kFlowerWidth;
			// 左右に花を落とす
			m_isLeftFlower = true;
			m_isRightFlower = true;
		}
	}
	// 定位置に戻る
	if (m_isJumpFixedPos && m_jumpFrame >= 60 && m_pos.y >= kFixedPositionY)
	{
		m_isJumpPreparation = false;
		m_isJumpFell = false;
		m_pos.x += 8;
		m_isJumpFixedEndPos = true;
	}
	if (m_isJumpFixedEndPos && m_pos.x >= kFixedPositionX)
	{
		m_isJumpPreparation = false;
		m_isJumpFixedPos = false;
		m_isStandBy = true;
		m_isCenterJump = false;
		m_isJumpFixedEndPos = false;
	}
}

void Boss::JumpLeftFellAttack()
{
	// 中央に飛んで落ちていく
	// 地面についたら周りに吸い込める花を二つ落とす
	m_isJumpPreparation = true;

	// 準備
	if (m_isJumpPreparation && m_jumpFrame >= 0 && m_jumpFrame < 30)
	{
		if (!CheckSoundMem(m_preparationSeHandle))
		{
			PlaySoundMem(m_preparationSeHandle, DX_PLAYTYPE_BACK, true);
		}
		m_Color = 0xff0000;
		m_isJumpRise = true;
	}
	// 飛ぶ
	if (m_isJumpRise && m_jumpFrame >= 30)
	{
		m_isJumpPreparation = false;
		m_Color = 0x4069e1;
		m_pos.x -= 25;
		m_pos.y -= 10;
		if (m_isJumpRise && m_pos.y <= 200)
		{
			StopSoundMem(m_preparationSeHandle);
			m_pos.y = 200;
			m_isJumpFell = true;
		}
	}
	// 落ちる
	if (m_isJumpFell && m_pos.y <= kFixedPositionY)
	{
		if (!CheckSoundMem(m_fellSeHandle))
		{
			PlaySoundMem(m_fellSeHandle, DX_PLAYTYPE_BACK, true);
		}
		m_isJumpPreparation = false;
		m_isJumpRise = false;
		m_pos.y += 20;
		m_isJumpFixedPos = true;
		m_jumpFrame = 30;
		if (m_pos.y >= kFixedPositionY)
		{
			StopSoundMem(m_fellSeHandle);
			if (!CheckSoundMem(m_landingSeHandle))
			{
				PlaySoundMem(m_landingSeHandle, DX_PLAYTYPE_BACK, true);
			}
			m_pos.y = kFixedPositionY + 1;
			// 花の位置の調整
		// 足元中央を左上座標に変換する
			float x = m_pos.x - m_w * 0.5f;
			float y = m_pos.y - m_h;
			// スクロール量を反映させる
			x -= m_pCamera->GetScrollX();
			y -= m_pCamera->GetScrollY();

			m_leftFlowerPos.x = x - kFlowerWidth;
			m_rightFlowerPos.x = x + kWidth + kFlowerWidth;
			// 左右に花を落とす
			m_isLeftFlower = true;
			m_isRightFlower = true;
		}
	}
	// 定位置に戻る
	if (m_isJumpFixedPos && m_jumpFrame >= 60 && m_pos.y >= kFixedPositionY)
	{
		m_isJumpPreparation = false;
		m_isJumpFell = false;
		m_pos.x += 8;
		m_isJumpFixedEndPos = true;
	}
	if (m_isJumpFixedEndPos && m_pos.x >= kFixedPositionX)
	{
		m_isJumpPreparation = false;
		m_isJumpFixedPos = false;
		m_isStandBy = true;
		m_isLeftJump = false;
		m_isJumpFixedEndPos = false;
	}
}

void Boss::JumpRightFellAttack()
{
	// 中央に飛んで落ちていく
	// 地面についたら周りに吸い込める花を二つ落とす
	m_isJumpPreparation = true;

	// 準備
	if (m_isJumpPreparation && m_jumpFrame >= 0 && m_jumpFrame < 30)
	{
		if (!CheckSoundMem(m_preparationSeHandle))
		{
			PlaySoundMem(m_preparationSeHandle, DX_PLAYTYPE_BACK, true);
		}
		m_Color = 0xff0000;
		m_isJumpRise = true;
	}
	// 飛ぶ
	if (m_isJumpRise && m_jumpFrame >= 30)
	{
		m_isJumpPreparation = false;
		m_Color = 0x4069e1;
		m_pos.x -= 5;
		m_pos.y -= 10;
		if (m_isJumpRise && m_pos.y <= 200)
		{
			StopSoundMem(m_preparationSeHandle);
			m_pos.y = 200;
			m_isJumpFell = true;
		}
	}
	// 落ちる
	if (m_isJumpFell && m_pos.y <= kFixedPositionY)
	{
		if (!CheckSoundMem(m_fellSeHandle))
		{
			PlaySoundMem(m_fellSeHandle, DX_PLAYTYPE_BACK, true);
		}
		m_isJumpPreparation = false;
		m_isJumpRise = false;
		m_pos.y += 20;
		m_isJumpFixedPos = true;
		m_jumpFrame = 30;
		if (m_pos.y >= kFixedPositionY)
		{
			StopSoundMem(m_fellSeHandle);
			if (!CheckSoundMem(m_landingSeHandle))
			{
				PlaySoundMem(m_landingSeHandle, DX_PLAYTYPE_BACK, true);
			}

			m_pos.y = kFixedPositionY + 1;
			// 花の位置の調整
		// 足元中央を左上座標に変換する
			float x = m_pos.x - m_w * 0.5f;
			float y = m_pos.y - m_h;
			// スクロール量を反映させる
			x -= m_pCamera->GetScrollX();
			y -= m_pCamera->GetScrollY();

			m_leftFlowerPos.x = x - kFlowerWidth;
			m_rightFlowerPos.x = x + kWidth + kFlowerWidth;
			// 左右に花を落とす
			m_isLeftFlower = true;
			m_isRightFlower = true;
		}
	}
	// 定位置に戻る
	if (m_isJumpFixedPos && m_jumpFrame >= 60 && m_pos.y >= kFixedPositionY)
	{
		m_isJumpPreparation = false;
		m_isJumpFell = false;
		m_pos.x += 8;
		m_isJumpFixedEndPos = true;
	}
	if (m_isJumpFixedEndPos && m_pos.x >= kFixedPositionX)
	{
		m_isJumpPreparation = false;
		m_isJumpFixedPos = false;
		m_isStandBy = true;
		m_isRightJump = false;
		m_isJumpFixedEndPos = false;
	}
}
