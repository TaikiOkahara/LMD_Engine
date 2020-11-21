#include "base.h"
#include "director.h"
#include "renderer.h"
#include "input.h"
#include "StaticMesh.h"
#include "pointLight.h"
#include "chandelier.h"

void CPointLight::Init()
{
	m_pMesh = new StaticMesh();
	m_pMesh->LoadModel("../02 Visual File//UV.fbx");
	
	m_Collision.Init(D3DXVECTOR3(1, 1, 1), D3DXVECTOR3(0, 0, 0));
	//m_TransformList = new TRANSFORM[LIGHT_MAX];
	{

		/*for (int i = 0; i < LIGHT_MAX; i++)
		{
			
			m_TransformList[i].scale.x = -1;
		}*/

		for (int i = 0; i < 3; i++)
		{
			TRANSFORM transform;
			transform.position = D3DXVECTOR3(-2.5f, 2.0f, 10.0f * i + 5.0f);
			transform.scale = D3DXVECTOR3(5.0f, 5.0f, 1.0f);
			transform.rotation = D3DXVECTOR3(0, 0, 0);
			m_TransformList.push_back(transform);
			m_PointLight.pointList[i].color = D3DXVECTOR3(1.0f, 0.5f, 0.0f);
			m_PointLight.pointList[i].intensity = 1;
			m_PointLight.pointList[i].calc = D3DXVECTOR3(0.1f, 1.0f, 0.1f);
			m_PointLight.pointList[i].specular = 30;
		}
		
		
	}

	
	//シェーダー作成
	
	RENDERER::CreateVertexShader(&m_pVertexShader, &RENDERER::m_pCommonVertexLayout,nullptr, 0, "PointLightVertexShader.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "PointLightPixelShader.cso");

	InitInstance();
	//UpdateInstance();//視錐台カリングを行う場合入れる
	
}

void CPointLight::Uninit()
{

	//delete[] m_TransformList;

	m_pMesh->Unload();
	delete m_pMesh;

	m_Collision.Uninit();

	//UninitInstance();
	
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pPointLightBuffer);
	SAFE_RELEASE(m_pPointLightBufferSRV);
}

void CPointLight::Update()
{
	CChandelier* chandelier = Base::GetScene()->GetGameObject<CChandelier>(1);

	/*for (int i = 1; i < chandelier->GetMeshCount(); i++)
	{
		m_TransformList[i].position = chandelier->GetPosition(i) + D3DXVECTOR3(0, -1, 0);
		m_TransformList[i].scale = D3DXVECTOR3(5.0f, 5.0f, 1.0f);

		m_PointLight.pointList[i].color = D3DXVECTOR3(1.0f, 0.5f, 0.0f);
		m_PointLight.pointList[i].intensity = 1;
		m_PointLight.pointList[i].calc = D3DXVECTOR3(0.1f, 1.0f, 0.1f);
		m_PointLight.pointList[i].specular = 30;
	}*/


	
}

void CPointLight::Draw()
{
	
	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(RENDERER::m_pCommonVertexLayout);

	RENDERER::SetPointLight(m_PointLight);
	
	
	//　マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixRotationYawPitchRoll(&rot, m_Transform.rotation.y, m_Transform.rotation.x, m_Transform.rotation.z);


	for (int i = 0; i < m_TransformList.size() && LIGHT_MAX; i++)
	{
		
		D3DXMatrixScaling(&scale, m_TransformList[i].scale.x, m_TransformList[i].scale.y, i * 1.0f);//xに本当のサイズを入れる、zにインデックス番号を入れる

		D3DXMatrixTranslation(&trans, m_TransformList[i].position.x, m_TransformList[i].position.y, m_TransformList[i].position.z);
		world = scale * rot * trans;
		WORLDMATRIX worldMatrix;
		worldMatrix.worldMatrix = world;
		RENDERER::SetWorldMatrix(worldMatrix);
		
		
		m_pMesh->Draw();
	}


	
	
	


	
	//m_pMesh->DrawInstanced(m_MeshCount);
}

void CPointLight::Imgui()
{
	static bool show_light_window = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (CInput::KeyTrigger(DIK_F1))
		show_light_window = !show_light_window;

	if (show_light_window)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;
		ImGuiWindowFlags flag = 0;
		static ImVec4 clear_color = ImVec4(m_PointLight.pointList[0].color.x, m_PointLight.pointList[0].color.y, m_PointLight.pointList[0].color.z, 1.00f);

		m_PointLight.pointList[0].color.x = clear_color.x;
		m_PointLight.pointList[0].color.y = clear_color.y;
		m_PointLight.pointList[0].color.z = clear_color.z;

		ImGui::Begin("PointLight", &lw_is_open, lw_flag);

	
		for (int i = 0; i < m_TransformList.size(); i++)
		{
			D3DXVECTOR3 pos;
			pos = m_Transform.position;
			ImGui::InputFloat3("Position", pos, 1);
			//ImGui::SliderFloat3("Color", &m_PointLight.Color[0].x, 0.0f, 1.0f);

			ImGui::ColorEdit3("Color", (float*)&clear_color);

			ImGui::SliderFloat("x : ", &m_PointLight.pointList[0].calc.x,0.0f,5.0f);
			ImGui::SliderFloat("y : ", &m_PointLight.pointList[0].calc.y,0.0f,1.0f);
			ImGui::SliderFloat("z : ", &m_PointLight.pointList[0].calc.z,0.0f,10.0f);
			ImGui::SliderFloat("Specular", &m_PointLight.pointList[0].specular, 0.0f, 1.0f);
			//ImGui::InputFloat("input float", &f0, 0.01f, 1.0f, "%.3f");

		}

		ImGui::End();
	}
}