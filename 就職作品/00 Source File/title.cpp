#include "base.h"
#include"game.h"
#include "title.h"
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
#include "directionalLight.h"

void Title::Init() {

	CGameObject::Load();
	CInstanceGameObject::Load();


	AddGameObject<CCamera>(LAYER::HIDE);

	AddGameObject<CCubeMap>(LAYER::HIDE);


	AddGameObject<CFloor>(LAYER::DRAW);


	AddGameObject<CPlayer>(LAYER::DRAW);//プレイヤー



	AddPostProcess<CDirectionalLight>();
}

void Title::UnInit() {
	
	CScene::UnInit();


	CInstanceGameObject::Unload();
	CGameObject::Unload();

}

void Title::Update() {

	CScene::Update();

	//タイトルScene移動
	if (CInput::KeyTrigger(DIK_F8))
		Base::SetScene<Game>();
}

void Title::Imgui()
{

	CScene::Imgui();



	if (true)
	{
		ImGuiWindowFlags flag = 0;


		static bool is_open = true;

		ImGui::Begin("TitleScene", &is_open, flag);

		//ImGui::Text("Render Type");
		
		ImGui::End();
	}
}
