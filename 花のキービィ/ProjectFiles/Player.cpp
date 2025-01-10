#include "Player.h"
#include "DxLib.h"
#include "Game.h"
#include "util/Input.h"
#include "Attack/Vomit.h"
#include "Attack/AirBullet.h"
#include "Attack/Inhale.h"
#include "Object/Needle.h"
#include "Camera.h"

namespace
{
	// プレイヤーのスピード
	constexpr int m_Speed = 8;
	// グラフィックの切り取りサイズ
	constexpr int kWidth = 68;
	constexpr int kHeight = 67;
	// 重力
	constexpr float kGravity = 0.6f;
	// ホバリング時の重力
	constexpr float kHoveringGravity = 0.15f;
	// ジャンプ力
	constexpr float kJumpAcc = -10.0f;
	// ホバリング時のジャンプ力
	constexpr float kJumpHoveringAcc = -4.0f;
	// 弾のスピード
	constexpr int kVomitSpeed = 10;
	constexpr int kAirSpeed = 15;
	// プレイヤーの最大HP
	constexpr int kMaxHp = 6;
	// プレイヤーの最大残機
	constexpr int kMaxRemainingLives = 3;
	// アニメーション
	constexpr int kUseFrame[] = { 0,1,2,1 };
	// 1コマのフレーム数
	constexpr int kAnimFrameNum = 12;
	// １サイクルのフレーム数
	constexpr int kAnimFrameCycle = _countof(kUseFrame) * kAnimFrameNum;
	// ダメージ演出のフレーム数
	constexpr int kDamegeFrame = 60;

	// ダンサーの数
	constexpr int kDancerNum = 3;

	// ダンスの切り取りサイズ
	constexpr int kDancerWidth1 = 72;
	constexpr int kDanceHeight1 = 70;
	constexpr int kDancerWidth2 = 43;
	constexpr int kDanceHeight2 = 48;

	// ダンスのアニメーション
	constexpr int kDanceUseFrame[]{ 0,1,0 };
	// 1コマのフレーム数
	constexpr int kDanceAnimFrameNum = 12;
	// １サイクルのフレーム数
	constexpr int kDanceAnimFrameCycle = _countof(kUseFrame) * kDanceAnimFrameNum;
	// スピード
	constexpr float kDanceSinSpeed1 = 0.1f;
	constexpr float kDanceSinSpeed2 = 0.25f;
	// 揺れ幅
	constexpr float kDanceAnimSwing1 = -200;
	constexpr float kDanceAnimSwing2 = 80;
}

Player::Player() :
	m_pos(0,0),
	m_move(0,0),
	m_condition(kFall),
	m_dir(kDirRight),
	m_x(0),
	m_y(0),
	m_w(64),
	m_h(64),
	m_walkHandle(-1),
	m_inhaleHandle(-1),
	m_hoveringHandle(-1),
	m_gulpHandle(-1),
	m_jumpHandle(-1),
	m_missHandle(-1),
	m_danceHandle1(-1),
	m_danceHandle2(-1),
	m_clearBgmHandle(-1),
	m_inhaleSeHandle(-1),
	m_jumpSeHandle(-1),
	m_damageSeHandle(-1),
	m_fallSpeed(0.0f),
	m_jumpCount(0),
	m_hp(kMaxHp),
	m_remainingLives(kMaxRemainingLives),
	m_isGlup(false),
	m_isVomit(false),
	m_vomitFrame(0),
	m_animFrame(0),
	m_damageFrame(0),
	m_isDamage(false),
	m_isMiss(false),
	m_missFrame(0),
	m_missRotate(0.0f),
	m_isFellMiss(false),
	m_isDamageMiss(false),
	m_isDance(false),
	m_isFlowerMove(false),
	m_eventFrame(0),
	m_danceRotate(0),
	m_rotatePattern(0),
	m_danceSinCount(0),
	m_danceAnimationPosY(0),
	m_choreography(kRotate),
	m_danceAnimFrame(0),
	m_moveEventX(0),
	m_moveEventY(0),
	m_moveEventFrame(0)
{
	/*グラフィックのロード*/
	m_walkHandle = LoadGraph(_T("data/Player/Walk.png"));
	m_inhaleHandle = LoadGraph(_T("data/Player/Inhale.png"));
	m_gulpHandle = LoadGraph(_T("data/Player/Gulp.png"));
	m_hoveringHandle = LoadGraph(_T("data/Player/Hovering.png"));
	m_jumpHandle = LoadGraph(_T("data/Player/Jump.png"));
	m_missHandle = LoadGraph(_T("data/Player/Miss.png"));
	m_danceHandle1 = LoadGraph(_T("data/Event/Dance1.png"));
	m_danceHandle2 = LoadGraph(_T("data/Event/Dance2.png"));

	/*BGMのロード*/
	m_clearBgmHandle = LoadSoundMem(_T("data/Sound/SE/Clear.wav"));

	/*SEのロード*/
	m_inhaleSeHandle = LoadSoundMem(_T("data/Sound/SE/Inhale.wav"));
	m_jumpSeHandle = LoadSoundMem(_T("data/Sound/SE/Jump.mp3"));
	m_attackSeHandle = LoadSoundMem(_T("data/Sound/SE/Vomit.mp3"));
	m_damageSeHandle = LoadSoundMem(_T("data/Sound/SE/Damage.wav"));
}

