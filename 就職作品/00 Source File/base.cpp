/*---------------------------------------
*　base.cpp
* メインループを実現し、すべてのシーンを統括するクラス
*@author：Okahara Taiki
----------------------------------------*/
#include <string>
#include "base.h"
#include "renderer.h"
#include "Imgui11.h"
#include "input.h"

#include "scene.h"
#include "game.h"


static HINSTANCE	m_hInstance;
static WINDOW*		m_pWindow;
static HWND			m_hWnd;




CScene* Base::m_Scene = nullptr;

void Base::MainLoop()
{
	CInput::Update();
	m_Scene->Update();
	

	RENDERER::Clear();//　画面塗りつぶし
	
	m_Scene->Draw();
	
	RENDERER::Deferred();
	

	// IMGUI　Frame start
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	m_Scene->Imgui();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	// IMGUI　Frame end


	RENDERER::Present();
}

HRESULT Base::Init(HINSTANCE phInstance)
{
	m_hInstance = phInstance;

	CHAR dir[512];
	GetCurrentDirectory(sizeof(dir), dir);
	InitDirectory(dir);

	std::string samp = dir;
	

	//window
	m_pWindow = new WINDOW;
	if (!m_pWindow)
		return E_FAIL;


	MFAIL(m_pWindow->InitWindow(m_hInstance, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME), "ウィンドウ作成失敗");
	m_hWnd = m_pWindow->m_hWnd;
	


	RENDERER::Init(m_hWnd);

	SetScene<Game>();

	//　外部データ読み込み
	//LoadIni();


	IMGUI_Init(m_hWnd);

	//　キーボード初期化
	CInput::Init(m_hInstance, m_hWnd);


	
	//ウインドウの表示
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}

HRESULT Base::Uninit()
{

	CInput::Uninit();

	IMGUI_Uninit();

	m_Scene->UnInit();
	SAFE_DELETE(m_Scene);


	RENDERER::Uninit();

	DestroyWindow(m_pWindow->m_hWnd);
	SAFE_DELETE(m_pWindow);

	return S_OK;
}

HRESULT Base::Run()
{

	// メッセージループ
	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			MainLoop();
		}
	}

	return S_OK;
}
