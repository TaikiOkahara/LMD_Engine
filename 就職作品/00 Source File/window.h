/*---------------------------------------
*�@window.h
* 
* �v���V�[�W�����̃E�B���h�E�ݒ�
*@author�FOkahara Taiki
----------------------------------------*/
#pragma once
#include "director.h"

class WINDOW
{
public:
	HWND m_hWnd;

	HRESULT InitWindow(HINSTANCE,INT,INT,INT,INT,LPCSTR);
	LRESULT MsgProc(HWND,UINT,WPARAM,LPARAM);
};