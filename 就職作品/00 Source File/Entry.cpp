
/*「ゲームプロジェクト」======================================
　・D3DX11最初に作成したプロジェクト
		詳細はManualに記載しています。
		
		
 　 制作日：2020/03/06～
　　製作者：岡原大起　		(-"-)ノ　  ムッ
=============================================================*/
#include <windows.h>
#include "base.h"


INT WINAPI WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR,INT)
{
	Base::Init(hInstance);

	Base::Run();

	Base::Uninit();
	return 0;
}