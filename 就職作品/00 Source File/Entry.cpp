
/*�u�Q�[���v���W�F�N�g�v======================================
�@�ED3DX11�ŏ��ɍ쐬�����v���W�F�N�g
		�ڍׂ�Manual�ɋL�ڂ��Ă��܂��B
		
		
 �@ ������F2020/03/06�`
�@�@����ҁF������N�@		(-"-)�m�@  ���b
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