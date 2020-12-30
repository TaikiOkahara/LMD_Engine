
/*「ゲームプロジェクト」======================================
　・D3DX11最初に作成したプロジェクト
		詳細はManualに記載しています。
		
		
 　 制作日：2020/03/06～
　　製作者：岡原大起　		(-"-)ノ　  ムッ
=============================================================*/
#define _CRTDBG_MAP_ALLOC

#include <windows.h>
#include "base.h"

//メモリリーク検出
#include <stdlib.h>
#include <crtdbg.h>


INT WINAPI WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR,INT)
{

	{
		Base::Init(hInstance);

		Base::Run();

		Base::Uninit();
	}

	////メモリリーク検出
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();

	return 0;
}