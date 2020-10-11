/*「INPUT.cpp」=============================================
　・キーボード入力等のインプットクラス
　　製作者：岡原大起　	(-"-)
=============================================================*/
#include "input.h"

//
//
//　マクロ
#define	NUM_KEY_MAX			(256)

#define NUM_MOUSE_MAX		(3)	//右、左、真ん中の３つ

// game pad用設定値
#define DEADZONE		2500			// 各軸の25%を無効ゾーンとする
#define RANGE_MAX		1000			// 有効範囲の最大値
#define RANGE_MIN		-1000			// 有効範囲の最小値


static HWND m_phWnd = NULL;

static bool Keyboard_Initialize(HWND hWnd);
static void Keyboard_Finalize(void);
static void Keyboard_Update(void);

static bool GamePad_Initialize(HWND hWnd);
static void GamePad_Finalize(void);
static void GamePad_Update(void);

static bool Mouse_Initialize(HWND hWnd);
static void Mouse_Update();
static void Mouse_Finalize();

static LPDIRECTINPUT8			g_pInput = NULL;

//キーボード
static LPDIRECTINPUTDEVICE8	g_pDevKeyboard = NULL;
static BYTE					g_aKeyState[NUM_KEY_MAX];
static BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];
static BYTE					g_aKeyStateRelease[NUM_KEY_MAX];

//ゲームパッド
static LPDIRECTINPUTDEVICE8	g_pGamePad[GAMEPADMAX] = { NULL, NULL, NULL, NULL };// パッドデバイス
static DWORD				g_padState[GAMEPADMAX];	// パッド情報（複数対応）
static DWORD				g_padTrigger[GAMEPADMAX];
static int					g_padCount = 0;			// 検出したパッドの数

//マウス
static LPDIRECTINPUTDEVICE8	g_pDIMouse = NULL;
static BYTE					g_zdiMouseState[NUM_MOUSE_MAX];
//static DIMOUSESTATE			g_zdiMouseState_back;	// マウス情報(変化検知用)
static BYTE					g_zdiMouseStateTrigger[NUM_MOUSE_MAX];
static BYTE					g_zdiMouseStateRelease[NUM_MOUSE_MAX];


//static bool bState = false;//ドラッグドロップステート

//
//
//　コールバック
BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT result;

	result = g_pInput->CreateDevice(lpddi->guidInstance, &g_pGamePad[g_padCount++], NULL);
	return DIENUM_CONTINUE;	// 次のデバイスを列挙
}

//
//
//　初期化
bool Input_Initialize(HINSTANCE hInstance, HWND hWnd)
{
	m_phWnd = hWnd;
	if(g_pInput == NULL) {

		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL))) {

			return false;
		}
	}

	if (!Keyboard_Initialize(hWnd)) {
		return false;
	}
	if (!GamePad_Initialize(hWnd)) {
		return false;
	}
	if (!Mouse_Initialize(hWnd)) {
		return false;
	}

	return true;
}
//
//　
//　終了
void Input_Finalize(void)
{
	Keyboard_Finalize();
	GamePad_Finalize();
	Mouse_Finalize();
	if(g_pInput != NULL) {
		g_pInput->Release();
		g_pInput = NULL;
	}
}
//
//
//　更新
void Input_Update() 
{
	Keyboard_Update();
	GamePad_Update();
	Mouse_Update();
}
//=============================================================================

//
//
//　キーボード初期化
bool Keyboard_Initialize(HWND hWnd)
{

	if(FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		MessageBox(hWnd, "キーボードが那須！", "警告！", MB_ICONWARNING);
		return false;
	}

	if(FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定でき那須。", "警告！", MB_ICONWARNING);
		return false;
	}
	//													 DISCL_BACKGROUND | DISCL_EXCLUSIVE	
	if(FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定でき那須。", "警告！", MB_ICONWARNING);
		return false;
	}

	g_pDevKeyboard->Acquire();

	return true;
}
//
//
//　キーボード終了
void Keyboard_Finalize(void)
{
	if(g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
}
//
//
//　キーボード更新
void Keyboard_Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	if(SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for(int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];
			g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & g_aKeyState[nCnKey];

			g_aKeyState[nCnKey] = aKeyState[nCnKey];
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();
	}
}

//
//
//　キー入力
bool Keyboard_IsPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}
//
//
//　キー入力
bool Keyboard_IsTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true: false;
}
//
//
//　キー離す
bool Keyboard_IsRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true: false;
}

//===================================================================================

