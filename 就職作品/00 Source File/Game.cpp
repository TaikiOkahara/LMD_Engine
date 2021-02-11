/*---------------------------------------
*　game.h
*
* メインとなるシーン
*@author：Okahara Taiki
----------------------------------------*/
#include "base.h"
#include "game.h"
#include "title.h"
#include "demoScene.h"
#include "Imgui11.h"
#include "input.h"

#include "camera.h"
#include "pointLight.h"

#include "table.h"
#include "player.h"
#include "floor.h"
#include "wall.h"
#include "doorWay.h"
#include "pillar.h"
#include "halfArch.h"
#include "edge.h"
#include "pots.h"
#include "trim.h"
#include "gargoyle.h"
#include "stairs.h"
#include "ceiling.h"
#include "ceilingArch.h"
#include "chandelier.h"
#include "cubeMap.h"
#include "projectionShadow.h"

#include "directionalLight.h"
#include "fog.h"

void Game::Init() {

	CGameObject::Load();
	CInstanceGameObject::Load();



	AddGameObject<CCamera>(LAYER::HIDE);

	AddGameObject<CCubeMap>(LAYER::HIDE);

	AddGameObject<CTable>(LAYER::DRAW);
	AddGameObject<CFloor>(LAYER::DRAW);
	AddGameObject<CWall>(LAYER::DRAW);
	AddGameObject<CPillar>(LAYER::DRAW);
	AddGameObject<CEdge>(LAYER::DRAW);
	AddGameObject<CDoorWay>(LAYER::DRAW);

	AddGameObject<CGargoyle>(LAYER::DRAW);
	AddGameObject<CTrim>(LAYER::DRAW);
	AddGameObject<CCeiling>(LAYER::DRAW);
	AddGameObject<CCeilingArch>(LAYER::DRAW);
	AddGameObject<CPots>(LAYER::DRAW);
	AddGameObject<CChandelier>(LAYER::DRAW);
	AddGameObject<CStairs>(LAYER::DRAW);
	AddGameObject<CHalfArch>(LAYER::DRAW);

	AddGameObject<CProjShadow>(LAYER::DRAW);

	AddGameObject<CPlayer>(LAYER::DRAW);//プレイヤー


	AddGameObject<CPointLight>(LAYER::LIGHT);//一番最後に描画
	
	
	
	AddPostProcess<CDirectionalLight>();
	AddPostProcess<CFogEffect>();
	
}

void Game::UnInit() {
	
	CScene::UnInit();


	CInstanceGameObject::Unload();
	CGameObject::Unload();
}

void Game::Update() {

	if (m_TimeStop) return;
	CScene::Update();

	//ポストプロセス情報をシェーダーにセット

	//Deferred
	RENDERER::GetConstantList().GetStruct<EffectBuffer>()->SetDeferredParam(m_DeferredType * 1.0f, m_ShadowEnable, m_GBufferRenderEnable);
	RENDERER::GetConstantList().GetStruct<EffectBuffer>()->Set();
	
	//SSAO
	RENDERER::GetConstantList().GetStruct<EffectBuffer>()->SetAO(m_aoEnable,m_aoHemRedius,m_aoZfar,m_aoPower);

	//Culling
	RENDERER::GetConstantList().GetStruct<ToggleBuffer>()->SetFrustumCullingEnable(m_EnableCulling);
	RENDERER::GetConstantList().GetStruct<ToggleBuffer>()->Set();


}

void Game::Imgui()
{
	

	
	CScene::Imgui();

	//Sceneのデバッグ
	static bool show_scene_window = true;

	if (CInput::KeyTrigger(DIK_F2))
		show_scene_window = !show_scene_window;

	if (show_scene_window)
	{
		static float f1 = 0.0f;
		static int counter1 = 0;
		
		static int static_render_radio = m_DeferredType;
		static int static_culling_radio = m_EnableCulling;
		static bool static_aoEnable = m_aoEnable;
		static bool static_shEnable = m_ShadowEnable;

		ImGuiWindowFlags flag = 0;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


		static bool is_open = true;

		ImGui::Begin("Scene", &is_open, flag);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


		ImGui::Checkbox("isStop", &m_TimeStop);

		ImGui::Separator();

		ImGui::Checkbox("Enable Shadow", &static_shEnable);
		m_ShadowEnable = static_shEnable;

		ImGui::Separator();


		if (ImGui::TreeNode("Ambient Occlusion"))
		{
			ImGui::Checkbox("Enable AO", &static_aoEnable);
			m_aoEnable = static_aoEnable;


			ImGui::SliderFloat("HemRedius", &m_aoHemRedius, 0.01f, 1.0f);
			ImGui::SliderFloat("Power", &m_aoPower, 0.0f, 100.0f);
			ImGui::TreePop();
		}

		
		ImGui::Separator();


		
		if (ImGui::TreeNode("GBuffer Render"))
		{
			m_GBufferRenderEnable = true;

			ImGui::RadioButton("Albedo", &static_render_radio, 0);
			ImGui::RadioButton("Normal", &static_render_radio, 1);
			ImGui::RadioButton("Position", &static_render_radio, 2);
			ImGui::RadioButton("Lighting", &static_render_radio, 3);
			ImGui::RadioButton("Motion", &static_render_radio, 4);
			ImGui::RadioButton("Depth", &static_render_radio, 5);
			ImGui::RadioButton("Roughness", &static_render_radio, 6);
			ImGui::RadioButton("Metallic", &static_render_radio, 7);
			ImGui::RadioButton("AO", &static_render_radio, 8);
			ImGui::RadioButton("ProjectionShadowMapping", &static_render_radio, 9);
			ImGui::RadioButton("Shadow", &static_render_radio, 10);
			m_DeferredType = static_render_radio;
			ImGui::TreePop();
		}
		else
		{
			m_GBufferRenderEnable = false;
		}


		ImGui::Text("Culling Mode");
		ImGui::RadioButton("None", &static_culling_radio, 0);
		ImGui::SameLine();
		ImGui::RadioButton("FrustumCulling", &static_culling_radio, 1);
		m_EnableCulling = static_culling_radio;


		ImGui::End();
	}

}
