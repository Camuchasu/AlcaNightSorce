#include "GL.h"

//GPUを強制的に使用するための設定
extern "C" {
	//NVIDIA Optimus テクノロジーを有効にし、ディスクリートGPUを使用する
	_declspec(dllexport) DWORD NvOptimusEnablement = 1;
	//AMD PowerXpress でハイパフォーマンスなGPUを使用する
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

//GL名前空間のグローバル変数の実体定義
GLFWwindow* GL::window;
RECT GL::window_rect;
int GL::window_x;
int GL::window_y;
int GL::window_width;
int GL::window_height;
int GL::focus = GLFW_TRUE;
HWND GL::hWnd;
bool GL::full_screen = false;
bool GL::isQuit = false;

//ウィンドウの矩形領域情報を更新する関数
void GL::UpdateWindowRect(int x, int y, int w, int h) {
	window_x = x; window_y = y;
	window_width = w; window_height = h;
	//マージンを考慮して内部領域のサイズを計算
	window_rect.top = window_y + 32;
	window_rect.left = window_x + 16;
	window_rect.right = window_x + window_width - 16;
	window_rect.bottom = window_x + window_height - 24;
}

//デバイス(GPU)の各種制限や性能情報をコンソールに出力する関数
void GL::PrintDeviceInformation() {
	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	GLint va;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &va);
	GLint vv;
	glGetIntegerv(GL_MAX_VARYING_VECTORS, &vv);
	GLint vuv;
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &vuv);
	GLint fuv;
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &fuv);
	GLint vtiu;
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &vtiu);
	GLint tiu;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &tiu);
	GLint ts;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &ts);
	GLint rs;
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &rs);
	GLint vd[2];
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS, vd);
	printf("GL_MAX_VERTEX_ATTRIBS %d\n", va);
	printf("GL_MAX_VARYING_VECTORS %d\n", va); //NOTE: 元のコードのままだが変数はvaになっている
	printf("GL_MAX_VERTEX_UNIFORM_VECTORS %d\n", vuv);
	printf("GL_MAX_FRAGMENT_UNIFORM_VECTORS %d\n", fuv);
	printf("GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS %d\n", vtiu);
	printf("GL_MAX_TEXTURE_IMAGE_UNITS %d\n", tiu);
	printf("GL_MAX_TEXTURE_SIZE %d\n", ts);
	printf("GL_MAX_RENDERBUFFER_SIZE %d\n", rs);
	printf("GL_MAX_VIEWPORT_DIMS %d %d\n", vd[0], vd[1]);
}

//フルスクリーンモードとウィンドウモードを切り替える関数
void GL::ChangeFullScreen(bool full, int monitor_no)
{
	//既に指定のモードであれば何もしない
	if (full == GL::full_screen) return;
	//モニター情報の取得
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	//モニタ番号が範囲外の場合はデフォルトモニタ(0)を使用する
	if (monitor_no >= count) monitor_no = 0;

	GLFWmonitor* monitor = monitors[monitor_no];
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	//フルスクリーンへの切り替え
	if (!GL::full_screen) {
		glfwGetWindowPos(GL::window, &window_x, &window_y);
		glfwSetWindowMonitor(GL::window, monitor, 0, 0, GL::window_width, GL::window_height, mode->refreshRate);
		GL::full_screen = true;
	}
	//ウィンドウモードへの切り替え
	else {
		glfwSetWindowMonitor(GL::window, nullptr, window_x, window_y, GL::window_width, GL::window_height, mode->refreshRate);
		GL::full_screen = false;
	}
}