Player::~Player()
{
	/*グラフィックのデリート*/
	DeleteGraph(m_walkHandle);
	DeleteGraph(m_inhaleHandle);
	DeleteGraph(m_gulpHandle);
	DeleteGraph(m_hoveringHandle);
	DeleteGraph(m_jumpHandle);
	DeleteGraph(m_missHandle);
	DeleteGraph(m_danceHandle1);
	DeleteGraph(m_danceHandle2);
	
	/*SEのデリート*/
	DeleteSoundMem(m_inhaleSeHandle);
	DeleteSoundMem(m_jumpSeHandle);
	DeleteSoundMem(m_attackSeHandle);
	DeleteSoundMem(m_damageSeHandle);
}

void Player::Init()
{
	// リトライ時の数値の初期化
	m_isMiss = false;
	m_missFrame = 0;
	m_dir = kDirRight;
	m_condition = kFall;
	m_damageFrame = 0;
	m_isDamageMiss = false;
	m_isFellMiss = false;
	m_isDance = false;
	m_isFlowerMove = false;
	m_isGlup = false;
	m_isVomit = false;
	m_eventFrame = 0;

	// 攻撃の初期化
	m_pAirBullet->Init();
	m_pInhale->Init();
	m_pVomit->Init();
}


void Player::Draw()
{
	// ダメージ時の点滅
	if (m_damageFrame % 4 >= 2)return;

	// 足元中央の座標を左上座標に変換する
	m_x = m_pos.x - kWidth * 0.5f;
	m_y = m_pos.y - kHeight;
	// スクロール量を反映する
	m_x -= m_pCamera->GetScrollX();
	m_y -= m_pCamera->GetScrollY();
	// アニメーション
	int animFrame = m_animFrame / kAnimFrameNum;
	int srcX = kUseFrame[animFrame] * kWidth;
	int srcY = kHeight * m_dir;

	// 現在の状態によって画像が切り替わる
	switch (m_condition)
	{
	case Player::kMove:
		DrawRectGraph(static_cast<int>(m_x), static_cast<int>(m_y + 5), srcX, srcY, kWidth, kHeight, m_walkHandle, true);
		break;
	case Player::kInhale:
		DrawRectGraph(static_cast<int>(m_x), static_cast<int>(m_y + 5), srcX, srcY, kWidth, kHeight, m_inhaleHandle, true);
		break;
	case Player::kVomit:
		DrawRectGraph(static_cast<int>(m_x), static_cast<int>(m_y + 5), srcX, srcY, kWidth, kHeight, m_inhaleHandle, true);
		break;
	case Player::kGulp:
		DrawRectGraph(static_cast<int>(m_x), static_cast<int>(m_y + 5), srcX, srcY, kWidth, kHeight, m_gulpHandle, true);
		break;
	case Player::kJump:
		DrawRectGraph(static_cast<int>(m_x), static_cast<int>(m_y + 5), 0, srcY, kWidth, kHeight, m_jumpHandle, true);
		break;
	case Player::kHovering:
		DrawRectGraph(static_cast<int>(m_x), static_cast<int>(m_y + 5), srcX, srcY, kWidth, kHeight, m_hoveringHandle, true);
		break;
	case Player::kFall:
		DrawRectGraph(static_cast<int>(m_x), static_cast<int>(m_y + 5), 0, srcY, kWidth, kHeight, m_jumpHandle, true);
		break;
	case Player::kMiss:
		DrawRotaGraph(static_cast<int>(m_x), static_cast<int>(m_y + 5), 1.0, m_missRotate, m_missHandle, true);
		break;
	case Player::kDance:
		DanceDraw();
		break;
	default:
		break;
	}

#ifdef _DEBUG
	// 当たり判定用の矩形
	DrawBox(static_cast<int>(GetColLect()), static_cast<int>(GetColTop()), static_cast<int>(GetColRight()), static_cast<int>(GetColBottom()), 0xff0000, false);
#endif

	// 攻撃の描画
	m_pAirBullet->Draw();
	m_pInhale->Draw();
	m_pVomit->Draw();
}

