#include "base.h"
#include "director.h"
#include "renderer.h"
#include "input.h"
#include "StaticMesh.h"
#include "pointLight.h"
#include "chandelier.h"
#include "player.h"

void CPointLight::Init()
{
	m_pMesh = new StaticMesh();
	m_pMesh->LoadModel("../02 Visual File//UV.fbx");
	
	m_Collision.Init(D3DXVECTOR3(1, 1, 1), D3DXVECTOR3(0, 0, 0));
	
	{

		D3DXVECTOR3 rot,scale;
		rot = D3DXVECTOR3(0, 0, 0);
		scale = D3DXVECTOR3(12.0f, 12.0f, 1.0f);

	

		for (int i = 0; i < 3; i++)
		{
			
			m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-2.5f, 2.0f, 10.0f * i + 5.0f),rot, scale });

			m_PointLight[i] = POINTLIGHT{ D3DXVECTOR3(1.0f, 0.5f, 0.0f) ,100,D3DXVECTOR3(0.1f, 0.25f, 0.1f) ,0};
		}


		//一番奥の壁のライト
		m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(-2.5f, 2.0f, 32.5f),rot, scale });
		m_PointLight[3] = POINTLIGHT{ D3DXVECTOR3(1.0f, 0.5f, 0.0f) ,100,D3DXVECTOR3(0.1f, 0.15f, 0.1f) ,0 };


		//Playerlight
		m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(5.0f, 5.0f, 1.0f),rot, scale });
		m_PointLight[4] = POINTLIGHT{ D3DXVECTOR3(1.0f, 1.0f, 1.0f) ,100,D3DXVECTOR3(1.0f, 1.0f, 0.1f) ,0 };


		
	}

	
	//シェーダー作成
	
	RENDERER::CreateVertexShader(&m_pVertexShader, &m_pCommonVertexLayout,nullptr, 0, "pointLightVS.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "pointLightPS.cso");

	//InitInstance();
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
	//SAFE_RELEASE(m_pVertexLayout);
	/*SAFE_RELEASE(m_pPointLightBuffer);
	SAFE_RELEASE(m_pPointLightBufferSRV);*/
}

void CPointLight::Update()
{
	CPlayer* player = Base::GetScene()->GetGameObject<CPlayer>();
	m_TransformList[4].position = player->GetPosition();
	
}

void CPointLight::Draw()
{
	RENDERER::PointLighting();




	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_pCommonVertexLayout);

	RENDERER::m_ConstantBufferList.GetStruct<PointLightBuffer>()->Set(m_PointLight);
	
	
	//　マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixRotationYawPitchRoll(&rot, m_Transform.rotation.y, m_Transform.rotation.x, m_Transform.rotation.z);


	for (int i = 0; i < m_TransformList.size() && LIGHT_MAX; i++)
	{
		D3DXMatrixScaling(&scale, m_TransformList[i].scale.x, m_TransformList[i].scale.y, i * 1.0f);//xに本当のサイズを入れる、zにインデックス番号を入れる
		
		if (i == 4 && !m_EnablePlayerPointLight)
			D3DXMatrixScaling(&scale, m_TransformList[i].scale.x, m_TransformList[i].scale.y, -1);

		

		D3DXMatrixTranslation(&trans, m_TransformList[i].position.x, m_TransformList[i].position.y, m_TransformList[i].position.z);
		world = scale * rot * trans;
		
		RENDERER::m_ConstantBufferList.GetStruct<WorldBuffer>()->Set(world);
		
		
		m_pMesh->Draw();
	}


	
}

void CPointLight::Imgui()
{
	static bool show_light_window = true;


	if (CInput::KeyTrigger(DIK_F1))
		show_light_window = !show_light_window;

	if (show_light_window)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;
		ImGuiWindowFlags flag = 0;
		
		ImGui::Begin("PointLight", &lw_is_open, lw_flag);

		//PlayerPointLight-----------------------------------------------------------
		if (ImGui::TreeNode("Player Light"))
		{
			D3DXVECTOR3 pos;
			pos = m_TransformList[4].position;
			ImGui::InputFloat3("Position", pos, 1);

			ImGui::Checkbox("Enable", &m_EnablePlayerPointLight);

			static ImVec4 player_clear_color = ImVec4(m_PointLight[4].color.x, m_PointLight[4].color.y, m_PointLight[4].color.z, 1.00f);

			m_PointLight[4].color.x = player_clear_color.x;
			m_PointLight[4].color.y = player_clear_color.y;
			m_PointLight[4].color.z = player_clear_color.z;
			ImGui::ColorEdit3("Color", (float*)&player_clear_color);

			ImGui::SliderFloat("Constant Attenuation coefficient ", &m_PointLight[4].calc.x, 0.0f, 5.0f);
			ImGui::SliderFloat("Linear Attenuation coefficient", &m_PointLight[4].calc.y, 0.0f, 1.0f);
			ImGui::SliderFloat("2nd Constant Attenuation coefficient", &m_PointLight[4].calc.z, 0.0f, 10.0f);
			ImGui::SliderFloat("Intensity", &m_PointLight[4].intensity, 0.0f, 200.0f);

			ImGui::TreePop();
		}
	
		

		ImGui::Separator();
		//ScenePointLight-----------------------------------------------------------

		

		if (ImGui::TreeNode("Point Light List"))
		{

			static ImVec4 clear_color = ImVec4(m_PointLight[0].color.x, m_PointLight[0].color.y, m_PointLight[0].color.z, 1.00f);

			m_PointLight[0].color.x = clear_color.x;
			m_PointLight[0].color.y = clear_color.y;
			m_PointLight[0].color.z = clear_color.z;
			ImGui::ColorEdit3("Color", (float*)&clear_color);

			ImGui::SliderFloat("Constant Attenuation coefficient ", &m_PointLight[0].calc.x, 0.0f, 5.0f);
			ImGui::SliderFloat("Linear Attenuation coefficient", &m_PointLight[0].calc.y, 0.0f, 1.0f);
			ImGui::SliderFloat("2nd Constant Attenuation coefficient", &m_PointLight[0].calc.z, 0.0f, 10.0f);
			ImGui::SliderFloat("Intensity", &m_PointLight[0].intensity, 0.0f, 200.0f);

			



			ImGui::Separator();

			

			ImGui::TreePop();
		}

		ImGui::End();
	}
}