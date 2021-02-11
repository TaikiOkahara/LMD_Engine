/*---------------------------------------
*@directionalLight.cpp
*
*@authorFOkahara Taiki
----------------------------------------*/
#include "base.h"
#include "director.h"
#include "renderer.h"
#include "input.h"
#include "directionalLight.h"
#include "Imgui11.h"
#include "camera.h"



void CDirectionalLight::Init()
{
	m_Transform.rotation = D3DXVECTOR3(15, 15, 0);
	m_Transform.position = D3DXVECTOR3(-2.5, 1, 0);
	m_Transform.scale = D3DXVECTOR3(10, 10, 10);

	m_Color = D3DXVECTOR4(1, 1, 1, 1);

	
	RENDERER::CreateVertexShader(&m_pVertexShader, nullptr, nullptr, 0, "deferredVS.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "directionalLightPS.cso");

}

void CDirectionalLight::Uninit()
{
	m_pVertexShader->Release();
	m_pPixelShader->Release();
}

void CDirectionalLight::Update()
{
	if (CInput::KeyPress(DIK_J)){
		m_Transform.rotation.z -= 0.1f;
	}
	else if (CInput::KeyPress(DIK_L)){
		m_Transform.rotation.z += 0.1f;
	}
	else if (CInput::KeyPress(DIK_I)){
		m_Transform.rotation.x -= 0.1f;
	}
	else if (CInput::KeyPress(DIK_K)){
		m_Transform.rotation.x += 0.1f;
	}


	D3DXMATRIX lrot;

	D3DXMatrixRotationYawPitchRoll(&lrot, m_Transform.rotation.y, m_Transform.rotation.x, m_Transform.rotation.z);
	m_Direction.x = lrot._21;
	m_Direction.y = lrot._22;
	m_Direction.z = lrot._23;
	m_Direction.w = 0.0f;
	D3DXVec4Normalize(&m_Direction, &m_Direction);
	m_Direction.w = 1.0f;


	RENDERER::GetConstantList().GetStruct<DirectionalLightBuffer>()->SetDirectinalLight(m_Direction, m_Position, m_Color);
}

void CDirectionalLight::Draw()
{
}

void CDirectionalLight::Imgui()
{
	static bool show_main_window = true;

	if (CInput::KeyTrigger(DIK_F1))
		show_main_window = !show_main_window;


	if (show_main_window)
	{
		static bool is_open = true;

		ImGuiWindowFlags static_flag = 0;
		static ImVec4 static_color = ImVec4(m_Color.x, m_Color.y, m_Color.z, 1.00f);

		m_Color.x = static_color.x;
		m_Color.y = static_color.y;
		m_Color.z = static_color.z;

		ImGui::Begin("DirectionalLight", &is_open, static_flag);
		ImGui::InputFloat3("LightDirection", m_Transform.rotation, 1);
		ImGui::ColorEdit3("Color", (float*)&static_color);

		ImGui::Checkbox("DirectionalLight", &m_Enable);

		ImGui::End();
	}
}