void Player::Update(Input& input, std::vector<std::vector<int>> mapData, int chipNumX, int chipNumY, int chipWidth, int chipHeight)
{
	// 吸い込み中にミスをしたときに音が鳴り続けないようにする
	if (m_condition != kInhale)
	{
		StopSoundMem(m_inhaleSeHandle);
		m_pInhale->FalseExist();
	}

	// ミスをしたときは操作や当たり判定が無効になる
	if (!m_isMiss)
	{
		// ダメージ演出
		// HPがゼロになった場合はやらない
		if (m_hp >= 1)
		{
			m_damageFrame--;
			if (m_damageFrame < 0)m_damageFrame = 0;
		}

		// 動いているときだけアニメーションする
		bool isMove = false;
		if (m_condition == kHovering)
		{
			isMove = true;
		}
		// 重力
		// ホバリング中は落ちる速度が遅くなる

		if (m_condition == kHovering)
		{
			m_move.y += kHoveringGravity;
		}
		else
		{
			m_move.y += kGravity;
		}		

		// プレイヤーの移動
		m_move.x = 0.0f;
		if (input.IsPressing("left"))
		{
			m_move.x -= m_Speed;

			m_dir = kDirLeft;
			isMove = true;
		}
		if (input.IsPressing("right"))
		{
			m_move.x += m_Speed;

			m_dir = kDirRight;
			isMove = true;
		}
		// ジャンプ&ホバリング
		if (m_condition == kMove || 
			m_condition == kJump || 
			m_condition == kHovering || 
			m_condition == kVomit ||
			m_condition == kFall)
		{
			//  Bボタンかスペースキーをおしたらジャンプする
			if (input.IsTriggered("B"))
			{
				PlaySoundMem(m_jumpSeHandle, DX_PLAYTYPE_BACK, true);
				if (m_jumpCount >= 1)
				{
					m_move.y = kJumpHoveringAcc;
				}
				else
				{
					m_move.y = kJumpAcc;
					m_condition = kJump;
				}
				m_jumpCount++;
				isMove = true;
			}
			// ジャンプを続けて2回以上するとホバリング状態になる
			if (m_jumpCount >= 2)
			{
				m_condition = kHovering;
			}
		}

		/*マップとの当たり判定*/
		m_pos.x += m_move.x;
		// 横から当たったかをチェックする
		Rect chipRect; // 当たったマップチップの矩形
		if (IsMapCollision(mapData, chipNumX, chipNumY, chipWidth, chipHeight, GetRect(), chipRect))
		{
			// 横からぶつかった場合の処理
			if (m_move.x > 0.0f)// 右方向
			{
				m_pos.x = chipRect.left - chipWidth * 0.5f - 1;
			}
			else if (m_move.x < 0.0f)// 左方向
			{
				m_pos.x = chipRect.right + chipWidth * 0.5f + 1;
			}
		}

		m_pos.y += m_move.y;
		// 縦から当たったかをチェックする
		if (IsMapCollision(mapData, chipNumX, chipNumY, chipWidth, chipHeight, GetRect(), chipRect))
		{
			// 縦から当たったかをチェックする
			if (m_move.y > 0.0f)// 下方向(足元)
			{
				m_pos.y = chipRect.top - 1;

				// 移動値をゼロにする
				m_move.y = 0;
				// ジャンプのカウントをゼロにする
				m_jumpCount = 0;

				// 敵を飲み込んでいなかった場合は移動状態に戻す
				if (!m_isGlup && !m_isVomit)
				{
					m_condition = kMove;
				}
			}
			else if (m_move.y < 0.0f)// 上方向(頭)
			{
				m_pos.y = chipRect.bottom + chipHeight + 1;
				// 下に跳ね返す
				m_move.y *= -0.2f;
			}
		}

		// 移動時のみアニメーションをする
		if (isMove)
		{
			// アニメーション
			m_animFrame++;
			if (m_animFrame >= kAnimFrameCycle)
			{
				m_animFrame = 0;
			}
		}

		// 画面外に出ないようにする
		if (GetLeft() <= 0)
		{
			m_pos.x = 0 + kWidth * 0.5f;
		}
		if (GetRight() >= m_pCamera->GetMapWidth())
		{
			m_pos.x = m_pCamera->GetMapWidth() - kWidth * 0.5f;
		}
		if (GetTop() <= 0)
		{
			m_pos.y = 0 + kHeight;
			m_move.y *= -0.2f;
		}
		// HPの最大値を設定しておく
		if (m_hp >= kMaxHp)
		{
			m_hp = kMaxHp;
		}
		// 敵を吐き出してから少しの間すいこみができないようにする
		if (m_isVomit)
		{
			m_vomitFrame++;
		}
		if (m_vomitFrame >= 30)
		{
			m_vomitFrame = 0;
			m_isVomit = false;
		}

		/*攻撃の更新*/
		// すいこみ
		AttackInhale(input);
		m_pInhale->Update();
		// 吐き出し
		AttackVomit(input);
		m_pVomit->Update();
		// 空気弾
		AttackAirBullet(input);
		m_pAirBullet->Update();

		// 当たり判定の更新
		GetColRect();

	}
	else
	{
		// ミスをした時の演出
		m_condition = kMiss;
		m_missFrame++;
		m_missRotate += 0.1f;
		// 落下したときのミス
		// 上がる速度が速い
		if (m_isFellMiss)
		{
			if (m_missFrame <= 30)
			{
				m_pos.y -= 20;
			}
			else if (m_missFrame >= 60)
			{
				m_pos.y += 8;
			}
		
		}
		// ダメージが0になった時のミス
		// 落ちる速度が速い
		if (m_isDamageMiss)
		{
			if (m_missFrame <= 30)// いったん止まる
			{
				m_pos.y += 0;
				m_missRotate = 0.0f;
			}
			if (m_missFrame <= 45)
			{
				m_pos.y -= 8;
			}
			else if (m_missFrame >= 60)
			{
				m_pos.y += 20;
			}
		}
	}
}


