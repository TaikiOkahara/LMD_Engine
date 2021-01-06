/*「BASE.h」=============================================
　　製作者：岡原大起　	(-"-)
=============================================================*/
#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NOMINMAX
#include <windows.h>
#include <assert.h>


#pragma warning(push)
//警告非表示
#pragma warning(disable : 4005)

#include <d3d11.h>
#include <d3dx9.h>
#include <d3dx10.h>
#include <d3dCompiler.h>
#include <d3dx11.h>

#pragma warning(pop)



#include <vector>
#include <map>
#include <string>
#include <list>
#include <typeinfo>
#include <numeric>
#include <algorithm>



//必要なライブラリファイルのロード
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

#pragma warning(disable : 4305)
#pragma warning(disable : 4996)
#pragma warning(disable : 4018)
#pragma warning(disable : 4111)


//
//　マクロ
#define APP_NAME "LMD Engine ：　Okahara Taiki"//アプリ名
#define WINDOW_WIDTH 1920	//　サイズ横
#define WINDOW_HEIGHT 1080	//　サイズ縦
//
//動的確保の解放
#define SAFE_DELETE(p) { if(p) { delete (p); (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define MFAIL(code,string) if(FAILED( code ) ) { MessageBox(0,string,"error",MB_OK); return E_FAIL; }
#define MFALSE(code,string) if(!( code ) ) { MessageBox(0,string,"error",MB_OK); return E_FAIL; }
#define MSG(t) MessageBox(0,t,0,MB_OK);
//
//
//　外部保存データ構造体
struct INI
{
	bool	MeshRender;	//1=メッシュを表示 0=非表示
	bool	SoundPlay;	// 1 = サウンド再生 0 = 再生しない
	float	CameraZ;	//	カメラのZ位置

	//コンストラクタ
	INI(){
		ZeroMemory(this, sizeof(INI));//　ゼロクリア
	}
};



//
//プロトタイプ
void InitDirectory(const std::string root);
void SetRootDirectory();
void SetDataDirectory();
void SetVisualDirectory();
void SetVisualDirectory(std::string file_name);//さらに下の階層を参照
void SetShaderDirectory();
void SetSoundDirectory();

//std::string FindFile(const std::string path_name, const std::string file_name, const std::string file_format);

HRESULT LoadIni();
INI* GetIni();
