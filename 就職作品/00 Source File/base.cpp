/*�uDIRECTOR.cpp�v=============================================
�@�E�S�ẴV�[���𓝊�����N���X
�@�@����ҁF������N�@	(-"-)
=============================================================*/
#include <string>
#include "base.h"
#include "renderer.h"
#include "Imgui11.h"
#include "input.h"

#include "Scene.h"
#include "Game.h"

static HINSTANCE	m_hInstance;
static WINDOW*		m_pWindow;
static HWND			m_hWnd;

//static void MainLoop();
//static void FixFPS60();


//Scene g_Scene;
Scene* Base::m_Scene = nullptr;

//
//�@���C�����[�v
//
void Base::MainLoop()
{
	Input_Update();

	m_Scene->Update();
	
	
	//RENDERER::ShadowClear();
	//g_Scene.Draw();

	RENDERER::Clear();//�@��ʓh��Ԃ�
	
	
	m_Scene->Draw();
	/*RENDERER::OutLineClear();
	g_Scene.Draw();*/
	
	//Deferred
	RENDERER::Deferred();
	//PointLighting
	RENDERER::Lighting();
	m_Scene->DrawLighting();

	m_Scene->Imgui();


	RENDERER::Present();
}
//
//
//�@FPS�\���֐�
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
//�@������
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
	MFAIL(m_pWindow->InitWindow(m_hInstance, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME), "�E�B���h�E�쐬���s");
	m_hWnd = m_pWindow->m_hWnd;
	//direct3D11
	D3D_INIT di;
	di.hWnd = m_hWnd;


	

	RENDERER::Init(&di);

	
	//m_Scene = new Scene();
	//m_Scene->Init();

	SetScene<Game>();

	//�@�O���f�[�^�ǂݍ���
	LoadIni();


	//IMGUI_INIT();
	IMGUI_Init(m_hWnd);

	//�@�L�[�{�[�h������
	Input_Initialize(m_hInstance, m_hWnd);


	//�E�C���h�E�̕\��
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

	IMGUI_Uninit();

	RENDERER::Uninit();

	Input_Finalize();

	SAFE_DELETE(m_pWindow);

	return S_OK;
}
//
//
//
HRESULT Base::Run()
{

	// ���b�Z�[�W���[�v
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