bool Player::IsMapCollision(std::vector<std::vector<int>> mapData, int chipNumX, int chipNumY, int chipWidth, int chipHeight, Rect playerRect, Rect& chipRect)
{
	/*マップとの当たり判定*/
	for (int x = 0; x < chipNumX; x++)
	{
		for (int y = 0; y < chipNumY; y++)
		{

			// mapDataが0だったら当たらない
			if (mapData[y][x] == 0)continue;

			float chipLeft = static_cast<float>(chipWidth * x);
			float chipRight = static_cast<float>(chipLeft + chipWidth);
			float chipTop = static_cast<float>(chipHeight * y);
			float chipBottom = static_cast<float>(chipTop + chipHeight);

			// 絶対に当たらない場合次へ
			if (chipLeft > playerRect.right)continue;
			if (chipTop > playerRect.bottom)continue;
			if (chipRight < playerRect.left)continue;
			if (chipBottom < playerRect.top)continue;

			// ぶつかったマップチップの矩形を取得する
			chipRect.left = chipLeft;
			chipRect.right = chipRight;
			chipRect.top = chipTop;
			chipRect.bottom = chipBottom;

			// いずれのチップと当たっていたらtrueを返して終了する(当たっている)
			return true;
		}
	}
	// すべてのチップと当たっていなければfalseを返す(当たっていない)
	return false;
}

