//#include "Scene.h"
#include "base.h"
#include "Game.h"
#include "Imgui11.h"

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
	AddGameObject<CCamera>(0);
	AddGameObject<CSceneLight>(0);
	AddGameObject<CPointLight>(2);


	AddGameObject<CWall>(1);	//壁
	AddGameObject<CPillar>(1);	//柱
	AddGameObject<CFloor>(1);	//地面
	AddGameObject<CStage>(1);	//ドア
	AddGameObject<CTrim>(1);	//つなぎ目
	AddGameObject<CCeiling>(1);
	AddGameObject<CPlayer>(1);
}

void Game::UnInit() {
	Scene::UnInit();
}

void Game::Update() {
	Scene::Update();
}

#ifdef DEBUG


void Game::Imgui_In()
{

	static bool show_demo_window = false;
	static bool show_main_window = true;
	static bool show_another_window = true;
	static bool show_camera_winow = true;
	static bool show_light_winow = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// IMGUI　Frame start
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	if (show_camera_winow)
	{
		ImGuiWindowFlags cw_flag = 0;
		static bool cw_is_open;

		ImGui::Begin("CameraController", &cw_is_open, cw_flag);

		ImGui::Text(u8"\n***Controller(日本語)***");

		//ImGui::Text("\nViewpoint turning :[Left  Arrow]");
		//ImGui::Text("Viewpoint turning :[Right Arrow]");
		//ImGui::Text("Viewpoint    rise       [Y]");
		//ImGui::Text("Viewpoint    descent    [N]");
		//ImGui::Text("Gazing Point descent    [T]");
		//ImGui::Text("Gazing Point descent    [B]");
		//ImGui::Text("Zoom In                 [U]");
		//ImGui::Text("Zome Out                [M]");


		//D3DXVECTOR3 PosV = m_Camera.GetCameraPos();
		//ImGui::InputFloat3("Eye", PosV, "%.2f");
		//D3DXVECTOR3 PosR = m_Camera.GetCameraData().Lookat;
		//ImGui::InputFloat3("Lookat", PosR, "%.2f");
		//D3DXVECTOR3 VecU = m_Camera.GetCameraData().Up;
		//ImGui::InputFloat3("Up", VecU, "%.2f");
		//ImGui::InputFloat("Distance", &m_pCamera->fDistance);

		ImGui::End();
	}


	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	if (show_main_window)
	{
		static float f1 = 0.0f;
		static int counter1 = 0;
		static int radio = 0;

		ImGuiWindowFlags flag = 0;

		//flag |= ImGuiWindowFlags_NoTitleBar;			// タイトルバーを非表示にします。
		//flag |= ImGuiWindowFlags_NoResize;			// ウィンドウをリサイズ不可にします。
		//flag |= ImGuiWindowFlags_NoMove;				// ウィンドウを移動不可にします。
		//flag |= ImGuiWindowFlags_NoScrollbar;			// スクロールバーを無効にします。
		//flag |= ImGuiWindowFlags_NoScrollWithMouse;	// マウスホイールでのスクロール操作を無効にします。
		//flag |= ImGuiWindowFlags_NoCollapse;			// タイトルバーをダブルクリックして閉じる挙動を無効にします。
		//flag |= ImGuiWindowFlags_NoBackground;		// ウィンドウ内の背景を非表示にします。
		//flag |= ImGuiWindowFlags_NoBringToFrontOnFocus;// ウィンドウをクリックしてフォーカスした際に他のウィンドウよりも前面に表示する挙動を無効にします。
		//flag |= ImGuiWindowFlags_NoNav;				// ゲームパッドやキーボードでのUIの操作を無効にします。
		//flag |= ImGuiWindowFlags_NoSavedSettings;		// imgui.iniでウィンドウの位置などを自動保存/ロードさせないようにします。
		//flag |= ImGuiWindowFlags_AlwaysAutoResize;	// 自動でウィンドウ内のコンテンツに合わせてリサイズします。
		//flag |= ImGuiWindowFlags_NoFocusOnAppearing;	// 表示/非表示の際のトランジションアニメーションを無効にします。

		static bool is_open = true;

		ImGui::Begin("DirectionalLight", &is_open, flag);

		//　シェーディングタイプ	

		/*
		if (ImGui::TreeNode("Ambient")) {

			ImGui::SliderFloat("r", &m_pField[0].Ambient.x, 0.0f, 1.0f);
			ImGui::SliderFloat("g", &m_pField[0].Ambient.y, 0.0f, 1.0f);
			ImGui::SliderFloat("b", &m_pField[0].Ambient.z, 0.0f, 1.0f);
			ImGui::SliderFloat("?", &m_pField[0].Ambient.w, 0.0f, 1.0f);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Diffuse")) {

			ImGui::SliderFloat("r", &m_pField[0].Diffuse.x, 0.0f, 1.0f);
			ImGui::SliderFloat("g", &m_pField[0].Diffuse.y, 0.0f, 1.0f);
			ImGui::SliderFloat("b", &m_pField[0].Diffuse.z, 0.0f, 1.0f);
			ImGui::SliderFloat("?", &m_pField[0].Diffuse.w, 0.0f, 1.0f);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Specular")) {

			ImGui::SliderFloat("r", &m_pField[0].Specular.x, 0.0f, 1.0f);
			ImGui::SliderFloat("g", &m_pField[0].Specular.y, 0.0f, 1.0f);
			ImGui::SliderFloat("b", &m_pField[0].Specular.z, 0.0f, 1.0f);
			ImGui::SliderFloat("?", &m_pField[0].Specular.w, 0.0f, 1.0f);

			ImGui::TreePop();
		}
		*/

		//ImGui::SliderFloat2("CursorPoint",(float)poi.x,(float)poi.y);               // Display some text (you can use a format strings too)

		CSceneLight* light = Base::GetScene()->GetGameObject<CSceneLight>(0);

		ImGui::InputFloat3("LightDirection", light->GetRotation(),1);


		ImGui::Checkbox("Open/Close", &is_open);      // チェックボックスがクリックされるとis_openが反転し、trueならチェックマークが表示されます。
		ImGui::Checkbox("Trigger", &show_another_window);

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

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	/*{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("create object.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Use Gravity", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Trigger", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

	}*/

	//3. Show another simple window.
	/*if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		//ImGui::Text("Hello from another window!");

		if (Mouse_On_Window()) {

			ImGui::InputFloat2("CursorPoint", Mouse_Cursor_Pos(), "%.2f");
			ImGui::InputFloat2("CursorDirection", GetCurDirect(M_CLICK_LEFT), "%.2f");



			if (Mouse_Click(M_CLICK_LEFT))
			{
				ImGui::Text("OnClickLeft!!");
			}
			if (Mouse_Click(M_CLICK_RIGHT))
			{
				ImGui::Text("OnClickRight!!");
			}
			if (Mouse_Click(M_CLICK_CENTER))
			{
				ImGui::Text("OnClickCenter!!");
			}



		}


		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}*/

	if (show_light_winow)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("CameraController", &lw_is_open, lw_flag);

		ImGui::Text(u8"ライトだよ！");

		//D3DXVECTOR4 rot = m_Light.GetLightData().directionalData.LightDir;
		//ImGui::Text("RotationX:%.3f",rot.x);
		//ImGui::Text("RotationY:%.3f",rot.y);
		//ImGui::Text("RotationZ:%.3f",rot.z);
		//ImGui::Text("Intensity:%.3f", m_Light.GetLightData().directionalData.Intensity.x);

		ImGui::End();
	}

	ImGui::EndFrame();

	//D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*255.0f), (int)(clear_color.y*255.0f), (int)(clear_color.z*255.0f), (int)(clear_color.w*255.0f));
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	//ImGui::set



}

#endif // DEBUG