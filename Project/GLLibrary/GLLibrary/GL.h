#pragma once

//標準Cライブラリ
#include <stdio.h>
#include <stdlib.h>

//数学定数(M_PI等)の有効化
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <math.h>
#include <windows.h>
#include <string.h>

//GLEWの静的リンク用定義
#define GLEW_STATIC
#include "../../include/glew.h"

//GLFWとGLUのインクルード
#define GLFW_INCLUDE_GLU
#include "../../include/GLFW/glfw3.h"

//#define FREEGLUT_STATIC
//#include <freeglut.h>

//OpenALの静的リンク用定義
#define AL_LIBTYPE_STATIC
//#include <AL/alut.h>

//OpenGLやウィンドウに関連するグローバル情報をまとめた名前空間
namespace GL {
	//メインウィンドウのポインタ
	extern GLFWwindow* window;
	//ウィンドウの矩形領域情報
	extern RECT window_rect;
	//ウィンドウのX座標
	extern int window_x;
	//ウィンドウのY座標
	extern int window_y;
	//ウィンドウの幅
	extern int window_width;
	//ウィンドウの高さ
	extern int window_height;
	//ウィンドウがフォーカスを持っているかどうかのフラグ
	extern int focus;
	//フルスクリーンモードかどうかのフラグ
	extern bool full_screen;
	//アプリケーション終了フラグ
	extern bool isQuit;
	//Windows API用ウィンドウハンドル
	extern HWND hWnd;

	///<summary>
	///ウィンドウの矩形領域情報を更新する
	///</summary>
	void UpdateWindowRect(int x, int y, int w, int h);

	///<summary>
	///スペルミス対応用のラッパー関数
	///</summary>
	inline void UpdateWindosRect(int x, int y, int w, int h) {
		UpdateWindowRect(x, y, w, h);
	}

	///<summary>
	///デバイス情報の出力
	///</summary>
	void PrintDeviceInformation();

	///<summary>
	///フルスクリーン/ウィンドウモードの切り替え
	///</summary>
	void ChangeFullScreen(bool full, int monitor_no = 0);
}

//デバッグモードの判定
#ifdef _DEBUG
#else
#define NDEBUG
#endif
