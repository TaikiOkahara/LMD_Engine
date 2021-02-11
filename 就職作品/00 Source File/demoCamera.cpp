/*---------------------------------------
*　demoCame.cpp
*
*@author：Okahara Taiki
----------------------------------------*/
#include "base.h"
#include "director.h"
#include "renderer.h"
#include "demoCamera.h"
#include "input.h"
//#include "player.h"
//#include "wall.h"
//#include "pillar.h"
#include "Imgui11.h"
#include "calculation.h"
//#include "ceiling.h"
#include "floor.h"


void CDemoCamera::Init()
{
	
	m_Distance = D3DXVECTOR3(0, 1.5, -5);

	m_Angle = D3DX_PI / 4;
	m_Aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
	m_Near = CAMERA_NEAR;
	m_Far = CAMERA_FAR;

	m_Transform.position = D3DXVECTOR3(0, 10, 40);

	m_virtualTransform.position = D3DXVECTOR3(0, 0, 0);
	m_virtualTransform.rotation = D3DXVECTOR3(0, D3DX_PI, 0);

	//仮想カメラ枠
	{
		D3DXVECTOR3* pVtx = new D3DXVECTOR3[16];

		pVtx[0] = m_virtualTransform.position;
		pVtx[1] = D3DXVECTOR3(
			-m_Far * (FLOAT)tan(m_Angle / 2) * m_Aspect,
			-m_Far * (FLOAT)tan(m_Angle / 2),
			m_Far),

			pVtx[2] = m_virtualTransform.position;
		pVtx[3] = D3DXVECTOR3(
			-m_Far * (FLOAT)tan(m_Angle / 2) * m_Aspect,
			m_Far * (FLOAT)tan(m_Angle / 2),
			m_Far);

		pVtx[4] = m_virtualTransform.position;
		pVtx[5] = D3DXVECTOR3(
			m_Far * (FLOAT)tan(m_Angle / 2) * m_Aspect,
			-m_Far * (FLOAT)tan(m_Angle / 2),
			m_Far);

		pVtx[6] = m_virtualTransform.position;
		pVtx[7] = D3DXVECTOR3(
			m_Far * (FLOAT)tan(m_Angle / 2) * m_Aspect,
			m_Far * (FLOAT)tan(m_Angle / 2),
			m_Far);
		//--------------------------------------------------
		pVtx[8] = D3DXVECTOR3(
			-m_Far * (FLOAT)tan(m_Angle / 2) * m_Aspect,
			-m_Far * (FLOAT)tan(m_Angle / 2),
			m_Far);
		pVtx[9] = D3DXVECTOR3(
			-m_Far * (FLOAT)tan(m_Angle / 2) * m_Aspect,
			m_Far * (FLOAT)tan(m_Angle / 2),
			m_Far);

		pVtx[10] = D3DXVECTOR3(
			-m_Far * (FLOAT)tan(m_Angle / 2) * m_Aspect,
			m_Far * (FLOAT)tan(m_Angle / 2),
			m_Far);
		pVtx[11] = D3DXVECTOR3(
			m_Far * (FLOAT)tan(m_Angle / 2) * m_Aspect,
			m_Far * (FLOAT)tan(m_Angle / 2),
			m_Far);

		pVtx[12] = D3DXVECTOR3(
			m_Far * (FLOAT)tan(m_Angle / 2) * m_Aspect,
			m_Far * (FLOAT)tan(m_Angle / 2),
			m_Far);
		pVtx[13] = D3DXVECTOR3(
			m_Far * (FLOAT)tan(m_Angle / 2) * m_Aspect,
			-m_Far * (FLOAT)tan(m_Angle / 2),
			m_Far);

		pVtx[14] = D3DXVECTOR3(
			m_Far * (FLOAT)tan(m_Angle / 2) * m_Aspect,
			-m_Far * (FLOAT)tan(m_Angle / 2),
			m_Far);
		pVtx[15] = D3DXVECTOR3(
			-m_Far * (FLOAT)tan(m_Angle / 2) * m_Aspect,
			-m_Far * (FLOAT)tan(m_Angle / 2),
			m_Far);


		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(D3DXVECTOR3) * 16;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vrData;
		vrData.pSysMem = &pVtx[0];
		RENDERER::GetDevice()->CreateBuffer(&bd, &vrData, &m_pVertexBuffer);

		delete[] pVtx;
	}


	//　入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[]{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,	0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//シェーダー作成
	RENDERER::CreateVertexShader(&m_VertexShader, &m_VertexLayout, layout, ARRAYSIZE(layout), "collisionVS.cso");
	RENDERER::CreatePixelShader(&m_PixelShader, "collisionPS.cso");
}


