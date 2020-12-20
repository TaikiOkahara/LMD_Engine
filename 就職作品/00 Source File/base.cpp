/*「DIRECTOR.cpp」=============================================
　・全てのシーンを統括するクラス
　　製作者：岡原大起　	(-"-)
=============================================================*/
#include <string>
#include "base.h"
#include "renderer.h"
#include "Imgui11.h"
#include "input.h"

#include "scene.h"
#include "game.h"

#include "postProcess.h"

static HINSTANCE	m_hInstance;
static WINDOW*		m_pWindow;
static HWND			m_hWnd;




CScene* Base::m_Scene = nullptr;
CPostProcess Base::m_PostProcess;
//
//　メインループ
//
void Base::MainLoop()
{
	CInput::Update();
	m_Scene->Update();
	m_PostProcess.Update();
	

	RENDERER::Clear();//　画面塗りつぶし
	
	
	m_Scene->Draw();
	
	/*if (RENDERER::togglePoint){
		RENDERER::PointLighting();
		m_Scene->DrawLighting();
	}*/
	
	RENDERER::Deferred();
	
	/*if (RENDERER::toggleDirectional){
		RENDERER::DirectionlLighting();
	}*/

	
	m_PostProcess.Draw();


	// IMGUI　Frame start
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	m_Scene->Imgui();
	m_PostProcess.Imgui();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	// IMGUI　Frame end



	RENDERER::Present();
}
//
//
//　FPS表示関数
void Base::FixFPS60()
{
	static INT Frames=0,FPS=0;
	static LARGE_INTEGER Frq={0},PreviousTime={0},CurrentTime={0};
	DOUBLE Time=0;
	char sz[11]={0};

	while(Time<16.6666)//1100ms / 60frame=16.6666 
	{
		QueryPerformanceFrequency(&Frq);
		
		QueryPerformanceCounter(&CurrentTime);
		Time=(DOUBLE)CurrentTime.QuadPart-PreviousTime.QuadPart;
		Time *=(DOUBLE)1100.0 / (DOUBLE)Frq.QuadPart;		
	}
	PreviousTime=CurrentTime;
}
//
//
//　初期化
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
	{
		return E_FAIL;
	}
	MFAIL(m_pWindow->InitWindow(m_hInstance, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME), "ウィンドウ作成失敗");
	m_hWnd = m_pWindow->m_hWnd;
	

	

	RENDERER::Init(m_hWnd);

	

	SetScene<Game>();
	m_PostProcess.Init();

	//　外部データ読み込み
	LoadIni();


	IMGUI_Init(m_hWnd);

	//　キーボード初期化
	CInput::Init(m_hInstance, m_hWnd);


	//ウインドウの表示
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);


	return S_OK;
}
//
//
//

HRESULT Base::Uninit()
{

	m_Scene->UnInit();
	delete m_Scene;

	m_PostProcess.Uninit();


	IMGUI_Uninit();

	RENDERER::Uninit();

	CInput::Uninit();

	SAFE_DELETE(m_pWindow);

	return S_OK;
}
//
//
//
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
			//FixFPS60();
			MainLoop();
		}
	}

	return S_OK;
}
