#pragma once

class Input;
class StoryScene
{
public:
	StoryScene();
	~StoryScene();

	// 初期化
	void Init();
	// 更新
	void Update(Input& input);
	// 描画
	void Draw();
	// シーンの終了
	void End();
	// シーンが終了したかどうか
	bool IsSceneEnd();

private:
	/* グラフィック*/
	// ストーリー
	int m_storyHandle[8];
	// スキップ
	int m_skipHandle;
	// ボタン
	int m_buttonHandle;

	/*BGM*/
	int m_bgmHandle;

	// ストーリーが流れるフレーム数
	int m_storyFrame;
	// ストーリーのカウント
	int m_storyCount;
	// フェードイン、アウト
	int m_fadeAlpha;

	// シーンが終了したかどうか
	bool m_isSceneEnd;
};