void CDemoCamera::Uninit()
{
	SAFE_RELEASE(m_VertexShader);
	SAFE_RELEASE(m_PixelShader);
	SAFE_RELEASE(m_VertexLayout);
}

void CDemoCamera::Update()
{

	/*CPlayer* player = Base::GetScene()->GetGameObject<CPlayer>();*/

	
	m_OffsetPosition = D3DXVECTOR3(0, 0, 0);//player->GetPosition();

	if (CInput::KeyPress(DIK_A)) {
		m_Transform.position.x -= 1.0;
	}
	else if (CInput::KeyPress(DIK_D)) {
		m_Transform.position.x += 1.0;
	}
	else if (CInput::KeyPress(DIK_W)) {
		m_Transform.position.z += 1.0;
	}
	else if (CInput::KeyPress(DIK_S)) {
		m_Transform.position.z -= 1.0;
	}

	m_Target = D3DXVECTOR3(0,0,0);// m_OffsetPosition + D3DXVECTOR3(0, 1, 0);


	if (CInput::KeyPress(DIK_LEFTARROW)){	
		m_Transform.rotation.y -= 0.02f;
	}
	else if (CInput::KeyPress(DIK_RIGHTARROW)) {
		m_Transform.rotation.y += 0.02f;
	}

	if (CInput::KeyPress(DIK_UPARROW)) {
		m_Transform.rotation.x -= 0.02f;
	}
	else if (CInput::KeyPress(DIK_DOWNARROW)) {
		m_Transform.rotation.x += 0.02f;
		if (m_Transform.rotation.x >= (D3DX_PI /180) * 70)
			m_Transform.rotation.x = (D3DX_PI / 180) * 70;
	}

	//注支店との距離更新
	if (CInput::KeyPress(DIK_Q)) {
		m_Distance.z -= 0.05f;
	}
	else if (CInput::KeyPress(DIK_E)) {
		m_Distance.z += 0.05f;
	}
	

	D3DXVECTOR3 offset = m_Distance;

	D3DXMATRIX rot;
	D3DXMatrixRotationYawPitchRoll(&rot, m_Transform.rotation.y, m_Transform.rotation.x, m_Transform.rotation.z);
	D3DXVec3TransformCoord(&offset, &offset, &rot);



	//m_Transform.position = offset + m_OffsetPosition;


	//仮想カメラの回転
	if (CInput::KeyPress(DIK_F)) {
		m_virtualTransform.rotation.y -= D3DX_PI/180;
	}
	else if (CInput::KeyPress(DIK_H)) {
		m_virtualTransform.rotation.y += D3DX_PI/180;
	}
	


	//カメラとプレイヤーの間に障害物があった場合、カメラ位置を移動させる
	/*if (m_CameraControl)
	{
		CFloor* floor = Base::GetScene()->GetGameObject<CFloor>();
		if (floor != NULL)
			m_Transform.position = CameraRayIntersect(floor);
	}*/



	D3DXMATRIX viewOldMatrix;
	D3DXMATRIX projOldMatrix;

	viewOldMatrix = m_ViewMatrix;
	projOldMatrix = m_ProjMatrix;


	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Transform.position, &m_Target, &D3DXVECTOR3(0, 1, 0));
	RENDERER::GetConstantList().GetStruct<ViewBuffer>()->Set(m_ViewMatrix,viewOldMatrix);

	D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, m_Angle, m_Aspect, m_Near, m_Far);
	RENDERER::GetConstantList().GetStruct<ProjBuffer>()->Set(m_ProjMatrix,projOldMatrix);


	



	//視錐台カリング用視錐台頂点計算
	D3DXMATRIX vp, invvp;
	D3DXMATRIX virtualViewMatrix;
	D3DXMATRIX virtualProjMatrix;

	D3DXVECTOR3 forward = GetVirtualCameraForward();
	D3DXMatrixLookAtLH(&virtualViewMatrix, &m_virtualTransform.position, &forward, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixPerspectiveFovLH(&virtualProjMatrix, m_Angle, m_Aspect, m_Near, m_Far);

	vp = virtualViewMatrix * virtualProjMatrix;

	D3DXMatrixInverse(&invvp, NULL, &vp);

	D3DXVECTOR3 vpos[4];
	D3DXVECTOR3 wpos[4];

	vpos[0] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	vpos[1] = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
	vpos[2] = D3DXVECTOR3(-1.0f,-1.0f, 1.0f);
	vpos[3] = D3DXVECTOR3( 1.0f,-1.0f, 1.0f);
	
	D3DXVec3TransformCoord(&wpos[0], &vpos[0], &invvp);
	D3DXVec3TransformCoord(&wpos[1], &vpos[1], &invvp);
	D3DXVec3TransformCoord(&wpos[2], &vpos[2], &invvp);
	D3DXVec3TransformCoord(&wpos[3], &vpos[3], &invvp);

	m_CullingWPos[0] = D3DXVECTOR4(wpos[0].x,wpos[0].y,wpos[0].z,0);
	m_CullingWPos[1] = D3DXVECTOR4(wpos[1].x,wpos[1].y,wpos[1].z,0);
	m_CullingWPos[2] = D3DXVECTOR4(wpos[2].x,wpos[2].y,wpos[2].z,0);
	m_CullingWPos[3] = D3DXVECTOR4(wpos[3].x,wpos[3].y,wpos[3].z,0);
	
	D3DXVECTOR4 setWorldCamera[4];

	setWorldCamera[0] = m_CullingWPos[0];
	setWorldCamera[1] = m_CullingWPos[1];
	setWorldCamera[2] = m_CullingWPos[2];
	setWorldCamera[3] = m_CullingWPos[3];
	
	RENDERER::GetConstantList().GetStruct<CullingBuffer>()->SetWorldCamera(&setWorldCamera[0]);



	D3DXVECTOR4 setCameraPos;
	setCameraPos = D3DXVECTOR4(m_Transform.position.x, m_Transform.position.y, m_Transform.position.z, 0);
	RENDERER::GetConstantList().GetStruct<CameraBuffer>()->SetCamera(setCameraPos);
	
	setCameraPos = D3DXVECTOR4(m_virtualTransform.position.x, m_virtualTransform.position.y, m_virtualTransform.position.z, 0);
	RENDERER::GetConstantList().GetStruct<CameraBuffer>()->SetCullingCameraPos(setCameraPos);

}


