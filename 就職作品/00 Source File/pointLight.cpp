#include "base.h"
#include "director.h"
#include "renderer.h"
#include "input.h"
#include "StaticMesh.h"
#include "pointLight.h"
#include "player.h"

void CPointLight::Init()
{
	{


		/*for (int i = 0; i < 3; i++)
		{
			D3DXVECTOR4 pos;
			pos = D3DXVECTOR4(0, 3, i * 3,1);
			m_PointList.push_back(&pos);

		}*/
		D3DXVECTOR4 pos = D3DXVECTOR4(-2.5, 2.0f, 5.0f, 3);


		//m_PointList.push_back(pos);//w�͌��̋���
		m_PointLight.LightPosition[0] = pos;
	}
	m_pMesh = new StaticMesh();
	m_pMesh->LoadModel("../02 Visual File//UV.fbx");
	//m_pMesh->LoadTexture("");

	m_Position = D3DXVECTOR3(0.0f, 1.0f, 4.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_Scale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);
	m_Scale = D3DXVECTOR3(100.0f, 100.0f, 100.0f);

	//�@���̓��C�A�E�g����
	D3D11_INPUT_ELEMENT_DESC layout[]{
	{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } };


	//�V�F�[�_�[�쐬
	RENDERER::CreateVertexShader(&m_pVertexShader, &m_pVertexLayout, layout, 5, "PointLightVertexShader.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "PointLightPixelShader.cso");








	//{//�T���v���R���X�^���g�o�b�t�@

	//	RENDERER::CreateStructuredBuffer(sizeof(D3DXVECTOR4), m_PointList.size(), &m_PointList[0], &m_pPointLightBuffer);


	//	//�V�F�[�_���\�[�X�r���[�����K�v������
	//	//�܂��AFormat�͕K��DXGI_FORMAT_UNKNOWN�ɂ��Ȃ��Ƃ����Ȃ�
	//	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	//	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	//	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	//	srvDesc.Buffer.FirstElement = 0;
	//	srvDesc.Buffer.NumElements = m_PointList.size();

	//	RENDERER::m_pDevice->CreateShaderResourceView(m_pPointLightBuffer, &srvDesc, &m_pPointLightBufferSRV);
	//	RENDERER::m_pDeviceContext->VSSetShaderResources(0, 1, &m_pPointLightBufferSRV);

	//	//Renderer�̃o�b�t�@�ɃZ�b�g
	//	RENDERER::SetPointLight(m_pPointLightBuffer, m_pPointLightBufferSRV);
	//}

	//�|�C���g���C�g�p�o�b�N�J�����O���X�^���C�Y�ݒ�
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rdc.DepthClipEnable = FALSE;
	//rdc.FrontCounterClockwise = TRUE;
	//rdc.MultisampleEnable = FALSE;

	RENDERER::m_pDevice->CreateRasterizerState(&rdc, &m_pBackCullingRasterizerState);

	//�A���t�@�u�����h�p�u�����h�X�e�[�g�쐬
	D3D11_BLEND_DESC blendd;
	ZeroMemory(&blendd, sizeof(D3D11_BLEND_DESC));
	blendd.IndependentBlendEnable =false;
	blendd.AlphaToCoverageEnable=false;
	blendd.RenderTarget[0].BlendEnable=true;
	blendd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;//���b�V���̃����_�����O�C���[�W
	blendd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;//�����_�[�^�[�Q�b�g�T�[�t�@�X�̃C���[�W
	blendd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;//�R�R�厖
	blendd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;//�R�R�厖
	blendd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	
	RENDERER::m_pDevice->CreateBlendState(&blendd, &m_pBackCullingBlendState);

	
}

void CPointLight::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	SAFE_RELEASE(m_pBackCullingRasterizerState);
	SAFE_RELEASE(m_pBackCullingBlendState);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pPointLightBuffer);
	SAFE_RELEASE(m_pPointLightBufferSRV);
}

void CPointLight::Update()
{

	/*D3DXVECTOR3 pos = Base::GetScene()->GetGameObject<CPlayer>(1)->GetPosition();

	m_PointLight.LightPosition[0] = D3DXVECTOR4(pos.x, pos.y, pos.z, 1);*/


	//RENDERER::SetPointLight(m_pPointLightBuffer, m_pPointLightBufferSRV);
	//RENDERER::m_pDeviceContext->UpdateSubresource(m_pPointLightBuffer, 0, NULL, &m_PointList[0], 0, 0);
	RENDERER::SetPointLight(m_PointLight);
}

void CPointLight::Draw()
{
	UINT mask = 0xffffffff;
	float blend[4] = { 1,1,1,1 };
	RENDERER::m_pDeviceContext->OMSetBlendState(m_pBackCullingBlendState, blend, mask);
	RENDERER::m_pDeviceContext->RSSetState(m_pBackCullingRasterizerState);
	
	SetWorldMatrix();
	//�s�N�Z���V�F�[�_�����I���W�i��
	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);//�I���W�i��
	RENDERER::m_pDeviceContext->IASetInputLayout(m_pVertexLayout);

	
	m_pMesh->Draw();

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

		for (int i = 0; i < 1; i++)
		{
			D3DXVECTOR3 pos = D3DXVECTOR3(m_PointLight.LightPosition[i].x, m_PointLight.LightPosition[i].y, m_PointLight.LightPosition[i].z);
			ImGui::InputFloat3("Position", pos, 1);
		}


		ImGui::End();
	}
}