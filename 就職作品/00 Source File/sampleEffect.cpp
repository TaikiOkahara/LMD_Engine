#include "director.h"
#include "renderer.h"
#include "sampleEffect.h"
#include "Imgui11.h"
#include "input.h"



void CEffectSample::Init()
{
	//シェーダー作成
	RENDERER::CreateVertexShader(&m_pVertexShader, nullptr, nullptr, 0, "deferredVS.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "fogPS.cso");

	RENDERER::CreateComputeShader("", &m_pComputeShader);
	assert(m_pComputeShader);

	/*D3DX11CreateShaderResourceViewFromFile(RENDERER::m_pDevice, "../02 Visual File//textures//fog.dds", NULL, NULL, &m_FogTexture, NULL);
	assert(m_FogTexture);*/


	{//スタックバッファ

		
		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(td));
		td.Width = WINDOW_WIDTH;
		td.Height = WINDOW_HEIGHT;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.CPUAccessFlags = 0;
		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		RENDERER::GetDevice()->CreateTexture2D(&td, NULL, &m_Texture);

		//D3D11_BUFFER_DESC desc = {};
		//desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		//desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		//desc.ByteWidth = sizeof(UINT) * m_MeshMax;//インデックスリスト
		//desc.StructureByteStride = sizeof(UINT);
		//desc.Usage = D3D11_USAGE_DEFAULT;
		//auto hr = RENDERER::GetDevice()->CreateBuffer(&desc, nullptr, &m_pIndexBuffer);
		//assert(SUCCEEDED(hr));

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		ZeroMemory(&uavDesc, sizeof(uavDesc));
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Format = td.Format;
		//uavDesc.Texture2D.MipSlice
		//uavDesc.Buffer.FirstElement = 0;
		//uavDesc.Buffer.NumElements = m_MeshMax;
		//uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;
		RENDERER::GetDevice()->CreateUnorderedAccessView(m_Texture, &uavDesc, &m_TextureUAV);


	}
}

void CEffectSample::Uninit()
{
	m_TextureUAV->Release();

	m_pComputeShader->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();
}

void CEffectSample::Update()
{

}

void CEffectSample::Draw()
{




	/*RENDERER::GetDeviceContext()->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::GetDeviceContext()->PSSetShader(m_pPixelShader, NULL, 0);*/


	//フォグテクスチャのセット
	//RENDERER::GetDeviceContext()->PSSetShaderResources(7, 1, &m_TextureSRV);

	RENDERER::SetBlendState_Lighting();


	RENDERER::PostProcessDraw();
}

void CEffectSample::Imgui()
{
	//static bool show_fog_window = true;

	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	//if (CInput::KeyTrigger(DIK_F1))
	//	show_fog_window = !show_fog_window;

	//if (show_fog_window)
	//{
	//	ImGuiWindowFlags lw_flag = 0;
	//	static bool lw_is_open;

	//	ImGui::Begin("Effect", &lw_is_open, lw_flag);

	//	if (ImGui::TreeNode("Fog"))
	//	{

	//		ImGui::Checkbox("EnableCollision", &m_Enable);

	//		//static D3DXVECTOR2 fogoffset = m_Effect.fogOffset[0];
	//		static ImVec4 clear_color = ImVec4(m_Fog.color.x, m_Fog.color.y, m_Fog.color.z, 1.00f);

	//		m_Fog.color.x = clear_color.x;
	//		m_Fog.color.y = clear_color.y;
	//		m_Fog.color.z = clear_color.z;


	//		ImGui::ColorEdit3("Color", (float*)&clear_color);

	//		ImGui::SliderFloat2("fogOffset0", m_Fog.fogOffset[0], 0.0f, 1.0f);
	//		ImGui::SliderFloat2("fogOffset1", m_Fog.fogOffset[1], 0.0f, 1.0f);


	//		ImGui::SliderFloat("texScale", &m_Fog.texScale, 0.0f, 1.0f);
	//		ImGui::SliderFloat("maxhHeight", &m_Fog.maxHeight, 0.0f, 2.0f);
	//		ImGui::SliderFloat("minhHeight", &m_Fog.minHeight, -2.0f, 1.0f);


	//		ImGui::TreePop();
	//	}

	//	ImGui::Separator();

	//	if (ImGui::TreeNode("MotionBlur"))
	//	{

	//		ImGui::TreePop();
	//	}

	//	ImGui::End();
	//}
}
