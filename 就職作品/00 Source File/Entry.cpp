
/*「LMD Engine」===============================================
* 
*・ゲームグラフィックスの既存技術を学ぶために制作したプロジェクト
*
*	制作日：	(環境構築：2020/03/06～、エンジン作成：2020/09/01～)
*　	製作者：	岡原大起　		(-"-)ノ　  ムッ
*=============================================================*/
#define _CRTDBG_MAP_ALLOC

#include "base.h"

//メモリリーク検出
#include <crtdbg.h>


INT WINAPI WinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE,_In_ LPSTR,_In_ INT)
{

	{
		Base::Init(hInstance);

		Base::Run();

		Base::Uninit();

	}

	//メモリリーク検出
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();

	return 0;
 }