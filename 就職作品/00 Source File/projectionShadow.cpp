#include "base.h"
#include "director.h"
#include "renderer.h"
#include "input.h"
#include "projectionShadow.h"
#include "Imgui11.h"
#include "pointLight.h"
#include "player.h"

void CProjShadow::Init()
{

	//　入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[]{
	{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONEINDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONEWEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } };

	//シェーダー作成
	RENDERER::CreateVertexShader(&m_pVertexShader, &m_pVertexLayout, layout, ARRAYSIZE(layout), "projectionTextureMappingShadowVS.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "projectionTextureMappingShadowPS.cso");
}

void CProjShadow::Uninit()
{
	m_pVertexLayout->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();
}

void CProjShadow::Update()
{


}

void CProjShadow::Draw()
{
	CPlayer* player = Base::GetScene()->GetGameObject<CPlayer>();



	D3DXMATRIX Lightproj, Lightview, LightVP;

	//CDirectionalLight* camera = Base::GetScene()->GetGameObject<CCamera>();
	CPointLight* pointLight = Base::GetScene()->GetGameObject<CPointLight>();

	D3DXVECTOR3 lPos = pointLight->GetPosition(1);

	D3DXVECTOR3 pos,target;

	pos = lPos + D3DXVECTOR3(0,2,0);// D3DXVECTOR3(2, 3, 2) + player->GetPosition();
	target = player->GetPosition();

	D3DXMatrixLookAtLH(&Lightview, &pos, &target, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixPerspectiveFovLH(&Lightproj, D3DX_PI / 4, 1, 0.1f, 100.0f);
	D3DXMatrixMultiply(&LightVP, &Lightview, &Lightproj);

	D3DXMatrixTranspose(&LightVP,&LightVP);

	RENDERER::m_ConstantBufferList.GetStruct<DirectionalLightBuffer>()->SetMatrix(LightVP);



	RENDERER::PointLighting();


	RENDERER::SetRasterizerState(D3D11_CULL_MODE::D3D11_CULL_NONE);

	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_pVertexLayout);


	
	//プレイヤー影描画

	player->DrawShadow();

	RENDERER::CommonDraw();
	
}

void CProjShadow::Imgui()
{
}
