#include "director.h"
#include "renderer.h"
#include "input.h"
#include "scenelight.h"
#include "Imgui11.h"

//
//
//
void CSceneLight::Init()
{
	
	m_Transform.rotation = D3DXVECTOR3(0.5f, 0.0f, -0.5f);
	m_Transform.position = D3DXVECTOR3(-2.5, 1, 0);
	m_Transform.scale = D3DXVECTOR3(10, 10, 10);

	m_DirectionalLight.Color = D3DXVECTOR4(0.4f, 0.6f, 1, 1);

	m_pMesh = new StaticMesh();
	m_pMesh->LoadModel("../02 Visual File//CursorModel//cursor.fbx");
	m_pMesh->LoadTexture("");


	//シェーダー作成
	RENDERER::CreateVertexShader(&m_VertexShader, nullptr, nullptr, 0, "CursorVertexShader.cso");
	RENDERER::CreatePixelShader(&m_PixelShader, "CursorPixelShader.cso");
}
//
//
//
void CSceneLight::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	m_VertexShader->Release();
	m_PixelShader->Release();
}
//
//
//
void CSceneLight::Update()
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
//
//
//
void CSceneLight::Draw()
{

	

	D3DXMATRIX LightView;
	D3DXMatrixInverse(&LightView, NULL, &LightView);


	D3DXMATRIX lrot;

	D3DXMatrixRotationYawPitchRoll(&lrot, m_Transform.rotation.y, m_Transform.rotation.x, m_Transform.rotation.z);
	m_DirectionalLight.LightDir.x = lrot._21;
	m_DirectionalLight.LightDir.y = lrot._22;
	m_DirectionalLight.LightDir.z = lrot._23;
	m_DirectionalLight.LightDir.w = 0.0f;
	D3DXVec4Normalize(&m_DirectionalLight.LightDir, &m_DirectionalLight.LightDir);
	m_DirectionalLight.LightDir.w = 1.0f;
	
	RENDERER::SetDirectionalLight(m_DirectionalLight);



	RENDERER::m_pDeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_PixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(RENDERER::m_pCommonVertexLayout);


	//　マトリクス設定
	D3DXMATRIX world, scale, rot, trans, view;

	D3DXMatrixScaling(&scale, m_Transform.scale.x, m_Transform.scale.y, m_Transform.scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Transform.rotation.y , m_Transform.rotation.x, m_Transform.rotation.z);
	D3DXMatrixTranslation(&trans, m_Transform.position.x, m_Transform.position.y, m_Transform.position.z);
	world = scale * rot * trans;


	WORLDMATRIX w;
	w.worldMatrix = world;
	RENDERER::SetWorldMatrix(w);

	//m_pMesh->Draw();
}

void CSceneLight::Imgui()
{
	static bool show_main_window = true;

	if (CInput::KeyTrigger(DIK_F1))
		show_main_window = !show_main_window;


	if (show_main_window)
	{
		static bool is_open = true;

		ImGuiWindowFlags flag = 0;
		static ImVec4 clear_color = ImVec4(m_DirectionalLight.Color.x, m_DirectionalLight.Color.y, m_DirectionalLight.Color.z, 1.00f);
		
		m_DirectionalLight.Color.x = clear_color.x;
		m_DirectionalLight.Color.y = clear_color.y;
		m_DirectionalLight.Color.z = clear_color.z;
		
		
		

		ImGui::Begin("DirectionalLight", &is_open, flag);
		ImGui::InputFloat3("LightDirection", m_Transform.rotation, 1);


		ImGui::ColorEdit3("Color", (float*)&clear_color);
		
		ImGui::End();
	}
}