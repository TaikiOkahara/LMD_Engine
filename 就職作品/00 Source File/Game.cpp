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
	AddGameObject<CPointLight>(LAYER::DRAW_LAYER_LIGHT);//��ԍŌ�ɕ`��


	AddGameObject<CWall>(LAYER::DRAW_LAYER_DRAW);	//��
	AddGameObject<CPillar>(LAYER::DRAW_LAYER_DRAW);	//��
	AddGameObject<CFloor>(LAYER::DRAW_LAYER_DRAW);	//�n��
	AddGameObject<CStage>(LAYER::DRAW_LAYER_DRAW);	//�h�A
	AddGameObject<CTrim>(LAYER::DRAW_LAYER_DRAW);	//�Ȃ���
	AddGameObject<CCeiling>(LAYER::DRAW_LAYER_DRAW);//�V��
	AddGameObject<CPlayer>(LAYER::DRAW_LAYER_DRAW);//�v���C���[
}

void Game::UnInit() {
	Scene::UnInit();
}

void Game::Update() {
	Scene::Update();
}

void Game::Imgui()
{
	// IMGUI�@Frame start
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	
	Scene::Imgui();

	//Scene�̃f�o�b�O
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



		ImGui::Checkbox("Open/Close", &is_open);      // �`�F�b�N�{�b�N�X���N���b�N������is_open�����]���Atrue�Ȃ�`�F�b�N�}�[�N���\������܂��B
		//ImGui::Checkbox("Trigger", &show_another_window);

		ImGui::SliderFloat("float", &f1, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		ImGui::RadioButton("RadioButton 0", &radio, 0); ImGui::SameLine();
		ImGui::RadioButton("RadioButton 1", &radio, 1); ImGui::SameLine();
		ImGui::RadioButton("RadioButton 2", &radio, 2);

		if (ImGui::Button("Button"))	// "Button"����������true�ɂȂ�
			counter1++;


		/*
			"##1" �� "##2" �̂悤�ȕ���������邱�ƂŌ݂�����ʂł���悤�ɂȂ�A�`�F�b�N�{�b�N�X���N���b�N���Ă��ʂ̃E�B���h�E�̊J���ł���悤�ɂȂ�܂��B
			"##"�ȍ~�̕�����͕`�悳��܂���B"Open/Close" �ƕ`�悳��܂��B"##"���g���̂͂����܂ŃR�[�h���ŋ�ʂ��邽�߂̍�@�ł��B
			"##"�ȍ~�̕������1��2�łȂ��Ă��\���܂���B

		*/

		ImGui::SameLine(); // ���ɏ���UI�p�[�c�����݂Ɠ����s�ɔz�u���܂��B

		ImGui::Text("counter = %d", counter1);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}


	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	//ImGui::set
}
