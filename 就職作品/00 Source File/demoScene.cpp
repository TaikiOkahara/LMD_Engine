/*---------------------------------------
*　demoScene.h
*
* カリング効果を確認するシーン
*@author：Okahara Taiki
----------------------------------------*/
#include "base.h"
#include "demoScene.h"
#include "game.h"
#include "Imgui11.h"
#include "input.h"

#include "demoCamera.h"
#include "pointLight.h"

#include "player.h"
#include "moss.h"

#include "demoGargoyle.h"

#include "demoCubeMap.h"


#include "demoDirectionalLight.h"
#include "fog.h"

void DemoScene::Init() {

	CGameObject::Load();
	CInstanceGameObject::Load();



	AddGameObject<CDemoCamera>(LAYER::HIDE);
	AddGameObject<CDemoCubeMap>(LAYER::HIDE);


	AddGameObject<CMoss>(LAYER::DRAW);
	AddGameObject<CDemoGargoyle>(LAYER::DRAW);
	
	AddPostProcess<CDemoDirectionalLight>();
	AddPostProcess<CFogEffect>();
	
}

void DemoScene::UnInit() {
	
	CScene::UnInit();


	CInstanceGameObject::Unload();
	CGameObject::Unload();
}

void DemoScene::Update() {

	if (m_TimeStop) return;
	CScene::Update();

	m_modelCount = GetGameObject<CDemoGargoyle>()->GetMeshCount();



	RENDERER::GetConstantList().GetStruct<ToggleBuffer>()->SetFrustumCullingEnable(m_EnableCulling);
	RENDERER::GetConstantList().GetStruct<ToggleBuffer>()->Set();


	//タイトルScene移動
	if (CInput::KeyTrigger(DIK_F8))
		Base::SetScene<Game>();
}

void DemoScene::Imgui()
{
	

	//シーンのデバッグは表示させない
	CScene::Imgui();

	//Sceneのデバッグ
	static bool show_scene_window = true;

	if (CInput::KeyTrigger(DIK_F2))
		show_scene_window = !show_scene_window;

	if (show_scene_window)
	{
		static float f1 = 0.0f;
		static int counter1 = 0;
		
		static int culling_radio = culling_radio;

		ImGuiWindowFlags flag = 0;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


		static bool is_open = true;

		ImGui::Begin("cullingDemo", &is_open, flag);


		ImGui::Checkbox("isStop", &m_TimeStop);


		ImGui::Text("Culling Mode");
		ImGui::RadioButton("None", &culling_radio, 0);
		ImGui::SameLine();
		ImGui::RadioButton("FrustumCulling", &culling_radio, 1);
		m_EnableCulling =  culling_radio;

		ImGui::Text("Model Count : %d / 22500", m_modelCount);
	

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

}
