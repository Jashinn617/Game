#include "DxLib.h"
#include "HandleManager.h"

#include <cassert>

HandleManager::HandleManager()
{
	LoadModel();
	LoadImg();
	LoadSound();
	ChangeSoundVolume();
	LoadFont();
}

HandleManager::~HandleManager()
{
	DeleteModel();
	DeleteImg();
	DeleteSound();
	DeleteFont();
}

int HandleManager::GetModel(const char* command) const
{
	auto it = m_modelHandleTable.find(command);

	return m_modelHandleTable.at(command);
}

int HandleManager::GetImg(const char* command) const
{
	auto it = m_imgHandleTable.find(command);

	return m_imgHandleTable.at(command);
}

int HandleManager::GetSound(const char* command) const
{
	auto it = m_soundHandleTable.find(command);

	return m_soundHandleTable.at(command);
}

int HandleManager::GetFont(const char* command) const
{
	auto it = m_fontHandleTable.find(command);

	return m_fontHandleTable.at(command);
}

void HandleManager::LoadModel()
{
	m_modelHandleTable["player"] = MV1LoadModel("Data/Model/Player/chicken.mv1");
	m_modelHandleTable["ground"] = MV1LoadModel("Data/Model/Ground.mv1");
	m_modelHandleTable["item"] = MV1LoadModel("Data/Model/Heart.mv1");
}

void HandleManager::LoadImg()
{
	m_imgHandleTable["startScreen"] = LoadGraph("Data/Img/Screen/StartScreen.png");
	m_imgHandleTable["fastClearScreen"] = LoadGraph("Data/Img/Screen/FastClearscreen.png");
	m_imgHandleTable["normalClearScreen"] = LoadGraph("Data/Img/Screen/NormalClearScreen.png");
	m_imgHandleTable["slowClearScreen"] = LoadGraph("Data/Img/Screen/SlowClearScreen.png");
	m_imgHandleTable["backSky"] = LoadGraph("Data/Img/Back/Sky.png");
	m_imgHandleTable["startLogo"] = LoadGraph("Data/Img/Logo/StartLogo.png");
	m_imgHandleTable["clearLogo"] = LoadGraph("Data/Img/Logo/ClearLogo.png");
	m_imgHandleTable["countdown1"] = LoadGraph("Data/Img/Logo/Countdown1.png");
	m_imgHandleTable["countdown2"] = LoadGraph("Data/Img/Logo/Countdown2.png");
	m_imgHandleTable["countdown3"] = LoadGraph("Data/Img/Logo/Countdown3.png");
	m_imgHandleTable["go"] = LoadGraph("Data/Img/Logo/Go.png");
	m_imgHandleTable["startLogo"] = LoadGraph("Data/Img/Logo/StartLogo.png");
	m_imgHandleTable["clearLogo"] = LoadGraph("Data/Img/Logo/ClearLogo.png");
}

void HandleManager::LoadSound()
{
	m_soundHandleTable["jumpSE"] = LoadSoundMem("Data/Sound/SE/JumpSE.mp3");
	m_soundHandleTable["fastClearSE"] = LoadSoundMem("Data/Sound/SE/FastClearSE.mp3");
	m_soundHandleTable["normalClearSE"] = LoadSoundMem("Data/Sound/SE/NormalClearSE.mp3");
	m_soundHandleTable["slowClearSE"] = LoadSoundMem("Data/Sound/SE/SlowClearSE.mp3");
	m_soundHandleTable["itemChatchSE"] = LoadSoundMem("Data/Sound/SE/ItemChatchSE.mp3");
	m_soundHandleTable["selectSE"] = LoadSoundMem("Data/Sound/SE/SelectSE.mp3");
	m_soundHandleTable["countdownSE"] = LoadSoundMem("Data/Sound/SE/CountdownSE.mp3");
	m_soundHandleTable["startBGM"] = LoadSoundMem("Data/Sound/BGM/StartBGM.mp3");
	m_soundHandleTable["mainBGM"] = LoadSoundMem("Data/Sound/BGM/MainBGM.mp3");
	m_soundHandleTable["clearBGM"] = LoadSoundMem("Data/Sound/BGM/ClearBGM.mp3");
}

