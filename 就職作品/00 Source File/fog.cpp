#include "director.h"
#include "renderer.h"
#include "fog.h"
#include "Imgui11.h"
#include "input.h"

//
//
//
void CFog::Init()
{
	
	//シェーダー作成
	RENDERER::CreateVertexShader(&m_pVertexShader, &RENDERER::m_pCommonVertexLayout, nullptr, 0, "DeferredVS.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "FogPixelShader.cso");


	D3DX11CreateShaderResourceViewFromFile(RENDERER::m_pDevice,"../02 Visual File//fog.jpg",NULL,NULL,&m_FogTexture,NULL);
	assert(m_FogTexture);


	m_Effect.fogOffset[0] = D3DXVECTOR2(0, 0);
	m_Effect.fogOffset[1] = D3DXVECTOR2(0, 0);

	m_Effect.texScale = 0.1f;
	m_Effect.maxHeight = 1.0f;
	m_Effect.minHeight = 0.1f;
	m_Effect.dummy0 = 0;
	m_Effect.dummy1 = 0;
	m_Effect.color = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
}

void CFog::Uninit()
{
	m_FogTexture->Release();
	
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
}

void CFog::Update()
{
	static float fogSpeed0 = 0.001f;
	static float fogSpeed1 = 0.0005f;

	m_Effect.fogOffset[0].x += fogSpeed0;
	m_Effect.fogOffset[0].y += fogSpeed0;

	m_Effect.fogOffset[1].x += fogSpeed1;

	if (m_Effect.fogOffset[0].x >= 1)
	{
		m_Effect.fogOffset[0].x = 0;
	}
	if (m_Effect.fogOffset[0].y >= 1)
	{
		m_Effect.fogOffset[0].y = 0;
	}
	
	if (m_Effect.fogOffset[1].x >= 1)
	{
		m_Effect.fogOffset[1].x = 0;
	}

}

void CFog::Draw()
{

	RENDERER::SetEffect(m_Effect);

	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	//RENDERER::m_pDeviceContext->IASetInputLayout(RENDERER::m_pCommonVertexLayout);



	//フォグテクスチャのセット
	RENDERER::m_pDeviceContext->PSSetShaderResources(3, 1, &m_FogTexture);

	if (!isEnableCollision) return;


	RENDERER::Effect();
}

void CFog::Imgui()
{
	static bool show_fog_window = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (CInput::KeyTrigger(DIK_F1))
		show_fog_window = !show_fog_window;

	if (show_fog_window)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Fog", &lw_is_open, lw_flag);

		ImGui::Checkbox("isEnableCollision", &isEnableCollision);

		//static D3DXVECTOR2 fogoffset = m_Effect.fogOffset[0];
		static ImVec4 clear_color = ImVec4(m_Effect.color.x, m_Effect.color.y, m_Effect.color.z, 1.00f);

		m_Effect.color.x = clear_color.x;
		m_Effect.color.y = clear_color.y;
		m_Effect.color.z = clear_color.z;


		ImGui::ColorEdit3("Color", (float*)&clear_color);


		ImGui::SliderFloat("foggoffset:x", &m_Effect.fogOffset[0].x, 0.0f, 1.0f);
		ImGui::SliderFloat("foggoffset:y", &m_Effect.fogOffset[0].y, 0.0f, 1.0f);
		ImGui::SliderFloat("foggoffset2:x", &m_Effect.fogOffset[1].x, 0.0f, 1.0f);
		ImGui::SliderFloat("foggoffset2:y", &m_Effect.fogOffset[1].y, 0.0f, 1.0f);

		ImGui::SliderFloat("texScale", &m_Effect.texScale, 0.0f, 1.0f);
		ImGui::SliderFloat("maxhHeight", &m_Effect.maxHeight, 0.0f, 2.0f);
		ImGui::SliderFloat("minhHeight", &m_Effect.minHeight, -2.0f, 1.0f);

		ImGui::End();
	}
}