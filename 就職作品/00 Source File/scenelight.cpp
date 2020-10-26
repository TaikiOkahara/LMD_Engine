#include "director.h"
#include "renderer.h"
#include "light.h"
#include "input.h"
#include "scenelight.h"
#include "Imgui11.h"

//
//
//
void CSceneLight::Init()
{
	
	
	//m_DL_Intensity = D3DXVECTOR4(1, 0, 0, 0);
	

	m_Position = D3DXVECTOR3(0.0f,0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.5f, 0.0f, 3.5f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);


}
//
//
//
void CSceneLight::Uninit()
{

}
//
//
//
void CSceneLight::Update()
{
	

	if (Keyboard_IsPress(DIK_J))
	{
		m_Rotation.z -= 0.1f;
	}
	else if (Keyboard_IsPress(DIK_L))
	{
		m_Rotation.z += 0.1f;
	}
	else if (Keyboard_IsPress(DIK_I))
	{
		m_Rotation.x -= 0.1f;
	}
	else if (Keyboard_IsPress(DIK_K))
	{
		m_Rotation.x += 0.1f;
	}


}
//
//
//
void CSceneLight::Draw()
{

	//�@�}�g���N�X�ݒ�
	D3DXMATRIX scale, rot, trans;

	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	

	D3DXMATRIX LightView;
	D3DXMatrixInverse(&LightView, NULL, &LightView);


	D3DXVECTOR4 LightVector;
	LightVector.x = rot._21;
	LightVector.y = rot._22;
	LightVector.z = rot._23;
	LightVector.w = 0.0f;
	D3DXVec4Normalize(&LightVector, &LightVector);
	LightVector.w = 1.0f;


	RENDERER::SetDirectionalLight(LightVector, LightView);
	

}

void CSceneLight::Imgui()
{
	static bool show_main_window = true;

	if (Keyboard_IsTrigger(DIK_F1))
		show_main_window = !show_main_window;


	if (show_main_window)
	{

		ImGuiWindowFlags flag = 0;


		static bool is_open = true;

		ImGui::Begin("DirectionalLight", &is_open, flag);
		ImGui::InputFloat3("LightDirection", m_Rotation, 1);

		/*
			"##1" �� "##2" �̂悤�ȕ���������邱�ƂŌ݂�����ʂł���悤�ɂȂ�A�`�F�b�N�{�b�N�X���N���b�N���Ă��ʂ̃E�B���h�E�̊J���ł���悤�ɂȂ�܂��B
			"##"�ȍ~�̕�����͕`�悳��܂���B"Open/Close" �ƕ`�悳��܂��B"##"���g���̂͂����܂ŃR�[�h���ŋ�ʂ��邽�߂̍�@�ł��B
			"##"�ȍ~�̕������1��2�łȂ��Ă��\���܂���B

		*/

		ImGui::End();
	}
}