//
//
//　ゲームパッド初期化
bool GamePad_Initialize(HWND hWnd)
{

	HRESULT		result;
	int			i;

	g_padCount = 0;
	// ジョイパッドを探す
	g_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// セットしたコールバック関数が、パッドを発見した数だけ呼ばれる。

	for (i = 0; i<g_padCount; i++) 
	{
		// ジョイスティック用のデータ・フォーマットを設定
		result = g_pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result))
			return false; // データフォーマットの設定に失敗

		// モードを設定（フォアグラウンド＆非排他モード）
		//		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		//		if ( FAILED(result) )
		//			return false; // モードの設定に失敗

		// 軸の値の範囲を設定
		// X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
		// (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
		// 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドの性能による)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;
		// X軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_X;
		g_pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_Y;
		g_pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 各軸ごとに、無効のゾーン値を設定する。
		// 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
		// 指定する値は、10000に対する相対値(2000なら20パーセント)。
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		//X軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_X;
		g_pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//Y軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_Y;
		g_pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		//ジョイスティック入力制御開始
		g_pGamePad[i]->Acquire();
	}

	return true;
}
//
//
//　ゲームパッド終了
void GamePad_Finalize(void)
{
	for (int i = 0; i<GAMEPADMAX; i++) {
		if (g_pGamePad[i])
		{
			g_pGamePad[i]->Unacquire();
			g_pGamePad[i]->Release();
		}
	}
}
//
//
//　ゲームパッド更新
void GamePad_Update(void)
{
	HRESULT			result;
	DIJOYSTATE2		dijs;
	int				i;

	for (i = 0; i<g_padCount; i++)
	{
		DWORD lastPadState;
		lastPadState = g_padState[i];
		g_padState[i] = 0x00000000l;	// 初期化

		result = g_pGamePad[i]->Poll();	// ジョイスティックにポールをかける
		if (FAILED(result)) {
			result = g_pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = g_pGamePad[i]->Acquire();
		}

		result = g_pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// デバイス状態を読み取る
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			result = g_pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = g_pGamePad[i]->Acquire();
		}

		// ３２の各ビットに意味を持たせ、ボタン押下に応じてビットをオンにする
		//* y-axis (forward)
		if (dijs.lY < 0)					g_padState[i] |= BUTTON_UP;
		//* y-axis (backward)
		if (dijs.lY > 0)					g_padState[i] |= BUTTON_DOWN;
		//* x-axis (left)
		if (dijs.lX < 0)					g_padState[i] |= BUTTON_LEFT;
		//* x-axis (right)
		if (dijs.lX > 0)					g_padState[i] |= BUTTON_RIGHT;
		//* Ａボタン
		if (dijs.rgbButtons[0] & 0x80)	g_padState[i] |= BUTTON_A;
		//* Ｂボタン
		if (dijs.rgbButtons[1] & 0x80)	g_padState[i] |= BUTTON_B;
		//* Ｃボタン
		if (dijs.rgbButtons[2] & 0x80)	g_padState[i] |= BUTTON_C;
		//* Ｘボタン
		if (dijs.rgbButtons[3] & 0x80)	g_padState[i] |= BUTTON_X;
		//* Ｙボタン
		if (dijs.rgbButtons[4] & 0x80)	g_padState[i] |= BUTTON_Y;
		//* Ｚボタン
		if (dijs.rgbButtons[5] & 0x80)	g_padState[i] |= BUTTON_Z;
		//* Ｌボタン
		if (dijs.rgbButtons[6] & 0x80)	g_padState[i] |= BUTTON_L;
		//* Ｒボタン
		if (dijs.rgbButtons[7] & 0x80)	g_padState[i] |= BUTTON_R;
		//* ＳＴＡＲＴボタン
		if (dijs.rgbButtons[8] & 0x80)	g_padState[i] |= BUTTON_START;
		//* Ｍボタン
		if (dijs.rgbButtons[9] & 0x80)	g_padState[i] |= BUTTON_M;

		// Trigger設定
		g_padTrigger[i] = ((lastPadState ^ g_padState[i])	// 前回と違っていて
			& g_padState[i]);					// しかも今ONのやつ

	}
}
//
//
//　ゲームパッド入力（ずっと）
BOOL GamePad_IsPress(int padNo, DWORD button)
{
	return (button & g_padState[padNo]);
}
//
//
//　ゲームパッド入力（一度）
BOOL GamePad_IsTrigger(int padNo, DWORD button)
{
	return (button & g_padTrigger[padNo]);
}

//===================================================================================



