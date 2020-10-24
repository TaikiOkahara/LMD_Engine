#include "base.h"
#include "director.h"
#include "renderer.h"
#include "input.h"
#include "StaticMesh.h"
#include "pointLight.h"
#include "player.h"

void CPointLight::Init()
{
	m_pMesh = new StaticMesh();
	m_pMesh->LoadModel("../02 Visual File//UV.fbx");
	
	m_Position = D3DXVECTOR3(-2.5f, 0.0f, 5.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(100.0f, 100.0f, 100.0f);

	posList = new D3DXVECTOR3[128];
	{


		posList[0] = D3DXVECTOR3(-2.5f, 0.0f, 5.0f);
		m_PointLight.Color[0] = D3DXVECTOR4(1,1,1, 1);
		m_PointLight.CalcInfo[0] = D3DXVECTOR4(1, 1, 1, 10);

		//m_PointLight.Position[0] = D3DXVECTOR3(-2.5f, 0.0f, 5.0f);

		//for (int i = 0; i < 3; i++)
		//{
		//	D3DXMatrixTranslation(&trans, -2.5f, 0.0f, 2.0f + i * 2);
		//	world = scale * trans;
		//	D3DXMatrixTranspose(&world, &world);

		//	//m_MatrixList.push_back(world);
		//}

		
	}

	//　入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[]{
	{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } };


	//シェーダー作成
	RENDERER::CreateVertexShader(&m_pVertexShader, &m_pVertexLayout, layout, 5, "PointLightVertexShader.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "PointLightPixelShader.cso");

	//InitInstance();
	//UpdateInstance();//視錐台カリングを行う場合入れる


}

void CPointLight::Uninit()
{
	delete[] posList;

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

	CPlayer* player = Base::GetScene()->GetGameObject<CPlayer>(1);

	m_Position = player->GetPosition() +  player->GetForward() * 3.0f;
	m_Position.y = 0.0f;
	
}

void CPointLight::Draw()
{
	//RENDERER::SetPointLight(m_PointLight);
	
	//DrawInstance();
	//SetWorldMatrix();
	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_pVertexLayout);

	RENDERER::SetPointLight(m_PointLight);
	
	
	//　マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);


	for (int i = 0; i < 128; i++)
	{
		D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, i);//xに本当のサイズを入れる、zにインデックス番号を入れる

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

		ImGui::Begin("Light", &lw_is_open, lw_flag);

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
			ImGui::SliderFloat3("Color", &m_PointLight.Color[0].x, 0.0f, 1.0f);

			ImGui::SliderFloat("x : ", &m_PointLight.CalcInfo[0].x,0.0f,1.0f);
			ImGui::SliderFloat("y : ", &m_PointLight.CalcInfo[0].y,0.0f,1.0f);
			ImGui::SliderFloat("z : ", &m_PointLight.CalcInfo[0].z,0.0f,1.0f);
			ImGui::SliderFloat("Specular", &m_PointLight.CalcInfo[0].w, 0.0f, 30.0f);
			//ImGui::InputFloat("input float", &f0, 0.01f, 1.0f, "%.3f");

		}

		ImGui::End();
	}
}