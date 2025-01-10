#pragma once

class Input;
class HandleManager;

/// <summary>
/// プレイヤークラス
/// </summary>
class Player
{
public:
	Player();
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="handle">ハンドル</param>
	void Init(HandleManager& handle);
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力</param>
	/// <param name="handle">ハンドル</param>
	/// <param name="isOperate">操作ができるかどうか</param>
	void Update(Input& input, HandleManager& handle, bool isOperate);
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="handleManager">ハンドル</param>
	void Draw(HandleManager& handleManager)const;

	/// <summary>
	/// 位置の取得
	/// </summary>
	/// <returns>現在位置</returns>
	const VECTOR GetPos()const { return m_pos; }

	/// <summary>
	/// 半径の取得
	/// </summary>
	/// <returns>半径</returns>
	const float GetRadius()const { return kRadius; }

	/// <summary>
	/// カメラの角度を設定する
	/// </summary>
	/// <param name="angle">カメラのアングル</param>
	void SetCameraAngle(float angle) { m_cameraAngle = angle; }

private:	// 関数
	/// <summary>
	/// コントローラーでの移動処理
	/// </summary>
	/// <param name="input">入力</param>
	void Move(Input& input);

	/// <summary>
	/// ジャンプ処理
	/// </summary>
	/// <param name="input">入力</param>
	/// <param name="handle">ハンドル</param>
	void Jump(Input& input, HandleManager& handle);

	/// <summary>
	/// 重力と地面に関する処理
	/// </summary>
	void GravityAndGround();

	/// <summary>
	/// 壁の当たり判定に関する処理
	/// </summary>
	void Wall();

private:	// 変数
	VECTOR m_pos;			// 位置
	float m_dir;			// Y方向の向き
	float m_jumpPower;		// ジャンプ力
	bool m_isJump;			// ジャンプ中かどうか
	float m_cameraAngle;	// カメラ情報

private:	// 定数
	static constexpr float kMaxSpeed = 400.0f;							// 速度
	static constexpr float kScale = 0.002f;								// 拡大率
	static constexpr float kRadius = 0.5f;								// 半径
	static constexpr float kStartModelDir = (DX_PI_F / 180.0f) * 90;	// モデルの初期のY軸向き
	static constexpr float kGravity = 0.025f;							// 重力
	static constexpr float kJumpPower = 0.35f;							// ジャンプ力
	static constexpr float kGroundPosY = 0.4f;							// 地面のY位置
	static constexpr float kWallZ = 30;									// z軸の壁
	static constexpr float kWallX = 50;									// x軸の壁
	static constexpr float kAnalogRangeMax = 0.8f;						// アナログスティックの有効範囲の最大値
	static constexpr float kAnalogRangeMim = 0.1f;						// アナログスティックの有効範囲の最小値
	static constexpr float kAnalogInputMax = 1000.0f;					// アナログスティックの最大入力範囲
};