void Player::DanceDraw()
{
	// 足元中央の座標を左上座標に変換する
	m_x = m_pos.x - kWidth * 0.5f;
	m_y = m_pos.y - kHeight;
	// スクロール量を反映する
	m_x -= m_pCamera->GetScrollX();
	m_y -= m_pCamera->GetScrollY();
	// アニメーション
	int animFrame = m_danceAnimFrame / kDanceAnimFrameNum;
	int srcX = kDanceUseFrame[animFrame] * kDancerWidth2;
	int srcY = kDanceHeight2 * m_dir;

	for (int i = 0; kDancerNum > i; i++)
	{
		int x = static_cast<int>(m_x) + (200 * i);
		int y = static_cast<int>(m_y) + static_cast<int>(m_danceAnimationPosY);

		if (m_choreography == kRotate)
		{
			DrawRectRotaGraph(x, y, m_rotatePattern * kDancerWidth1, 0, kDancerWidth1, kDanceHeight1, 1.8, m_danceRotate, m_danceHandle1, true);
		}
		else if (m_choreography == kLateralMove)
		{
			DrawRectRotaGraph(x, y, srcX, srcY, kDancerWidth2, kDanceHeight2, 1.8, 0.0, m_danceHandle2, true);
		}
	}
}


void Player::AttackInhale(Input& input)
{
	/* 吸い込みの実装*/
	if ((m_condition == kMove || m_condition == kInhale) && !m_isVomit)
	{
		//  Aボタンをおしたら吸い込む
		if (input.IsPressing("A"))
		{
			// SEを鳴らす
			if (!CheckSoundMem(m_inhaleSeHandle))
			{
				PlaySoundMem(m_inhaleSeHandle, DX_PLAYTYPE_LOOP, true);
			}
			// すいこみ状態にする
			m_condition = kInhale;

			// 吸い込みの処理
			if (m_dir == kDirRight)
			{
				m_pInhale->PutBulletRight();
			}
			if (m_dir == kDirLeft)
			{
				m_pInhale->PutBulletLeft();
			}
		}
		else // 何も押していないとき
		{
			// SEを止める
			StopSoundMem(m_inhaleSeHandle);
			// 移動状態にする
			m_condition = kMove;
			// 存在をfalseにする
			m_pInhale->FalseExist();
		}
	}
	else if (m_condition == kGulp) // ほおばり状態の場合
	{
		// SEを止める
		StopSoundMem(m_inhaleSeHandle);
		// 存在をfalseにする
		m_pInhale->FalseExist();
	}
}

void Player::AttackVomit(Input& input)
{
	// 吐き出し攻撃の実装
	// ほおばり状態のときにAボタンを押したら花を吐き出す
	if (m_condition == kGulp && input.IsTriggered("A"))
	{
			// 同時に吸い込み状態にならないためにm_isVomitをtrueにする
			// m_isVomitがtrueの時は、すいこみ状態にならない
			// 詳細はUpdate内に記載
			m_isVomit = true;
			// SEを鳴らす
			PlaySoundMem(m_attackSeHandle, DX_PLAYTYPE_BACK, true);

			// 吐き出し処理
			if (m_dir == kDirRight)
			{
				m_pVomit->PutBulletRight();
				m_pVomit->SetSpeed(-kVomitSpeed);
			}
			if (m_dir == kDirLeft)
			{
				m_pVomit->PutBulletLeft();
				m_pVomit->SetSpeed(kVomitSpeed);
			}

			// 吐き出し状態にする
			m_condition = kVomit;
			m_isGlup = false;
	}
}

