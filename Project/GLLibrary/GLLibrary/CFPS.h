///<summary>
///FPS制御クラス
///</summary>
#pragma once

#include <time.h>
#include <windows.h>

class CFPS {
	//時間計測用
	static LARGE_INTEGER freq;
	static LARGE_INTEGER time_buf;
	static int fps;
	static int m_Setfps;
	static float deltaTime;
	static float timeScale;

public:
	///<summary>
	///初期化を行う
	///</summary>
	///<returns>無し</returns>
	static void Init();

	///<summary>
	///６０FPSになるよう待機する
	///</summary>
	///<returns>無し</returns>
	static void Wait();

	///<summary>
	///現在のFPSを取得
	///</summary>
	///<returns>FPS</returns>
	static int GetFPS() { return fps; }

	///<summary>
	///現在のdeltaTimeを取得
	///</summary>
	///<returns>deltaTime</returns>
	static float GetDeltaTime() { return deltaTime; }
	static LONGLONG GetTimeCnt();
	static float GetTimeScale();
	///<summary>
	///<para>FPSを設定します</para>
	///<para>1～60まで設定できます</para>
	///</summary>
	static void SetFPS(int fps);

	static void SetTimeScale(float scale);

};

class Time {
public:
	static LARGE_INTEGER freq;
	static LARGE_INTEGER time_buf;
	static void Start();
	static float End();
};