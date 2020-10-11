#pragma once
#include "director.h"
#include "renderer.h"
#include "input.h"
#include "gameobject.h"

class CPointLight : public CGameObject
{
private:

	//std::vector<D3DXVECTOR4> m_PointList;
	POINTLIGHT m_PointLight;

	int m_PointCount;

	//構造体バッファとビュー
	ID3D11Buffer* m_pPointLightBuffer = nullptr;
	ID3D11ShaderResourceView* m_pPointLightBufferSRV = nullptr;
public:
	CPointLight() {}
	~CPointLight() {}


	//このInitはGameObjectクラスのInitの最後に入れる
	void Init()
	{
		{
			

			/*for (int i = 0; i < 3; i++)
			{
				D3DXVECTOR4 pos;
				pos = D3DXVECTOR4(0, 3, i * 3,1);
				m_PointList.push_back(&pos);

			}*/
			D3DXVECTOR4 pos = D3DXVECTOR4(-2.5, 2.0f, 5.0f, 3);

			
			//m_PointList.push_back(pos);//wは光の強さ
			m_PointLight.LightPosition[0] = pos;
		}


		//{//サンプルコンスタントバッファ

		//	RENDERER::CreateStructuredBuffer(sizeof(D3DXVECTOR4), m_PointList.size(), &m_PointList[0], &m_pPointLightBuffer);


		//	//シェーダリソースビューも作る必要がある
		//	//また、Formatは必ずDXGI_FORMAT_UNKNOWNにしないといけない
		//	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		//	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		//	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		//	srvDesc.Buffer.FirstElement = 0;
		//	srvDesc.Buffer.NumElements = m_PointList.size();

		//	RENDERER::m_pDevice->CreateShaderResourceView(m_pPointLightBuffer, &srvDesc, &m_pPointLightBufferSRV);
		//	RENDERER::m_pDeviceContext->VSSetShaderResources(0, 1, &m_pPointLightBufferSRV);

		//	//Rendererのバッファにセット
		//	RENDERER::SetPointLight(m_pPointLightBuffer, m_pPointLightBufferSRV);
		//}
	}
	void Uninit() 
	{
		SAFE_RELEASE(m_pPointLightBuffer);
		SAFE_RELEASE(m_pPointLightBufferSRV);
	}
	void Update()
	{
		//RENDERER::SetPointLight(m_pPointLightBuffer, m_pPointLightBufferSRV);
		//RENDERER::m_pDeviceContext->UpdateSubresource(m_pPointLightBuffer, 0, NULL, &m_PointList[0], 0, 0);
		RENDERER::SetPointLight(m_PointLight);

	}
	void Draw()
	{
		
	}
	void Imgui()
	{
		static bool show_light_window = true;

		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		if (Keyboard_IsTrigger(DIK_F1))
			show_light_window = !show_light_window;

		if (show_light_window)
		{
			ImGuiWindowFlags lw_flag = 0;
			static bool lw_is_open;

			ImGui::Begin("Light", &lw_is_open, lw_flag);

			for (int i = 0; i < 1; i++)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(m_PointLight.LightPosition[i].x, m_PointLight.LightPosition[i].y, m_PointLight.LightPosition[i].z);
				ImGui::InputFloat3("Position", pos, 1);
			}


			ImGui::End();
		}
	}
};