void HandleManager::ChangeSoundVolume()
{
	ChangeVolumeSoundMem(kSeVolume, m_soundHandleTable["jumpSE"]);
	ChangeVolumeSoundMem(kSeVolume, m_soundHandleTable["fastClearSE"]);
	ChangeVolumeSoundMem(kSeVolume, m_soundHandleTable["normalClearSE"]);
	ChangeVolumeSoundMem(kSeVolume, m_soundHandleTable["slowClearSE"]);
	ChangeVolumeSoundMem(kItemChatchSeVolume, m_soundHandleTable["itemChatchSE"]);
	ChangeVolumeSoundMem(kSeVolume, m_soundHandleTable["selectSE"]);
	ChangeVolumeSoundMem(kSeVolume, m_soundHandleTable["countdownSE"]);
	ChangeVolumeSoundMem(kBgmVolume, m_soundHandleTable["startBGM"]);
	ChangeVolumeSoundMem(kBgmVolume, m_soundHandleTable["mainBGM"]);
	ChangeVolumeSoundMem(kBgmVolume, m_soundHandleTable["clearBGM"]);
}

void HandleManager::LoadFont()
{
	m_fontHandleTable["timeFont"] = CreateFontToHandle("GN-ƒLƒ‹ƒSU‚©‚ÈNA", 100, -1);
	m_fontHandleTable["itemNumFont"] = CreateFontToHandle("GN-ƒLƒ‹ƒSU‚©‚ÈNA", 80, -1);
	m_fontHandleTable["clearTimeFont"] = CreateFontToHandle("GN-ƒLƒ‹ƒSU‚©‚ÈNA", 70, -1);
}

void HandleManager::DeleteModel()
{
	MV1DeleteModel(m_modelHandleTable["player"]);
	MV1DeleteModel(m_modelHandleTable["ground"]);
	MV1DeleteModel(m_modelHandleTable["item"]);
}

void HandleManager::DeleteImg()
{
	DeleteGraph(m_imgHandleTable["startScreen"]);
	DeleteGraph(m_imgHandleTable["fastClearScreen"]);
	DeleteGraph(m_imgHandleTable["normalClearScreen"]);
	DeleteGraph(m_imgHandleTable["slowClearScreen"]);
	DeleteGraph(m_imgHandleTable["backSky"]);
	DeleteGraph(m_imgHandleTable["startLogo"]);
	DeleteGraph(m_imgHandleTable["clearLogo"]);
	DeleteGraph(m_imgHandleTable["countdown1"]);
	DeleteGraph(m_imgHandleTable["countdown2"]);
	DeleteGraph(m_imgHandleTable["countdown3"]);
	DeleteGraph(m_imgHandleTable["go"]);
	DeleteGraph(m_imgHandleTable["startLogo"]);
	DeleteGraph(m_imgHandleTable["clearLogo"]);
}

void HandleManager::DeleteSound()
{
	DeleteSoundMem(m_soundHandleTable["jumpSE"]);
	DeleteSoundMem(m_soundHandleTable["fastClearSE"]);
	DeleteSoundMem(m_soundHandleTable["normalClearSE"]);
	DeleteSoundMem(m_soundHandleTable["slowClearSE"]);
	DeleteSoundMem(m_soundHandleTable["selectSE"]);
	DeleteSoundMem(m_soundHandleTable["countdownSE"]);
	DeleteSoundMem(m_soundHandleTable["startBGM"]);
	DeleteSoundMem(m_soundHandleTable["mainBGM"]);
	DeleteSoundMem(m_soundHandleTable["clearBGM"]);
}

void HandleManager::DeleteFont()
{
	DeleteFontToHandle(m_fontHandleTable["timeFont"]);
	DeleteFontToHandle(m_fontHandleTable["itemNumFont"]);
	DeleteFontToHandle(m_fontHandleTable["clearTimeFont"]);
}
