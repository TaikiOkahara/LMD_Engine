#include "director.h"
#include "renderer.h"
#include "input.h"
#include "directionalLight.h"
#include "Imgui11.h"



void CDirectionalLight::Init()
{
	m_Transform.rotation = D3DXVECTOR3(0.5f, 0.0f, -0.5f);
	m_Transform.position = D3DXVECTOR3(-2.5, 1, 0);
	m_Transform.scale = D3DXVECTOR3(10, 10, 10);


	m_DLightCol = D3DXVECTOR4(0.4f, 0.6f, 1, 1);

	////シェーダー作成
	//RENDERER::CreatePixelShader(&m_PixelShader, "cursorPS.cso");

		//シェーダー作成
	RENDERER::CreateVertexShader(&m_pVertexShader, nullptr, nullptr, 0, "deferredVS.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "directionalLightPS.cso");

}

void CDirectionalLight::Uninit()
{
	/*m_VertexShader->Release();
	m_PixelShader->Release();*/



	m_pVertexShader->Release();
	m_pPixelShader->Release();
}

void CDirectionalLight::Update()
{
	if (CInput::KeyPress(DIK_J))
	{
		m_Transform.rotation.z -= 0.1f;
	}
	else if (CInput::KeyPress(DIK_L))
	{
		m_Transform.rotation.z += 0.1f;
	}
	else if (CInput::KeyPress(DIK_I))
	{
		m_Transform.rotation.x -= 0.1f;
	}
	else if (CInput::KeyPress(DIK_K))
	{
		m_Transform.rotation.x += 0.1f;
	}
}

void CDirectionalLight::Draw()
{
	if (!m_Enable) return;

	//D3DXMATRIX LightView;
	//D3DXMatrixInverse(&LightView, NULL, &LightView);


	D3DXMATRIX lrot;

	D3DXMatrixRotationYawPitchRoll(&lrot, m_Transform.rotation.y, m_Transform.rotation.x, m_Transform.rotation.z);
	m_DLightDir.x = lrot._21;
	m_DLightDir.y = lrot._22;
	m_DLightDir.z = lrot._23;
	m_DLightDir.w = 0.0f;
	D3DXVec4Normalize(&m_DLightDir, &m_DLightDir);
	m_DLightDir.w = 1.0f;

	RENDERER::m_ConstantBufferList.GetStruct<DirectionalLightBuffer>()->Set(m_DLightDir, m_DLightPos, m_DLightCol);



	/*RENDERER::m_pDeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_PixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(RENDERER::m_pCommonVertexLayout);*/


	////　マトリクス設定
	//D3DXMATRIX world, scale, rot, trans, view;

	//D3DXMatrixScaling(&scale, m_Transform.scale.x, m_Transform.scale.y, m_Transform.scale.z);
	//D3DXMatrixRotationYawPitchRoll(&rot, m_Transform.rotation.y, m_Transform.rotation.x, m_Transform.rotation.z);
	//D3DXMatrixTranslation(&trans, m_Transform.position.x, m_Transform.position.y, m_Transform.position.z);
	//world = scale * rot * trans;

	//RENDERER::BuffList.GetStruct<WorldBuffer>()->Set(world);

	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	//RENDERER::m_pDeviceContext->IASetInputLayout(RENDERER::m_pCommonVertexLayout);

}

void CDirectionalLight::Imgui()
{
	static bool show_main_window = true;

	if (CInput::KeyTrigger(DIK_F1))
		show_main_window = !show_main_window;


	if (show_main_window)
	{
		static bool is_open = true;

		ImGuiWindowFlags flag = 0;
		static ImVec4 clear_color = ImVec4(m_DLightCol.x, m_DLightCol.y, m_DLightCol.z, 1.00f);

		m_DLightCol.x = clear_color.x;
		m_DLightCol.y = clear_color.y;
		m_DLightCol.z = clear_color.z;




		ImGui::Begin("DirectionalLight", &is_open, flag);
		ImGui::InputFloat3("LightDirection", m_Transform.rotation, 1);
		ImGui::ColorEdit3("Color", (float*)&clear_color);

		ImGui::Checkbox("DirectionalLight", &m_Enable);

		ImGui::End();
	}
}
