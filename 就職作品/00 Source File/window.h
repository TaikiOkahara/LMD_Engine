/*---------------------------------------
*　window.h
* 
* プロシージャ等のウィンドウ設定
*@author：Okahara Taiki
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