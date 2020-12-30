
/*�u�Q�[���v���W�F�N�g�v======================================
�@�ED3DX11�ŏ��ɍ쐬�����v���W�F�N�g
		�ڍׂ�Manual�ɋL�ڂ��Ă��܂��B
		
		
 �@ ������F2020/03/06�`
�@�@����ҁF������N�@		(-"-)�m�@  ���b
=============================================================*/
#define _CRTDBG_MAP_ALLOC

#include <windows.h>
#include "base.h"

//���������[�N���o
#include <stdlib.h>
#include <crtdbg.h>

//// debug_new.cpp
//// compile by using: cl /EHsc /W4 /D_DEBUG /MDd debug_new.cpp
//#define _CRTDBG_MAP_ALLOC
//#include <cstdlib>
//#include <crtdbg.h>
//
//#ifdef _DEBUG
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
//// allocations to be of _CLIENT_BLOCK type
//#else
//#define DBG_NEW new
//#endif

INT WINAPI WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR,INT)
{

	{
		Base::Init(hInstance);

		Base::Run();

		Base::Uninit();
	}

	////���������[�N���o
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();

	return 0;
}