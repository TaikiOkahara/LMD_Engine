#pragma once

#pragma warning (disable : 4005) //マクロの再定義警告の無視
#include "../05 Resource File/imgui/imgui_impl_dx11.h"
#include "../05 Resource File/imgui/imgui_impl_win32.h"


void IMGUI_Init(HWND);
void IMGUI_Uninit();
