/*---------------------------------------
*　window.cpp
*
*@author：Okahara Taiki
----------------------------------------*/
#include "director.h"
#include "window.h"


WINDOW* g_pWindow=NULL;

const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "DX11Tempate";

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return g_pWindow->MsgProc(hWnd,uMsg,wParam,lParam);
}

HRESULT WINDOW::InitWindow(HINSTANCE hInstance,
		INT iX,INT iY,INT iWidth,INT iHeight,LPCSTR WindowName)
{
	g_pWindow=this;
	// ウィンドウの定義
	WNDCLASSEX wc;
	ZeroMemory(&wc,sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= WndProc;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName	= WindowName;
	wc.hIconSm			= LoadIcon(NULL,IDI_APPLICATION);
	RegisterClassEx(&wc);

	
	DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);

	
	RECT window_rect = { 0, 0, iWidth, iHeight };

	
	AdjustWindowRect(&window_rect, window_style, FALSE);

	
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	// デスクトップの真ん中にウィンドウが生成されるように座標を計算
	// ※ただし万が一、デスクトップよりウィンドウが大きい場合は左上に表示
	int window_x = std::max((desktop_width - window_width) / 2, 0);
	int window_y = std::max((desktop_height - window_height) / 2, 0);


	
	m_hWnd=CreateWindowEx(
		0,
		WindowName,
		WindowName,
		window_style,
		window_x,
		window_y,
		window_width,
		window_height,
		0,
		0,
		hInstance,
		0
		);


	if(!m_hWnd)
	{
		return E_FAIL;
	}

	
	return S_OK;
}
//
//
//
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
 LRESULT WINDOW::MsgProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	 if (ImGui_ImplWin32_WndProcHandler(hWnd, iMsg, wParam, lParam))
		 return true;

	switch(iMsg)
	{
		case WM_KEYDOWN:
		
			switch ((char)wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc (hWnd, iMsg, wParam, lParam);
}