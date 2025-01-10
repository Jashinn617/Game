#pragma once

class Input;
class ClearScene
{
public:
	ClearScene();
	~ClearScene();

	// 初期化
	void Init();
	// 更新
	void Update(Input& input);
	// 描画
	void Draw();
	// 前景のびょうが
	void ForeDraw();
	// シーン終了時の処理
	void End();

	// シーンが終了したかどうか
	bool IsSceneEnd();

private:
	/*グラフィック*/
	// 背景
	int m_bgHandle;
	// 前景
	int m_fgHandle;
	// オブジェクト
	int m_objHandle0;
	int m_objHandle1;
	int m_objHandle2;
	int m_objHandle3;
	/*BGM*/
	int m_bgmHandle;
	// スクロール
	float m_scrollX;
	// サイズ
	int m_fgW;
	int m_fgH;
	// シーン終了フラグ
	bool m_isSceneEnd;

};

