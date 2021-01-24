/*---------------------------------------
*　director.cpp
* インクルードを一括で行う
* ディレクトリパスを変更する関数群
*@author：Okahara Taiki
----------------------------------------*/
#include <iostream>
#include <fstream>
#include "director.h"

static  std::string g_szRootPath;
static INI g_Ini;



INI* GetIni()
{
	return &g_Ini;
}


//　設定パラメーター読み込み
HRESULT LoadIni()
{
	SetDataDirectory();	//ディレクトリ指定
	FILE* fp = fopen("game.ini", "rt");

	char line[MAX_PATH];

	fgets(line, MAX_PATH, fp);
	g_Ini.MeshRender = atoi(line);//1=メッシュを表示 0=非表示

	fgets(line, MAX_PATH, fp);
	g_Ini.SoundPlay = atoi(line);//1=サウンド再生 0=再生しない

	fgets(line, MAX_PATH, fp);
	g_Ini.CameraZ = (FLOAT)atof(line);//カメラのZ位置

	fclose(fp);

	return S_OK;
}


//　初期化
void InitDirectory(const std::string root)
{
	g_szRootPath = root;
}


//　ルートセット
void SetRootDirectory()
{
	SetCurrentDirectory(g_szRootPath.c_str());
}

void SetSoundDirectory()
{
	std::string tmp;
	tmp = g_szRootPath;
	tmp += "\\04 Sound File";
	SetCurrentDirectory(tmp.c_str());
}


void SetDataDirectory()
{
	std::string tmp;
	tmp = g_szRootPath;
	tmp += "\\01 Data File";
	SetCurrentDirectory(tmp.c_str());
}

void SetVisualDirectory()
{
	std::string tmp;
	tmp = g_szRootPath;
	tmp += "\\02 Visual File";
	SetCurrentDirectory(tmp.c_str());
}


//　Visualパス2階層
void SetVisualDirectory(std::string file_name)
{
	std::string tmp;
	tmp = g_szRootPath;
	tmp += "\\02 Visual File";
	tmp += "\\";
	tmp += file_name;
	SetCurrentDirectory(tmp.c_str());
}

void SetShaderDirectory()
{
	std::string tmp;
	tmp = g_szRootPath;
	tmp += "\\03 Shader File";
	SetCurrentDirectory(tmp.c_str());
}
