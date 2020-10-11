/*�uINPUT.h�v=============================================
�@�E�L�[�{�[�h���͓��̃C���v�b�g�N���X
�@�@����ҁF������N�@	(-"-)
=============================================================*/
#pragma once
//#include "renderer.h"
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

bool Input_Initialize(HINSTANCE hInstance, HWND hWnd);
void Input_Finalize(void);
void Input_Update();

//
//
//�@�L�[�{�[�h�v���g�^�C�v
bool Keyboard_IsPress(int nKey);
bool Keyboard_IsTrigger(int nKey);
bool Keyboard_IsRelease(int nKey);

//
//
//�@�Q�[���p�b�h�v���g�^�C�v
BOOL GamePad_IsPress(int padNo, DWORD button);
BOOL GamePad_IsTrigger(int padNo, DWORD button);

//
//
//
enum MouseState
{
	M_CLICK_LEFT,	//�@���N���b�N
	M_CLICK_RIGHT,	//�@�E�N���b�N
	M_CLICK_CENTER,	//�@�^�񒆃{�^���N���b�N
	
	M_DRAGDROP,		//�@�h���b�O�A���h�h���b�v
	M_CURDOR_SLIDE,	//�@�^�񒆃h���b�O�h���b�v
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