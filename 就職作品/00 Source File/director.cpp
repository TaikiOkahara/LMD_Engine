/*「BASE.cpp」=============================================
　・外部ファイルを読み込んでデバック初期化をする。
  ・ディレクトリパスを通す関数

　　製作者：岡原大起　	(-"-)
==========================================================*/
#include <iostream>
#include <fstream>
#include <string>
#include "director.h"
static  std::string g_szRootPath;


static INI g_Ini;



//
//
//
INI* GetIni()
{
	return &g_Ini;
}
//
//
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

//
//
//　初期化
void InitDirectory(const std::string root)
{
	g_szRootPath = root;
}

//
//
//　ルートセット
void SetRootDirectory()
{
	SetCurrentDirectory(g_szRootPath.c_str());
}
//
//
//　Soundパス
void SetSoundDirectory()
{
	std::string tmp;
	tmp = g_szRootPath;
	tmp += "\\04 Sound File";
	SetCurrentDirectory(tmp.c_str());
}
//
//
//　Dataパス
void SetDataDirectory()
{
	std::string tmp;
	tmp = g_szRootPath;
	tmp += "\\01 Data File";
	SetCurrentDirectory(tmp.c_str());
}
//
//
//　Visualパス
void SetVisualDirectory()
{
	std::string tmp;
	tmp = g_szRootPath;
	tmp += "\\02 Visual File";
	SetCurrentDirectory(tmp.c_str());
}
//
//
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
//
//
//　Shaderパス
void SetShaderDirectory()
{
	std::string tmp;
	tmp = g_szRootPath;
	tmp += "\\03 Shader File";
	SetCurrentDirectory(tmp.c_str());
}
//
//
//
//std::string FindFile(const std::string path_name, const std::string file_name, const std::string file_format)
//{
//	SetRootDirectory();
//
//	HANDLE hFind;
//	WIN32_FIND_DATA fd;
//
//
//	std::string name;
//	name = path_name;
//	name += "\\";
//	name += file_name;
//	name += "\\*";
//	name += file_format;
//
//
//	hFind = FindFirstFile(name.c_str(), &fd);
//
//	// 検索失敗? 
//	if (hFind == INVALID_HANDLE_VALUE) {
//		MSG("Fbx読み込み失敗！(多分パスをミスってるかも)");
//		return "Error!";
//	}
//
//	// 検索終了
//	FindClose(hFind);
//
//	std::string fbx_name = fd.cFileName;
//	int char_num = 0;
//	for (int i = 0; fbx_name[i] != NULL; i++)
//	{
//		char_num++;
//	}
//	std::string return_name;
//
//	//strncpy(return_name, fd.cFileName, char_num);
//	return_name = fbx_name.erase(char_num);
//
//	return return_name;
//}