void CDemoCamera::Draw()
{
	//仮想カメラ外枠描画

	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, 1, 1, 1);
	D3DXMatrixRotationYawPitchRoll(&rot, m_virtualTransform.rotation.y, m_virtualTransform.rotation.x, m_virtualTransform.rotation.z);
	D3DXMatrixTranslation(&trans, m_virtualTransform.position.x, m_virtualTransform.position.y, m_virtualTransform.position.z);//影は少し地面から離す
	world = scale * rot * trans;

	RENDERER::GetConstantList().GetStruct<WorldBuffer>()->Set(world);


	RENDERER::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	RENDERER::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
	RENDERER::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	RENDERER::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	UINT stride = sizeof(D3DXVECTOR3);
	UINT offset = 0;
	RENDERER::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	RENDERER::GetDeviceContext()->Draw(16, 0);

	RENDERER::GetDeviceContext()->VSSetShader(NULL, NULL, 0);
	RENDERER::GetDeviceContext()->PSSetShader(NULL, NULL, 0);


}


void CDemoCamera::Imgui()
{
	static bool show = true;

	

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (CInput::KeyTrigger(DIK_F1))
		show = !show;

	if (show)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Camera", &lw_is_open, lw_flag);

		ImGui::Checkbox("CameraControl", &m_CameraControl);


		ImGui::InputFloat3("Position", m_Transform.position, 1);
		ImGui::InputFloat3("Rotation", m_Transform.rotation, 1);
		ImGui::InputFloat3("Target", m_Target, 1);
		ImGui::InputFloat3("Distance", m_Distance, 1);

		ImGui::End();
	}
}