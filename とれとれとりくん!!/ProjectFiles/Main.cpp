#include "DxLib.h"
#include "Util/Game.h"
#include "Util/Input.h"
#include "Util/HandleManager.h"
#include "Scene/SceneManager.h"

#include <memory>

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 一部の関数はDxLib_Init()の前に実行する必要がある
	ChangeWindowMode(true);

	// 画面のサイズ変更
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, 32);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK);

	// 3D描画の奥行情報を使用する
	SetUseZBuffer3D(true);

	// ********** フォントのロード **********
	LPCSTR font_path = "Data/font/GN-KillGothic-U-KanaNA.ttf"; // 読み込むフォントファイルのパス
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// フォント読込エラー処理
		MessageBox(NULL, _T("フォント読込失敗"), _T(""), MB_OK);
	}


	// ゲームバーのタイトル
	SetWindowText(_T("とれとれとりくん！！"));

	// ハンドル管理
	HandleManager handle;

	// シーン管理
	shared_ptr<SceneManager> pScene = make_shared<SceneManager>();
	pScene->Init(handle);

	// 入力
	Input input;

	// ゲームループ
	while (ProcessMessage() != -1)
	{
		// このフレームの開始時刻を覚えておく
		LONGLONG start = GetNowHiPerformanceCount();

		// 描画を行う前に画面をクリアする
		ClearDrawScreen();

		// 入力の更新
		input.Update();

		// ゲームの処理
		pScene->Update(input,handle);
		pScene->Draw(handle);

		// 画面が切り替わるのを待つ
		ScreenFlip();

		// escキーでゲーム終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// FPS60に固定する
		while (GetNowHiPerformanceCount() - start < 16667)
		{
			// 16.66ミリ秒(16667マイクロ秒)経過するまで待つ
		}
	}

	pScene->End();

	// ********** フォントのアンロード **********
	if (RemoveFontResourceEx(font_path, FR_PRIVATE, NULL)) {
	}
	else {
		MessageBox(NULL, _T("remove failure"), _T(""), MB_OK);
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}

