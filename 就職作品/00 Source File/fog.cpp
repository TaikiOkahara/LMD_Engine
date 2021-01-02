#include "director.h"
#include "renderer.h"
#include "fog.h"
#include "Imgui11.h"
#include "input.h"



void CFogEffect::Init()
{
	//シェーダー作成
	RENDERER::CreateVertexShader(&m_pVertexShader, nullptr, nullptr, 0, "deferredVS.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "fogPS.cso");


	D3DX11CreateShaderResourceViewFromFile(RENDERER::m_pDevice, "../02 Visual File//fog.dds", NULL, NULL, &m_FogTexture, NULL);
	assert(m_FogTexture);


	m_Fog.fogOffset[0] = D3DXVECTOR2(0, 0);
	m_Fog.fogOffset[1] = D3DXVECTOR2(0, 0);

	m_Fog.texScale = 0.1f;
	m_Fog.maxHeight = 1.0f;
	m_Fog.minHeight = 0.1f;
	m_Fog.dummy0 = 0;
	m_Fog.fogEnable = true;
	m_Fog.color = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
}

void CFogEffect::Uninit()
{
	m_FogTexture->Release();

	m_pVertexShader->Release();
	m_pPixelShader->Release();
}

void CFogEffect::Update()
{
	static float fogSpeed0 = 0.001f;
	static float fogSpeed1 = 0.0005f;

	m_Fog.fogOffset[0].x += fogSpeed0;
	m_Fog.fogOffset[0].y += fogSpeed0;

	m_Fog.fogOffset[1].x += fogSpeed1;

	if (m_Fog.fogOffset[0].x >= 1)
	{
		m_Fog.fogOffset[0].x = 0;
	}
	if (m_Fog.fogOffset[0].y >= 1)
	{
		m_Fog.fogOffset[0].y = 0;
	}

	if (m_Fog.fogOffset[1].x >= 1)
	{
		m_Fog.fogOffset[1].x = 0;
	}

	m_Fog.fogEnable = m_Enable;
}

void CFogEffect::Draw()
{


	RENDERER::m_ConstantBufferList.GetStruct<EffectBuffer>()->SetFog(m_Fog);

	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);


	//フォグテクスチャのセット
	RENDERER::m_pDeviceContext->PSSetShaderResources(7, 1, &m_FogTexture);

	RENDERER::SetBlendState_Lighting();


	RENDERER::PostProcessDraw();
}

void CFogEffect::Imgui()
{
	static bool show_fog_window = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (CInput::KeyTrigger(DIK_F1))
		show_fog_window = !show_fog_window;

	if (show_fog_window)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Effect", &lw_is_open, lw_flag);

		if (ImGui::TreeNode("Fog"))
		{

			ImGui::Checkbox("EnableCollision", &m_Enable);

			//static D3DXVECTOR2 fogoffset = m_Effect.fogOffset[0];
			static ImVec4 clear_color = ImVec4(m_Fog.color.x, m_Fog.color.y, m_Fog.color.z, 1.00f);

			m_Fog.color.x = clear_color.x;
			m_Fog.color.y = clear_color.y;
			m_Fog.color.z = clear_color.z;


			ImGui::ColorEdit3("Color", (float*)&clear_color);

			ImGui::SliderFloat2("fogOffset0", m_Fog.fogOffset[0], 0.0f, 1.0f);
			ImGui::SliderFloat2("fogOffset1", m_Fog.fogOffset[1], 0.0f, 1.0f);


			ImGui::SliderFloat("texScale", &m_Fog.texScale, 0.0f, 1.0f);
			ImGui::SliderFloat("maxhHeight", &m_Fog.maxHeight, 0.0f, 2.0f);
			ImGui::SliderFloat("minhHeight", &m_Fog.minHeight, -2.0f, 1.0f);


			ImGui::TreePop();
		}

		ImGui::Separator();

		if (ImGui::TreeNode("MotionBlur"))
		{

			ImGui::TreePop();
		}

		ImGui::End();
	}
}
