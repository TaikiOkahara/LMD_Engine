/*「INPUT.h」=============================================
　・キーボード入力等のインプットクラス
　　製作者：岡原大起　	(-"-)
=============================================================*/
#pragma once
//#include "renderer.h"
#include "director.h"
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>

/* game pad情報 */
#define BUTTON_UP		0x00000001l	// 方向キー上(.IY<0)
#define BUTTON_DOWN		0x00000002l	// 方向キー下(.IY>0)
#define BUTTON_LEFT		0x00000004l	// 方向キー左(.IX<0)
#define BUTTON_RIGHT	0x00000008l	// 方向キー右(.IX>0)
#define BUTTON_A		0x00000010l	// Ａボタン(.rgbButtons[0]&0x80)
#define BUTTON_B		0x00000020l	// Ｂボタン(.rgbButtons[1]&0x80)
#define BUTTON_C		0x00000040l	// Ｃボタン(.rgbButtons[2]&0x80)
#define BUTTON_X		0x00000080l	// Ｘボタン(.rgbButtons[3]&0x80)
#define BUTTON_Y		0x00000100l	// Ｙボタン(.rgbButtons[4]&0x80)
#define BUTTON_Z		0x00000200l	// Ｚボタン(.rgbButtons[5]&0x80)
#define BUTTON_L		0x00000400l	// Ｌボタン(.rgbButtons[6]&0x80)
#define BUTTON_R		0x00000800l	// Ｒボタン(.rgbButtons[7]&0x80)
#define BUTTON_START	0x00001000l	// ＳＴＡＲＴボタン(.rgbButtons[8]&0x80)
#define BUTTON_M		0x00002000l	// Ｍボタン(.rgbButtons[9]&0x80)
#define GAMEPADMAX		4			// 同時に接続するジョイパッドの最大数をセット

bool Input_Initialize(HINSTANCE hInstance, HWND hWnd);
void Input_Finalize(void);
void Input_Update();

//
//
//　キーボードプロトタイプ
bool Keyboard_IsPress(int nKey);
bool Keyboard_IsTrigger(int nKey);
bool Keyboard_IsRelease(int nKey);

//
//
//　ゲームパッドプロトタイプ
BOOL GamePad_IsPress(int padNo, DWORD button);
BOOL GamePad_IsTrigger(int padNo, DWORD button);

//
//
//
enum MouseState
{
	M_CLICK_LEFT,	//　左クリック
	M_CLICK_RIGHT,	//　右クリック
	M_CLICK_CENTER,	//　真ん中ボタンクリック
	
	M_DRAGDROP,		//　ドラッグアンドドロップ
	M_CURDOR_SLIDE,	//　真ん中ドラッグドロップ
};
//
//
//
D3DXVECTOR2 Mouse_Cursor_Pos();
bool Mouse_On_Window();
//bool Mouse_Click_Left();
//bool Mouse_Click_Right();
//bool Mouse_Click_Center();

bool Mouse_Click(int nMouse);
bool Mouse_Trigger(int nMouse);
bool Mouse_Release(int nMouse);

bool GetMouseState(int state_num);
//bool GetMouseClick(int click_num);

D3DXVECTOR2 GetCurDirect(int nMouse);