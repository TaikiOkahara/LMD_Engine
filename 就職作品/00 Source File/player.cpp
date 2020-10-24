#include "base.h"
#include "director.h"
#include "renderer.h"
#include "input.h"

#include "camera.h"

#include "player.h"

//
//
//
void CPlayer::Init()
{
	
	m_AnimModel = new CAnimationModel();

	m_AnimModel->LoadModel("../02 Visual File//Akai//Akai_Idle.fbx", D3DXVECTOR3(0,2,0));
	m_AnimModel->LoadTexture("../02 Visual File//Akai//texture.ini");
	m_AnimModel->LoadAnimation("../02 Visual File//Akai//Akai_Run.fbx", "Run");
	m_AnimModel->LoadAnimation("../02 Visual File//Akai//Akai_Idle.fbx", "Idle");

	m_Position = D3DXVECTOR3(-2.5f, 0.0f, -3.5f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.008f, 0.008f, 0.008f);
	//m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	//　入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[]{
	{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONEINDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONEWEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } };


	//シェーダー作成
	RENDERER::CreateVertexShader(&m_VertexShader,&m_VertexLayout,layout,7,"SkeletalVertexShader.cso");
	RENDERER::CreatePixelShader(&m_PixelShader, "SkeletalPixelShader.cso");

	D3DXQuaternionIdentity(&m_Quaternion);
	m_OldPosition = m_Position;
	m_OldForward = GetForward();
	m_OldForward.y = 0;

}
//
//
//
void CPlayer::Uninit()
{

	m_AnimModel->Unload();
	delete m_AnimModel;
}
//
//
//
void CPlayer::Update()
{

	Scene* scene = Base::GetScene();

	CCamera* camera = scene->GetGameObject<CCamera>(0);

	D3DXVECTOR3 cameraright = camera->GetRight();
	cameraright.y = 0;
	D3DXVECTOR3 cameraforward = camera->GetForward();
	cameraforward.y = 0;
	D3DXVECTOR3 camerarotation = camera->GetRotation();
	camerarotation.x = 0;
	camerarotation.z = 0;
	



	D3DXVECTOR3 position(0,0,0);
	float rotation = 0;

	if (Keyboard_IsPress(DIK_W))
	{
		position += cameraforward;
		rotation = camerarotation.y + 2 * D3DX_PI;
		if (Keyboard_IsPress(DIK_A)){
			position -= cameraright;
			rotation = camerarotation.y + 2 * D3DX_PI - D3DX_PI/4;
		}
		else if (Keyboard_IsPress(DIK_D)) {
			position += cameraright;
			rotation = camerarotation.y + 2 * D3DX_PI + D3DX_PI / 4;
		}
	}
	else if (Keyboard_IsPress(DIK_S))
	{
		position -= cameraforward;
		rotation = camerarotation.y + D3DX_PI;
		if (Keyboard_IsPress(DIK_A)) {
			position -= cameraright;
			rotation = camerarotation.y + D3DX_PI + D3DX_PI / 4;
		}
		else if (Keyboard_IsPress(DIK_D)) {
			position += cameraright;
			rotation = camerarotation.y + D3DX_PI - D3DX_PI / 4;
		}
	}
	else if (Keyboard_IsPress(DIK_A))
	{
		position -= cameraright;
		rotation = camerarotation.y -D3DX_PI/2;
	}
	else if (Keyboard_IsPress(DIK_D))
	{
		position += cameraright;
		rotation = camerarotation.y + D3DX_PI/2;
	}
	

	D3DXVec3Normalize(&position, &position);
	m_Position += position * 0.1f;
	
	if (rotation != 0){
		m_Rotation.y = rotation;
	}
	
	

	if (m_Position != m_OldPosition)
		m_AnimModel->Update("Run", m_Frame);
	else
		m_AnimModel->Update("Idle", m_Frame);
	
	
	m_OldPosition = m_Position;


	m_Frame++;
}
//
//
//
void CPlayer::Draw()
{
	//　マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y + D3DX_PI, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	RENDERER::SetWorldMatrix(world);

	RENDERER::m_pDeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_PixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_VertexLayout);


	m_AnimModel->Draw();

}

void CPlayer::Imgui()
{
	static bool show_player_winow = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (Keyboard_IsTrigger(DIK_F1))
		show_player_winow = !show_player_winow;

	if (show_player_winow)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Player", &lw_is_open, lw_flag);



		ImGui::InputFloat3("Position", m_Position, 1);
		ImGui::InputFloat3("Rotation", m_Rotation, 1);

		ImGui::End();
	}
}