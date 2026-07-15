///<summary>
///キー入力クラス
///</summary>
#pragma once

//ヘッダーファイルのインクルード
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
//必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

#include "windows.h"
#include "CVector.h"

#define PAD_MAX 5

class CInput {
public:
	enum E_STATE {
		ePush,		//押した瞬間
		eHold,		//押している
		ePull,		//離した瞬間
		eFree,		//離している
		eStateMax,
	};

	enum E_BUTTON {
		eButton1 = 0,
		eButton2,
		eButton3,
		eButton4,
		eButton5,
		eButton6,
		eButton7,
		eButton8,
		eButton9,
		eButton10,
		eButton11,
		eButton12,
		eButton13,
		eNum1,
		eNum2,
		eNum3,
		eNum4,
		eNum5,
		eMouseL,
		eMouseR,
		eMouseC,
		eUp,
		eDown,
		eLeft,
		eRight,
		eDebugCamera1,		//デバッグカメラで使用するボタン１
		eDebugCamera2,		//デバッグカメラで使用するボタン2
		eNavDebug,
		eKeyMax = 31
	};
	static unsigned char	m_key_code[PAD_MAX][eKeyMax];
	static unsigned char	m_pad_code[PAD_MAX][16];
	static unsigned long m_key_old[PAD_MAX];
	static unsigned long m_key_state[PAD_MAX][eStateMax];
	static CVector2D m_mouse_vec;
	static int		m_mouse_wheel;
	static int		m_mouse_wheelOld;
	static bool		m_mouse_inside;

	static LPDIRECTINPUT8 m_pDinput;
	static LPDIRECTINPUTDEVICE8 m_pMouse;
	struct SPadDevice {
		LPDIRECTINPUTDEVICE8 m_pPadDevice;
		DIJOYSTATE2 js;
		int no;
	};

	static SPadDevice m_device[PAD_MAX];
	static bool CALLBACK _padCallback(const LPDIDEVICEINSTANCE lpddi, LPVOID pvRef);
	static bool m_debug_mode;
public:
	static bool Init();
	static void ClearInstance();

	///<summary>
	///キーの状態を検査
	///</summary>
	///<param name="no">コントローラー番号</param>
	///<param name="state">状態</param>
	///<param name="button">ボタン</param>
	///<param name="debug">デバッグモード中でも取得できるかどうか</param>
	///<returns>true:指定した状態　false:指定した状態ではない</returns>
	static bool GetState(int no, CInput::E_STATE state, CInput::E_BUTTON button, bool debug = false)
	{
		//デバッグモード中は通常の取得処理では、falseしか返さない
		if (!debug && m_debug_mode)return false;
		return (m_key_state[no][state] & (1 << button)) > 0;
	}
	static long unsigned GetState(int no, CInput::E_STATE state) {
		return m_key_state[no][state];
	}
	///<summary>
	///キーの状態を強制設定
	///</summary>
	///<param name="no">コントローラー番号</param>
	///<param name="state">状態</param>
	///<param name="button">ボタン</param>
	///<returns>無し</returns>
	static void SetState(int no, CInput::E_STATE state, CInput::E_BUTTON button) {
		m_key_state[no][state] |= (1 << button);
	}

	///<summary>
	///各ボタンに対応するキーを設定
	///</summary>
	///<param name="no">コントローラー番号</param>
	///<param name="button">ボタン</param>
	///<param name="code">キーコード</param>
	///<returns>無し</returns>
	static void SetButton(int no, CInput::E_BUTTON button, unsigned char code) {
		m_key_code[no][button] = code;
	}
	///<summary>
	///各ボタンに対応するキーを設定
	///</summary>
	///<param name="no">コントローラー番号</param>
	///<param name="button">ボタン</param>
	///<param name="num">パッドボタン</param>
	///<returns>無し</returns>
	static void SetPadButton(int no, CInput::E_BUTTON button, unsigned char num) {
		m_pad_code[no][button] = num;
	}


	///<summary>
	///マウスカーソルの座標を取得
	///</summary>
	///<returns>マウスカーソルの座標</returns>
	static CVector2D GetMousePoint() {
		POINT pt;
		//マウスの現在の座標を取得する
		GetCursorPos(&pt);
		ScreenToClient(GL::hWnd, &pt);

		return CVector2D((float)pt.x, (float)pt.y);
	}

	///<summary>
	///マウスの移動量を取得
	///</summary>
	///<returns>マウスの移動量</returns>
	static CVector2D GetMouseVec() {
		return m_mouse_vec;
	}

	///<summary>
	///マウスカーソルをウインドウ内に収めるフラグの設定
	///</summary>
	///<param name="f">true:ウインドウ内に収める</param>
	///<param name="f">true:ウインドウ内でカーソルがループする</param>
	///<returns>無し</returns>
	static void SetMouseInside(bool f);

	///<summary>
	///マウスカーソルの表示設定
	///</summary>
	///<param name="f">true:表示　false:非表示</param>
	///<returns>無し</returns>
	static void ShowCursor(bool f);

	///<summary>
	///マウスホイールの変化量取得
	///</summary>
	///<returns>マウスホイールの変化量</returns>
	static int GetMouseWheel() {
		return m_mouse_wheelOld;
	}
	///<summary>
	///マウスホイールの変化量を加える
	///</summary>
	///<param name="a">変化量</param>
	///<returns>無し</returns>
	static void AddMouseWheel(int a) {
		m_mouse_wheel += a;
	}
	///<summary>
	///パッドデータの取得
	///</summary>
	///<param name="no">コントローラ番号</param>
	///<returns>無し</returns>
	static DIJOYSTATE2* GetPadData(int no) {
		return (m_device[no].m_pPadDevice) ? &m_device[no].js : NULL;
	}
	///<summary>
	///右スティックを取得
	///</summary>
	///<param name="no">コントローラ番号</param>
	///<returns>Vec -1.0～1.0</returns>
	static CVector2D GetRStick(int no);
	///<summary>
	///左スティックを取得
	///</summary>
	///<param name="no">コントローラ番号</param>
	///<returns>Vec -1.0～1.0</returns>
	static CVector2D GetLStick(int no);
	///<summary>
	///キーの状態を更新する
	///</summary>
	///<returns>無し</returns>
	static void Update();

	static float toFloat(int i) {
		return ((float)i - (0x7FFF)) / 0x8000;
	};
	static void UpdateClipCursor(int f);
};

#define PUSH(key) CInput::GetState(0,CInput::ePush,key)
#define HOLD(key) CInput::GetState(0,CInput::eHold,key)
#define PULL(key) CInput::GetState(0,CInput::ePull,key)
#define FREE(key) CInput::GetState(0,CInput::eFree,key)


#define PUSH_PAD(id,key) CInput::GetState(id,CInput::ePush,key)
#define HOLD_PAD(id,key) CInput::GetState(id,CInput::eHold,key)
#define PULL_PAD(id,key) CInput::GetState(id,CInput::ePull,key)
#define FREE_PAD(id,key) CInput::GetState(id,CInput::eFree,key)

#define DEBUG_PUSH(key) CInput::GetState(0,CInput::ePush,key,true)
#define DEBUG_HOLD(key) CInput::GetState(0,CInput::eHold,key,true)
#define DEBUG_PULL(key) CInput::GetState(0,CInput::ePull,key,true)
#define DEBUG_FREE(key) CInput::GetState(0,CInput::eFree,key,true)
