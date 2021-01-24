/*---------------------------------------
*�@game.h
*
* ���C���ƂȂ�V�[��
*@author�FOkahara Taiki
----------------------------------------*/
#include "base.h"
#include "game.h"
#include "title.h"
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
#include "edge.h"
#include "pots.h"
#include "trim.h"
#include "gargoyle.h"
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
	AddGameObject<CPointLight>(LAYER::LIGHT);//��ԍŌ�ɕ`��

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


	AddGameObject<CProjShadow>(LAYER::DRAW);

	AddGameObject<CPlayer>(LAYER::DRAW);//�v���C���[


	AddPostProcess<CDirectionalLight>();
	AddPostProcess<CFogEffect>();
	
}

void Game::UnInit() {
	
	CScene::UnInit();


	CInstanceGameObject::Unload();
	CGameObject::Unload();
}

void Game::Update() {

	if (timeStop) return;
	CScene::Update();

	//�|�X�g�v���Z�X�����V�F�[�_�[�ɃZ�b�g
	RENDERER::GetConstantList().GetStruct<EffectBuffer>()->SetDeferredParam(D3DXVECTOR3(deferredType * 1.0f, 0, 0), gBufferRenderEnable);
	RENDERER::GetConstantList().GetStruct<EffectBuffer>()->Set();
	
	RENDERER::GetConstantList().GetStruct<EffectBuffer>()->SetAO(D3DXVECTOR4(ambientOcclusionPower,0,0,0));


	RENDERER::GetConstantList().GetStruct<ToggleBuffer>()->SetFrustumCullingEnable(frustumEnable);
	RENDERER::GetConstantList().GetStruct<ToggleBuffer>()->Set();


	//�^�C�g��Scene�ړ�
	if (CInput::KeyTrigger(DIK_F8))
		Base::SetScene<Title>();
}

void Game::Imgui()
{
	

	
	CScene::Imgui();

	//Scene�̃f�o�b�O
	static bool show_scene_window = true;

	if (CInput::KeyTrigger(DIK_F2))
		show_scene_window = !show_scene_window;

	if (show_scene_window)
	{
		static float f1 = 0.0f;
		static int counter1 = 0;
		
		static int render_radio = deferredType;
		static int culling_radio = culling_radio;
		static bool gBuffer_render = gBufferRenderEnable;

		ImGuiWindowFlags flag = 0;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


		static bool is_open = true;

		ImGui::Begin("Scene", &is_open, flag);


		ImGui::Checkbox("isStop", &timeStop);

		
		ImGui::Checkbox("GBuffer Render", &gBuffer_render);
		gBufferRenderEnable = gBuffer_render;

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

		//if (ImGui::Button("Button"))	// "Button"����������true�ɂȂ�
		//	counter1++;


		/*
			"##1" �� "##2" �̂悤�ȕ���������邱�ƂŌ݂�����ʂł���悤�ɂȂ�A�`�F�b�N�{�b�N�X���N���b�N���Ă��ʂ̃E�B���h�E�̊J���ł���悤�ɂȂ�܂��B
			"##"�ȍ~�̕�����͕`�悳��܂���B"Open/Close" �ƕ`�悳��܂��B"##"���g���̂͂����܂ŃR�[�h���ŋ�ʂ��邽�߂̍�@�ł��B
			"##"�ȍ~�̕������1��2�łȂ��Ă��\���܂���B

		*/

		//ImGui::SameLine(); // ���ɏ���UI�p�[�c�����݂Ɠ����s�ɔz�u���܂��B

		//ImGui::Text("counter = %d", counter1);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

}
