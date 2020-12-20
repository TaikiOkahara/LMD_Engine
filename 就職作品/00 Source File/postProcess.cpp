#include "postProcess.h"
#include "fog.h"
#include "scenelight.h"

void CPostProcess::Init()
{
	AddPostProcess<CDirectionalLight>();
	AddPostProcess<CFogEffect>();
}

void CPostProcess::Uninit()
{
	for (CEffect* effect : m_Effect)
	{
		effect->Uninit();
		delete effect;
	}

	m_Effect.clear();
}

void CPostProcess::Update()
{
	for (CEffect* effect : m_Effect)
	{
		effect->Update();
	}
}

void CPostProcess::Draw()
{
	//
	for (CEffect* effect : m_Effect)
	{
		effect->Draw();
		RENDERER::PostProcessDraw();
	}
}

void CPostProcess::Imgui()
{
	for (CEffect* effect : m_Effect)
	{
		effect->Imgui();
	}
}
