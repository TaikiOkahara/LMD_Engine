#include "window.h"
#include "renderer.h"
#include "Imgui11.h"

void IMGUI_Init(HWND m_hWnd)
{
	// IMGUI�@�Z�b�g�A�b�v
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	//�@�v���b�g�t�H�[���@�Z�b�g�A�b�v�@Renderer �\�z
	ImGui_ImplWin32_Init(m_hWnd);
	ImGui_ImplDX11_Init(RENDERER::m_pDevice, RENDERER::m_pDeviceContext);
	//IMGUI �@�X�^�C���@�Z�b�g�A�b�v
	ImGui::StyleColorsDark();

	// Setup back-end capabilities flags
	ImGuiIO& io = ImGui::GetIO();
	io.BackendRendererName = "imgui_impl_dx11";
	io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;  // We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\meiryo.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	
	// Enable docking(available in imgui `docking` branch at the moment)
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;//�h�b�L���O�@�\�ǉ�
}

void IMGUI_Uninit()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}