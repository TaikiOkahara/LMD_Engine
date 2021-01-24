
/*�uLMD Engine�v===============================================
* 
*�E�Q�[���O���t�B�b�N�X�̊����Z�p���w�Ԃ��߂ɐ��삵���v���W�F�N�g
*
*	������F	(���\�z�F2020/03/06�`�A�G���W���쐬�F2020/09/01�`)
*�@	����ҁF	������N�@		(-"-)�m�@  ���b
*=============================================================*/
#define _CRTDBG_MAP_ALLOC

#include "base.h"

//���������[�N���o
#include <crtdbg.h>


INT WINAPI WinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE,_In_ LPSTR,_In_ INT)
{

	{
		Base::Init(hInstance);

		Base::Run();

		Base::Uninit();

	}

	//���������[�N���o
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();

	return 0;
 }