void Player::AttackAirBullet(Input& input)
{
	// 空気弾の処理
	// ホバリング中にAボタンを押したら空気弾を吐き出して落下する
	// 落下中は移動以外何もできない
	if (m_condition == kHovering && input.IsTriggered("A"))
	{
		m_jumpCount = 0;
		// SEを鳴らす
		PlaySoundMem(m_attackSeHandle, DX_PLAYTYPE_BACK, true);

		// 空気弾の処理
		if (m_dir == kDirRight)
		{
			m_pAirBullet->PutBulletRight();
			m_pAirBullet->SetSpeed(-kAirSpeed);
		}
		if (m_dir == kDirLeft)
		{
			m_pAirBullet->PutBulletLeft();
			m_pAirBullet->SetSpeed(kAirSpeed);
		}
		// 吐き出したあとは落下状態になる
		m_condition = kFall;
	}
}

Rect Player::GetRect()
{
	Rect rect;
	rect.top = GetTop();
	rect.left = GetLeft();
	rect.bottom = GetBottom();
	rect.right = GetRight();

	return rect;
}

Rect Player::GetColRect()
{
	m_colRect.top = GetColTop();
	m_colRect.left = GetColLect();
	m_colRect.bottom = GetColBottom();
	m_colRect.right = GetColRight();

	return m_colRect;
}

float Player::GetColLect() const
{
	// 足元中央を左上座標に変換する
	int x = static_cast<int>(m_pos.x - kWidth * 0.5f);
	int y = static_cast<int>(m_pos.y - kHeight);
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();
	
	// 実際の当たり判定は少し小さくする
	return static_cast<float>(x + 15);
}

float Player::GetColRight() const
{
	// 足元中央を左上座標に変換する
	int x = static_cast<int>(m_pos.x - kWidth * 0.5f);
	int y = static_cast<int>(m_pos.y - kHeight);
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	// 実際の当たり判定は少し小さくする
	return static_cast<float>(x + kWidth - 15);
}

float Player::GetColTop() const
{
	// 足元中央を左上座標に変換する
	int x = static_cast<int>(m_pos.x - kWidth * 0.5f);
	int y = static_cast<int>(m_pos.y - kHeight);
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	// 実際の当たり判定は少し小さくする
	return static_cast<float>(y + 15);
}

float Player::GetColBottom() const
{
	// 足元中央を左上座標に変換する
	int x = static_cast<int>(m_pos.x - kWidth * 0.5f);
	int y = static_cast<int>(m_pos.y - kHeight);
	// スクロール量を反映させる
	x -= m_pCamera->GetScrollX();
	y -= m_pCamera->GetScrollY();

	// 実際の当たり判定は少し小さくする
	return static_cast<float>(y + kHeight - 15);
}

void Player::SetStartPos(float posX, float posY)
{
	m_pos.x = posX;
	m_pos.y = posY;
}

void Player::CompleteRecoveryLives()
{
	// 残機を全快する
	m_remainingLives = kMaxRemainingLives;
}

void Player::CompleteRecoveryHp()
{
	// HPを全快する
	m_hp = kMaxHp;
}

void Player::RecoveryHp2()
{
	// HPを回復する
	if (m_hp <= 3)
	{
		m_hp += 3;
	}
	else if (m_hp = 4)
	{
		m_hp += 2;
	}
	else if (m_hp = 5)
	{
		m_hp += 1;
	}
	else
	{
	}
	
}

void Player::OnDamage()
{
	m_isDamage = false;
	// ダメージ中は再度食らわない
	if (m_damageFrame > 0)
	{
		m_isDamage = true;
		return;
	}
	// SEを鳴らす
	PlaySoundMem(m_damageSeHandle, DX_PLAYTYPE_BACK, true);
	// HPを減らす
	m_hp--;
	// 演出フレーム数の設定
	m_damageFrame = kDamegeFrame;
}

void Player::OnMiss()
{
	m_isMiss = true;
	// ダメージ時の演出フレームを初期化する
	m_damageFrame = 0;
}