bool Mouse_Initialize(HWND hWnd)
{
	HRESULT ret = S_FALSE;
	

	// マウス用にデバイスオブジェクトを作成
	ret = g_pInput->CreateDevice(GUID_SysMouse, &g_pDIMouse, NULL);
	if (FAILED(ret)) {
		// デバイスの作成に失敗
		return false;
	}

	// データフォーマットを設定
	ret = g_pDIMouse->SetDataFormat(&c_dfDIMouse);	// マウス用のデータ・フォーマットを設定
	if (FAILED(ret)) {
		// データフォーマットに失敗
		return false;
	}

	// モードを設定（フォアグラウンド＆非排他モード）
	ret = g_pDIMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(ret)) {
		// モードの設定に失敗
		return false;
	}

	// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	ret = g_pDIMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(ret)) {
		// デバイスの設定に失敗
		return false;
	}

	// 入力制御開始
	g_pDIMouse->Acquire();

	return true;
}
//
//
//
void Mouse_Finalize()
{
	// DirectInputのデバイスを開放
	if (g_pDIMouse) {
		g_pDIMouse->Release();
		g_pDIMouse = NULL;
	}
}
//
//
//
void Mouse_Update()
{
	DIMOUSESTATE zdiMouseState;

	if(SUCCEEDED(g_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &zdiMouseState)))
	{
	
		for (int nCnMouse = 0; nCnMouse < NUM_MOUSE_MAX ; nCnMouse++)
		{
			g_zdiMouseStateTrigger[nCnMouse] = (g_zdiMouseState[nCnMouse] ^ zdiMouseState.rgbButtons[nCnMouse]) & zdiMouseState.rgbButtons[nCnMouse];
			g_zdiMouseStateRelease[nCnMouse] = (g_zdiMouseState[nCnMouse] ^ zdiMouseState.rgbButtons[nCnMouse]) & g_zdiMouseState[nCnMouse];

			g_zdiMouseState[nCnMouse] = zdiMouseState.rgbButtons[nCnMouse];
		}
		
	}
	else
	{
		//cursor_direct[0] = 0;
		//cursor_direct[1] = 0;

		g_pDIMouse->Acquire();
	}
}
//
//
//
D3DXVECTOR2 Mouse_Cursor_Pos() {

	POINT poi;
	GetCursorPos(&poi);
	
	D3DXVECTOR2 pos;
	pos.x = (float)poi.x;
	pos.y = (float)poi.y;

	return pos;
}
//
//
//
bool Mouse_On_Window() {
	
	RECT w_rec;
	GetWindowRect(m_phWnd, &w_rec);

	POINT poi;
	GetCursorPos(&poi);
	if (w_rec.bottom > poi.y &&
		w_rec.top < poi.y &&
		w_rec.left < poi.x &&
		w_rec.right > poi.x) {
		
		return true;
	}
	else
	{
		return false;
	}
}


bool Mouse_Click(int nMouse) 
{
	return (g_zdiMouseState[nMouse] & 0x80) ? true : false;
}

bool Mouse_Trigger(int nMouse)
{
	return (g_zdiMouseStateTrigger[nMouse] & 0x80) ? true : false;
}

bool Mouse_Release(int nMouse)
{
	return (g_zdiMouseStateRelease[nMouse] & 0x80) ? true : false;
}

//　マウスの状態
bool GetMouseState(int state_num) {

	if (state_num == M_DRAGDROP){
		return (g_zdiMouseState[M_CLICK_LEFT] & 0x80) ? true : false;
	}
	else if(state_num == M_CURDOR_SLIDE){
		return (g_zdiMouseState[M_CLICK_CENTER] & 0x80) ? true : false;	
	}

	return (g_zdiMouseState[state_num] & 0x80) ? true : false;
}
//
//
D3DXVECTOR2 GetCurDirect(int nMouse) 
{
	//static D3DXVECTOR2	cursor_direct[2];	//x,yの二次元座標

	static D3DXVECTOR2 cursor_direct = D3DXVECTOR2(0, 0);
	static D3DXVECTOR2 cursor_start = D3DXVECTOR2(0, 0);
	static bool Once[NUM_MOUSE_MAX] = { false };

	if (Mouse_Click(nMouse))
	{
		if (Once[nMouse])
		{
			cursor_direct = Mouse_Cursor_Pos() - cursor_start;
		}
		else
		{
			cursor_start = Mouse_Cursor_Pos();
			Once[nMouse] = true;
		}
		
	}
	else if(Mouse_Release(nMouse))
	{
		cursor_direct = D3DXVECTOR2(0, 0);
		Once[nMouse] = false;
	}

	return cursor_direct;
}