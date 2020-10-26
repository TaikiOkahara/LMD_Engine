//#include "Scene.h"
#include "base.h"
#include "Game.h"
#include "Imgui11.h"
#include "input.h"

#include "camera.h"
#include "scenelight.h"
#include "player.h"
#include "floor.h"
#include "wall.h"
#include "stage.h"
#include "pillar.h"
#include "trim.h"
#include "ceiling.h"
#include "pointLight.h"

void Game::Init() {
	AddGameObject<CCamera>(LAYER::DRAW_LAYER_HIDE);
	AddGameObject<CSceneLight>(LAYER::DRAW_LAYER_HIDE);
	AddGameObject<CPointLight>(LAYER::DRAW_LAYER_LIGHT);//一番最後に描画


	AddGameObject<CWall>(LAYER::DRAW_LAYER_DRAW);	//壁
	AddGameObject<CPillar>(LAYER::DRAW_LAYER_DRAW);	//柱
	AddGameObject<CFloor>(LAYER::DRAW_LAYER_DRAW);	//地面
	AddGameObject<CStage>(LAYER::DRAW_LAYER_DRAW);	//ドア
	AddGameObject<CTrim>(LAYER::DRAW_LAYER_DRAW);	//つなぎ目
	AddGameObject<CCeiling>(LAYER::DRAW_LAYER_DRAW);//天井
	AddGameObject<CPlayer>(LAYER::DRAW_LAYER_DRAW);//プレイヤー
}

void Game::UnInit() {
	Scene::UnInit();
}

void Game::Update() {
	Scene::Update();
}

void Game::Imgui()
{
	// IMGUI　Frame start
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	
	Scene::Imgui();

	//Sceneのデバッグ
	static bool show_scene_window = true;

	if (Keyboard_IsTrigger(DIK_F1))
		show_scene_window = !show_scene_window;

	if (show_scene_window)
	{
		static float f1 = 0.0f;
		static int counter1 = 0;
		static int radio = 0;

		ImGuiWindowFlags flag = 0;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


		static bool is_open = true;

		ImGui::Begin("Scene", &is_open, flag);



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


	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	//ImGui::set
}
