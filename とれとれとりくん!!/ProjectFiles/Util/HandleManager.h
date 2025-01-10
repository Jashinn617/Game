#pragma once
#include <unordered_map>
#include <string>

using namespace std;

// <ハンドル名,ハンドル>
using HandleTable_t = unordered_map<string, int>;

/// <summary>
/// 各ハンドルの管理をするクラス
/// </summary>
class HandleManager
{
public:
	HandleManager();
	~HandleManager();

	/// <summary>
	/// モデルハンドルを取得する
	/// </summary>
	/// <param name="command">ハンドル名</param>
	/// <returns>ハンドル</returns>
	int GetModel(const char* command)const;
	/// <summary>
	/// 画像ハンドルを取得する
	/// </summary>
	/// <param name="command">ハンドル名</param>
	/// <returns>ハンドル</returns>
	int GetImg(const char* command)const;
	/// <summary>
	/// サウンドハンドルを取得する
	/// </summary>
	/// <param name="command">ハンドル名</param>
	/// <returns>ハンドル</returns>
	int GetSound(const char* command)const;
	/// <summary>
	/// フォントのハンドルを取得する
	/// </summary>
	/// <param name="command">ハンドル名</param>
	/// <returns>ハンドル</returns>
	int GetFont(const char* command)const;

private:	// 関数
	/// <summary>
	/// モデルのロード
	/// </summary>
	void LoadModel();
	/// <summary>
	/// 画像のロード
	/// </summary>
	void LoadImg();
	/// <summary>
	/// サウンドのロード
	/// </summary>
	void LoadSound();
	/// <summary>
	/// サウンドの初期音量の設定
	/// </summary>
	void ChangeSoundVolume();
	/// <summary>
	/// フォントのロード
	/// </summary>
	void LoadFont();

	/// <summary>
	/// モデルのデリート
	/// </summary>
	void DeleteModel();
	/// <summary>
	/// 画像のデリート
	/// </summary>
	void DeleteImg();
	/// <summary>
	/// サウンドのデリート
	/// </summary>
	void DeleteSound();
	/// <summary>
	///  フォントのデリート
	/// </summary>
	void DeleteFont();

private:	// 変数
	/*ハンドル名とハンドルをペアにしたテーブル*/
	HandleTable_t m_modelHandleTable;	// 3Dモデルのハンドル
	HandleTable_t m_imgHandleTable;		// 画像のハンドル
	HandleTable_t m_soundHandleTable;	// サウンドのハンドル
	HandleTable_t m_fontHandleTable;	// フォントのハンドル

private:	// 定数
	static constexpr int kBgmVolume = 130;			// BGMの音量
	static constexpr int kSeVolume = 255;			// SEの音量
	static constexpr int kItemChatchSeVolume = 200;	// アイテムをとるときのSEの音量
};

