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
	//Light::Init();

	
	m_DL_Intensity = D3DXVECTOR4(1, 0, 0, 0);
	//g_DLight.Eye = D3DXVECTOR4(0, 0, 0, 0);


	//Light::SetDirectionalLight(D3DXVECTOR4(0, -1,0, 0), 1.0f);
	
	
	m_PL = D3DXVECTOR3(0.1f, 1.5f, 1.5f);
	//Light::SetPointLight(&m_PL, &m_PL_Intensity, &m_PL_Range, &m_PL_Rate);

	
	

	m_Position = D3DXVECTOR3(0.0f,0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.5f, 0.0f, 3.5f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);


}
//
//
//
void CSceneLight::Uninit()
{
	//Light::Uninit();

	
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


	//Light::Update();

}
//
//
//
void CSceneLight::Draw()
{

	//　マトリクス設定
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
	//Light::SetDirectionalLight(LightVector, 1.0f);


	RENDERER::SetDirectionalLight(LightVector, LightView);
	//RENDERER::SetPLight(Light::GetPointData());
	

}

void CSceneLight::Imgui()
{
	static bool show_main_window = true;
	//static bool show_another_window = true;
	

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (Keyboard_IsTrigger(DIK_F1))
		show_main_window = !show_main_window;


	if (show_main_window)
	{
		static float f1 = 0.0f;
		static int counter1 = 0;
		static int radio = 0;

		ImGuiWindowFlags flag = 0;

		

		static bool is_open = true;

		ImGui::Begin("DirectionalLight", &is_open, flag);

		

		//ImGui::SliderFloat2("CursorPoint",(float)poi.x,(float)poi.y);               // Display some text (you can use a format strings too)

		//CSceneLight* light = Base::GetScene()->GetGameObject<CSceneLight>(0);

		ImGui::InputFloat3("LightDirection", m_Rotation, 1);


		ImGui::Checkbox("Open/Close", &is_open);      // チェックボックスがクリックされるとis_openが反転し、trueならチェックマークが表示されます。
		//ImGui::Checkbox("Trigger", &show_another_window);

		ImGui::SliderFloat("float", &f1, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		ImGui::RadioButton("RadioButton 0", &radio, 0); ImGui::SameLine();
		ImGui::RadioButton("RadioButton 1", &radio, 1); ImGui::SameLine();
		ImGui::RadioButton("RadioButton 2", &radio, 2);

		if (ImGui::Button("Button"))	// "Button"が押されるとtrueになる
			counter1++;


		/*
			"##1" や "##2" のような文字列を入れることで互いを区別できるようになり、チェックボックスをクリックしても別のウィンドウの開閉ができるようになります。
			"##"以降の文字列は描画されません。"Open/Close" と描画されます。"##"を使うのはあくまでコード内で区別するための作法です。
			"##"以降の文字列は1や2でなくても構いません。

		*/

		ImGui::SameLine(); // 次に書くUIパーツを現在と同じ行に配置します。

		ImGui::Text("counter = %d", counter1);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

	}
}