void Player::OnDance()
{
	m_condition = kDance;
	m_dir = kDirLeft;
	m_isDance;
	m_eventFrame++;
	

	if (m_eventFrame >= 0 && m_eventFrame < 30)
	{
		m_pos.x = 900;
		m_pos.y += 10;
		if (m_pos.y >= 800)
		{
			m_pos.y = 800;
		}
	}
	
	if (m_eventFrame >= 60)
	{
		if (!CheckSoundMem(m_clearBgmHandle))
		{
			PlaySoundMem(m_clearBgmHandle, DX_PLAYTYPE_BACK, true);
		}
	}
	
	if (m_eventFrame >= 60 && m_eventFrame < 120)
	{
		m_choreography = kLateralMove;
		m_pos.x -= 10;
	}
	if (m_eventFrame >= 120 && m_eventFrame < 170)
	{
		m_dir = kDirRight;
		m_pos.x += 5;
	}
	if (m_eventFrame >= 170 && m_eventFrame < 208)
	{
		m_choreography = kRotate;
		m_danceRotate += 0.1f;
		m_pos.x += 3;
		m_pos.y -= 5;
	}
	if (m_eventFrame >= 208 && m_eventFrame < 254)
	{
		m_danceRotate -= 0.1f;
		m_pos.x -= 3;
	}
	if (m_eventFrame >= 208 && m_eventFrame < 254)
	{
		m_danceSinCount += kDanceSinSpeed1;

		m_danceRotate += 0.1f;
		m_pos.x += 15;
		m_danceAnimationPosY = sinf(m_danceSinCount) * kDanceAnimSwing1;
	}
	if (m_eventFrame >= 254 && m_eventFrame < 300)
	{
		m_danceSinCount = 0;
		m_danceAnimationPosY = 0;
		m_danceRotate -= 0.2f;
		m_pos.x -= 5;
	}
	if (m_eventFrame >= 300 && m_eventFrame < 338)
	{
		m_danceSinCount += kDanceSinSpeed2;
		m_danceRotate = 0;
		m_choreography = kLateralMove;
		m_dir = kDirLeft;
		m_pos.x -= 8;
		m_danceAnimationPosY = sinf(m_danceSinCount) * kDanceAnimSwing2;
	}
	if (m_eventFrame >= 338 && m_eventFrame < 390)
	{
		m_danceSinCount = 0;
		m_danceAnimationPosY = 0;
		m_choreography = kRotate;
		m_pos.y -= 8;
	}
	if (m_eventFrame >= 415 && m_eventFrame < 450)
	{
		m_danceRotate += 0.5;
		m_pos.y += 10;
	}
	if (m_eventFrame >= 460 && m_eventFrame < 520)
	{
		m_danceRotate -= 0.2f;
		m_pos.x += 8;
	}
	if (m_eventFrame >= 520 && m_eventFrame < 575)
	{
		m_danceSinCount += kDanceSinSpeed2;
		m_pos.x -= 10;
		m_danceAnimationPosY = sinf(m_danceSinCount) * kDanceAnimSwing2;
	}
	if (m_eventFrame >= 575 && m_eventFrame < 590)
	{
		m_danceSinCount = 0;
		m_danceAnimationPosY = 0;
		m_danceRotate += 0.5;
		m_pos.y -= 10;
	}
	if (m_eventFrame >= 590 && m_eventFrame < 660)
	{
		m_danceRotate = 0;
		m_choreography = kLateralMove;
		m_dir = kDirRight;
		m_pos.x += 7;
	}
	if (m_eventFrame >= 660 && m_eventFrame < 700)
	{
		m_dir = kDirLeft;
		m_pos.x -= 10;
	}
	if (m_eventFrame >= 700 && m_eventFrame < 720)
	{
		m_choreography = kRotate;
		m_rotatePattern = 1;
		m_pos.y -= 3;
	}
	if (m_eventFrame >= 820)
	{
		StopSoundMem(m_clearBgmHandle);

		m_isFlowerMove = true;
	}
}