/*---------------------------------------
*�@input.h
* �L�[�{�[�h�A�}�E�X�A�R���g���[���[�Ή�
*@author�FOkahara Taiki
----------------------------------------*/
#pragma once
#include "director.h"
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>

/* game pad��� */
#define BUTTON_UP		0x00000001l	// �����L�[��(.IY<0)
#define BUTTON_DOWN		0x00000002l	// �����L�[��(.IY>0)
#define BUTTON_LEFT		0x00000004l	// �����L�[��(.IX<0)
#define BUTTON_RIGHT	0x00000008l	// �����L�[�E(.IX>0)
#define BUTTON_A		0x00000010l	// �`�{�^��(.rgbButtons[0]&0x80)
#define BUTTON_B		0x00000020l	// �a�{�^��(.rgbButtons[1]&0x80)
#define BUTTON_C		0x00000040l	// �b�{�^��(.rgbButtons[2]&0x80)
#define BUTTON_X		0x00000080l	// �w�{�^��(.rgbButtons[3]&0x80)
#define BUTTON_Y		0x00000100l	// �x�{�^��(.rgbButtons[4]&0x80)
#define BUTTON_Z		0x00000200l	// �y�{�^��(.rgbButtons[5]&0x80)
#define BUTTON_L		0x00000400l	// �k�{�^��(.rgbButtons[6]&0x80)
#define BUTTON_R		0x00000800l	// �q�{�^��(.rgbButtons[7]&0x80)
#define BUTTON_START	0x00001000l	// �r�s�`�q�s�{�^��(.rgbButtons[8]&0x80)
#define BUTTON_M		0x00002000l	// �l�{�^��(.rgbButtons[9]&0x80)
#define GAMEPADMAX		4			// �����ɐڑ�����W���C�p�b�h�̍ő吔���Z�b�g


//�@�}�N��
#define	NUM_KEY_MAX			(256)
#define NUM_MOUSE_MAX		(3)	//�E�A���A�^�񒆂̂R��

// game pad�p�ݒ�l
#define DEADZONE		2500			// �e����25%�𖳌��]�[���Ƃ���
#define RANGE_MAX		1000			// �L���͈͂̍ő�l
#define RANGE_MIN		-1000			// �L���͈͂̍ŏ��l

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
		M_CLICK_LEFT,	//�@���N���b�N
		M_CLICK_RIGHT,	//�@�E�N���b�N
		M_CLICK_CENTER,	//�@�^�񒆃{�^���N���b�N

		M_DRAGDROP,		//�@�h���b�O�A���h�h���b�v
		M_CURDOR_SLIDE,	//�@�^�񒆃h���b�O�h���b�v
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

	//�L�[�{�[�h
	static LPDIRECTINPUTDEVICE8	g_pDevKeyboard;
	static BYTE					g_aKeyState[NUM_KEY_MAX];
	static BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];
	static BYTE					g_aKeyStateRelease[NUM_KEY_MAX];

	//�Q�[���p�b�h
	static LPDIRECTINPUTDEVICE8	g_pGamePad[GAMEPADMAX];// �p�b�h�f�o�C�X
	static DWORD				g_padState[GAMEPADMAX];	// �p�b�h���i�����Ή��j
	static DWORD				g_padTrigger[GAMEPADMAX];
	static int					g_padCount;			// ���o�����p�b�h�̐�

	//�}�E�X
	static LPDIRECTINPUTDEVICE8	g_pDIMouse;
	static BYTE					g_zdiMouseState[NUM_MOUSE_MAX];
	static BYTE					g_zdiMouseStateTrigger[NUM_MOUSE_MAX];
	static BYTE					g_zdiMouseStateRelease[NUM_MOUSE_MAX];
};
