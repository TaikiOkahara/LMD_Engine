/*---------------------------------------
*　input.h
* キーボード、マウス、コントローラー対応
*@author：Okahara Taiki
----------------------------------------*/
#pragma once
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


//　マクロ
#define	NUM_KEY_MAX			(256)
#define NUM_MOUSE_MAX		(3)	//右、左、真ん中の３つ

// game pad用設定値
#define DEADZONE		2500			// 各軸の25%を無効ゾーンとする
#define RANGE_MAX		1000			// 有効範囲の最大値
#define RANGE_MIN		-1000			// 有効範囲の最小値

class CInput
{
public:
	

	static bool Init(HINSTANCE hInstance, HWND hWnd);
	static void Uninit();
	static void Update();
	

	static bool KeyPress(int nKey);
	static bool KeyTrigger(int nKey);
	static bool KeyRelease(int nKey);


	static BOOL PadPress(int padNo, DWORD button);
	static BOOL PadTrigger(int padNo, DWORD button);

private:

	enum MouseState
	{
		M_CLICK_LEFT,	//　左クリック
		M_CLICK_RIGHT,	//　右クリック
		M_CLICK_CENTER,	//　真ん中ボタンクリック

		M_DRAGDROP,		//　ドラッグアンドドロップ
		M_CURDOR_SLIDE,	//　真ん中ドラッグドロップ
	};


	static bool Keyboard_Initialize(HWND hWnd);
	static void Keyboard_Finalize();
	static void Keyboard_Update();

	static bool GamePad_Initialize(HWND hWnd);
	static void GamePad_Finalize(void);
	static void GamePad_Update(void);
	
	static bool Mouse_Initialize(HWND hWnd);
	static void Mouse_Update();
	static void Mouse_Finalize();


	static D3DXVECTOR2 Mouse_Cursor_Pos();
	static bool Mouse_On_Window();
	
	static bool Mouse_Click(int nMouse);
	static bool Mouse_Trigger(int nMouse);
	static bool Mouse_Release(int nMouse);

	static bool GetMouseState(int state_num);

	static D3DXVECTOR2 GetCurDirect(int nMouse);
	
	static BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);
	
	static HWND m_phWnd;

	static LPDIRECTINPUT8			g_pInput;

	//キーボード
	static LPDIRECTINPUTDEVICE8	g_pDevKeyboard;
	static BYTE					g_aKeyState[NUM_KEY_MAX];
	static BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];
	static BYTE					g_aKeyStateRelease[NUM_KEY_MAX];

	//ゲームパッド
	static LPDIRECTINPUTDEVICE8	g_pGamePad[GAMEPADMAX];// パッドデバイス
	static DWORD				g_padState[GAMEPADMAX];	// パッド情報（複数対応）
	static DWORD				g_padTrigger[GAMEPADMAX];
	static int					g_padCount;			// 検出したパッドの数

	//マウス
	static LPDIRECTINPUTDEVICE8	g_pDIMouse;
	static BYTE					g_zdiMouseState[NUM_MOUSE_MAX];
	static BYTE					g_zdiMouseStateTrigger[NUM_MOUSE_MAX];
	static BYTE					g_zdiMouseStateRelease[NUM_MOUSE_MAX];
};
