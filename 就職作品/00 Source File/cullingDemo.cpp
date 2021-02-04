/*---------------------------------------
*　cullingDemo.h
*
* カリング効果を確認するシーン
*@author：Okahara Taiki
----------------------------------------*/
#include "base.h"
#include "cullingDemo.h"
#include "game.h"
#include "Imgui11.h"
#include "input.h"

#include "camera.h"
#include "pointLight.h"

//#include "table.h"
#include "player.h"
#include "floor.h"
//#include "wall.h"
//#include "doorWay.h"
//#include "pillar.h"
//#include "halfArch.h"
//#include "edge.h"
//#include "pots.h"
//#include "trim.h"
#include "gargoyle.h"
//#include "stairs.h"
//#include "ceiling.h"
//#include "ceilingArch.h"
//#include "chandelier.h"
#include "cubeMap.h"
//#include "projectionShadow.h"

#include "directionalLight.h"
#include "fog.h"

void CullingDemo::Init() {

	CGameObject::Load();
	CInstanceGameObject::Load();



	AddGameObject<CCamera>(LAYER::HIDE);
	AddGameObject<CPointLight>(LAYER::LIGHT);//一番最後に描画

	AddGameObject<CCubeMap>(LAYER::HIDE);

	//AddGameObject<CTable>(LAYER::DRAW);
	AddGameObject<CFloor>(LAYER::DRAW);
	//AddGameObject<CWall>(LAYER::DRAW);
	//AddGameObject<CPillar>(LAYER::DRAW);
	//AddGameObject<CEdge>(LAYER::DRAW);
	//AddGameObject<CDoorWay>(LAYER::DRAW);

	AddGameObject<CGargoyle>(LAYER::DRAW);
	/*AddGameObject<CTrim>(LAYER::DRAW);
	AddGameObject<CCeiling>(LAYER::DRAW);
	AddGameObject<CCeilingArch>(LAYER::DRAW);
	AddGameObject<CPots>(LAYER::DRAW);
	AddGameObject<CChandelier>(LAYER::DRAW);
	AddGameObject<CStairs>(LAYER::DRAW);
	AddGameObject<CHalfArch>(LAYER::DRAW);

	AddGameObject<CProjShadow>(LAYER::DRAW);*/

	AddGameObject<CPlayer>(LAYER::DRAW);//プレイヤー


	AddPostProcess<CDirectionalLight>();
	AddPostProcess<CFogEffect>();
	
}

void CullingDemo::UnInit() {
	
	CScene::UnInit();


	CInstanceGameObject::Unload();
	CGameObject::Unload();
}

void CullingDemo::Update() {

	if (timeStop) return;
	CScene::Update();

	////ポストプロセス情報をシェーダーにセット
	//RENDERER::GetConstantList().GetStruct<EffectBuffer>()->SetDeferredParam(D3DXVECTOR3(deferredType * 1.0f, 0, 0), gBufferRenderEnable);
	//RENDERER::GetConstantList().GetStruct<EffectBuffer>()->Set();
	//
	//RENDERER::GetConstantList().GetStruct<EffectBuffer>()->SetAO(D3DXVECTOR4(ambientOcclusionPower,0,0,0));


	RENDERER::GetConstantList().GetStruct<ToggleBuffer>()->SetFrustumCullingEnable(frustumEnable);
	RENDERER::GetConstantList().GetStruct<ToggleBuffer>()->Set();


	//タイトルScene移動
	if (CInput::KeyTrigger(DIK_F8))
		Base::SetScene<Game>();
}

void CullingDemo::Imgui()
{
	

	//シーンのデバッグは表示させない
	//CScene::Imgui();

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


		ImGui::Checkbox("isStop", &timeStop);

		
	
		

		ImGui::Text("Culling Mode");
		ImGui::RadioButton("None", &culling_radio, 0);
		ImGui::SameLine();
		ImGui::RadioButton("FrustumCulling", &culling_radio, 1);
		frustumEnable =  culling_radio;

	

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

}
