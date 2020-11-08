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
	
	
	//posList = new D3DXVECTOR3[128];
	//scaleList = new D3DXVECTOR3[128];


	{

		for (int i = 0; i < 2; i++)
		{
			posList[i] = D3DXVECTOR3(-2.5f, 0.0f, 5.0f);
			scaleList[i] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

			m_PointLight.Color[i] = D3DXVECTOR4(1, 1, 1, 1);
			m_PointLight.CalcInfo[i] = D3DXVECTOR4(1, 1, 1, 10);
		}
		




		//m_PointLight.Position[0] = D3DXVECTOR3(-2.5f, 0.0f, 5.0f);

		//for (int i = 0; i < 3; i++)
		//{
		//	D3DXMatrixTranslation(&trans, -2.5f, 0.0f, 2.0f + i * 2);
		//	world = scale * trans;
		//	D3DXMatrixTranspose(&world, &world);

		//	//m_MatrixList.push_back(world);
		//}

		
	}

	
	//シェーダー作成
	
	RENDERER::CreateVertexShader(&m_pVertexShader, &RENDERER::m_pCommonVertexLayout,nullptr, 0, "PointLightVertexShader.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "PointLightPixelShader.cso");

	//InitInstance();
	//UpdateInstance();//視錐台カリングを行う場合入れる

	//CChandelier* chandelier = Base::GetScene()->GetGameObject<CChandelier>(1);

	posList[0] = D3DXVECTOR3(-2.5f, 4.0f, 3.0f) + D3DXVECTOR3(0, -1, 0);
	scaleList[0] = D3DXVECTOR3(5.0f, 5.0f, 1.0f);
	m_PointLight.Color[0] = D3DXVECTOR4(1.0f, 0.5f, 0.0f, 1);
	m_PointLight.CalcInfo[0] = D3DXVECTOR4(0.1f, 1.0f, 0.1f, 30);
}

void CPointLight::Uninit()
{
	delete[] posList;
	delete[] scaleList;

	m_pMesh->Unload();
	delete m_pMesh;

	//UninitInstance();
	
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pPointLightBuffer);
	SAFE_RELEASE(m_pPointLightBufferSRV);
}

void CPointLight::Update()
{

	
}

void CPointLight::Draw()
{
	
	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(RENDERER::m_pCommonVertexLayout);

	RENDERER::SetPointLight(m_PointLight);
	
	
	//　マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);


	for (int i = 0; i < LIGHT_MAX; i++)
	{
		if (scaleList[i].x <= 0)
		{
			continue;//スケールのないライト描画スキップ
		}
		D3DXMatrixScaling(&scale, scaleList[i].x, scaleList[i].y, i);//xに本当のサイズを入れる、zにインデックス番号を入れる

		D3DXMatrixTranslation(&trans, posList[i].x, posList[i].y, posList[i].z);
		world = scale * rot * trans;
		RENDERER::SetWorldMatrix(world);
		
		
		m_pMesh->Draw();
	}


	
	
	


	
	//m_pMesh->DrawInstanced(m_MeshCount);
}

void CPointLight::Imgui()
{
	static bool show_light_window = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (Keyboard_IsTrigger(DIK_F1))
		show_light_window = !show_light_window;

	if (show_light_window)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;
		ImGuiWindowFlags flag = 0;
		static ImVec4 clear_color = ImVec4(m_PointLight.Color[0].x, m_PointLight.Color[0].y, m_PointLight.Color[0].z, 1.00f);

		m_PointLight.Color[0].x = clear_color.x;
		m_PointLight.Color[0].y = clear_color.y;
		m_PointLight.Color[0].z = clear_color.z;

		ImGui::Begin("PointLight", &lw_is_open, lw_flag);

		/*for (int i = 0; i < 1; i++)
		{
			D3DXVECTOR3 pos = D3DXVECTOR3(m_PointLight.LightPosition[i].x, m_PointLight.LightPosition[i].y, m_PointLight.LightPosition[i].z);
			ImGui::InputFloat3("Position", pos, 1);
		}*/
		for (int i = 0; i < 1; i++)
		{
			D3DXVECTOR3 pos;
			pos = m_Position;
			ImGui::InputFloat3("Position", pos, 1);
			//ImGui::SliderFloat3("Color", &m_PointLight.Color[0].x, 0.0f, 1.0f);

			ImGui::ColorEdit3("Color", (float*)&clear_color);

			ImGui::SliderFloat("x : ", &m_PointLight.CalcInfo[0].x,0.0f,1.0f);
			ImGui::SliderFloat("y : ", &m_PointLight.CalcInfo[0].y,0.0f,1.0f);
			ImGui::SliderFloat("z : ", &m_PointLight.CalcInfo[0].z,0.0f,5.0f);
			ImGui::SliderFloat("Specular", &m_PointLight.CalcInfo[0].w, 0.0f, 30.0f);
			//ImGui::InputFloat("input float", &f0, 0.01f, 1.0f, "%.3f");

		}

		ImGui::End();
	}
}