#include "base.h"
#include "game.h"
#include "Imgui11.h"
#include "input.h"

#include "camera.h"
#include "pointLight.h"

#include "player.h"
#include "floor.h"
#include "wall.h"
#include "stage.h"
#include "pillar.h"
#include "pots.h"
#include "trim.h"
#include "ceiling.h"
#include "ceilingArch.h"
#include "chandelier.h"
#include "cubeMap.h"
#include "projectionShadow.h"

void Game::Init() {

	CGameObject::Load();
	CInstanceGameObject::Load();



	AddGameObject<CCamera>(LAYER::HIDE);
	AddGameObject<CPointLight>(LAYER::LIGHT);//一番最後に描画

	AddGameObject<CCubeMap>(LAYER::HIDE);


	AddGameObject<CFloor>(LAYER::DRAW);
	AddGameObject<CWall>(LAYER::DRAW);
	AddGameObject<CPillar>(LAYER::DRAW);
	//AddGameObject<CStage>(LAYER::DRAW_LAYER_DRAW);	//ドア
	AddGameObject<CTrim>(LAYER::DRAW);
	AddGameObject<CCeiling>(LAYER::DRAW);
	AddGameObject<CCeilingArch>(LAYER::DRAW);
	AddGameObject<CPots>(LAYER::DRAW);
	AddGameObject<CChandelier>(LAYER::DRAW);


	AddGameObject<CProjShadow>(LAYER::DRAW);

	AddGameObject<CPlayer>(LAYER::DRAW);//プレイヤー


}

void Game::UnInit() {
	
	CScene::UnInit();

	CInstanceGameObject::Unload();
	CGameObject::Unload();
}

void Game::Update() {

	if (timeStop) return;
	CScene::Update();

	RENDERER::m_ConstantBufferList.GetStruct<EffectBuffer>()->SetDeferredParam(D3DXVECTOR4(deferredType * 1.0f, 0, 0, 0));
	RENDERER::m_ConstantBufferList.GetStruct<EffectBuffer>()->SetAO(D3DXVECTOR4(ambientOcclusionPower,0,0,0));


	RENDERER::m_ConstantBufferList.GetStruct<ToggleBuffer>()->SetFrustumCullingEnable(frustumEnable);
	RENDERER::m_ConstantBufferList.GetStruct<ToggleBuffer>()->Set();

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
		static bool gBuffer_render = false;
		static int render_radio = deferredType;
		static int culling_radio = culling_radio;

		ImGuiWindowFlags flag = 0;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


		static bool is_open = true;

		ImGui::Begin("Scene", &is_open, flag);


		ImGui::Checkbox("isStop", &timeStop);

		//ImGui::Checkbox("DirectionalLight", &RENDERER::toggleDirectional);
		//ImGui::Checkbox("PointLight", &RENDERER::togglePoint);
		//ImGui::Checkbox("Trigger", &show_another_window);

		//ImGui::SliderFloat("float", &f1, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
		ImGui::Checkbox("GBuffer Render", &gBuffer_render);

		if (gBuffer_render)
		{
			ImGui::Text("Render Type");
			ImGui::RadioButton("Color", &render_radio, 0);
			ImGui::RadioButton("Normal", &render_radio, 1);
			ImGui::RadioButton("Position", &render_radio, 2);
			ImGui::RadioButton("Lighting(tmp)", &render_radio, 3);
			ImGui::RadioButton("Motion", &render_radio, 4);
			ImGui::RadioButton("Depth", &render_radio, 5);
			ImGui::RadioButton("Roughness", &render_radio, 6);
			ImGui::RadioButton("Metallic", &render_radio, 7);
			ImGui::RadioButton("AmbientOcclusion", &render_radio, 8);
			ImGui::RadioButton("ProjectionShadowMapping", &render_radio, 9);
			ImGui::RadioButton("Shadow", &render_radio, 10);
			deferredType = render_radio;
		}
		
		ImGui::SliderFloat("Ambient Occlusion", &ambientOcclusionPower, 0.0f, 1.0f);

		ImGui::Text("Culling Mode");
		ImGui::RadioButton("None", &culling_radio, 0);
		ImGui::SameLine();
		ImGui::RadioButton("FrustumCulling", &culling_radio, 1);
		frustumEnable =  culling_radio;

		//if (ImGui::Button("Button"))	// "Button"が押されるとtrueになる
		//	counter1++;


		/*
			"##1" や "##2" のような文字列を入れることで互いを区別できるようになり、チェックボックスをクリックしても別のウィンドウの開閉ができるようになります。
			"##"以降の文字列は描画されません。"Open/Close" と描画されます。"##"を使うのはあくまでコード内で区別するための作法です。
			"##"以降の文字列は1や2でなくても構いません。

		*/

		//ImGui::SameLine(); // 次に書くUIパーツを現在と同じ行に配置します。

		//ImGui::Text("counter = %d", counter1);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}


	

}
