#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <Windows.h>
#include <QDebug>
#include <QTimer>
#include "khook.h"

UINT g_uMsg = 0;

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "gdi32.lib")

void OutputDebugString(wchar_t *msg) {
	qDebug() << QString::fromStdWString(msg);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
							LPARAM lParam) {
	wchar_t msg[32];
	switch (uMsg) {
	case WM_SYSKEYDOWN:
		swprintf_s(msg, L"WM_SYSKEYDOWN: 0x%x\n", wParam);
		OutputDebugString(msg);
		break;

	case WM_SYSCHAR:
		swprintf_s(msg, L"WM_SYSCHAR: %c\n", (wchar_t)wParam);
		OutputDebugString(msg);
		break;

	case WM_SYSKEYUP:
		swprintf_s(msg, L"WM_SYSKEYUP: 0x%x\n", wParam);
		OutputDebugString(msg);
		break;

	case WM_KEYDOWN:
		swprintf_s(msg, L"WM_KEYDOWN: 0x%x\n", wParam);
		OutputDebugString(msg);
		break;

	case WM_KEYUP:
		swprintf_s(msg, L"WM_KEYUP: 0x%x\n", wParam);
		OutputDebugString(msg);
		break;

	case WM_CHAR:
		swprintf_s(msg, L"WM_CHAR: %c\n", (wchar_t)wParam);
		OutputDebugString(msg);
		break;

		/* Handle other messages (not shown) */
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

std::wstring GetLastErrorAsString() {
	// Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::wstring(); // No error message has been recorded

	LPWSTR messageBuffer = nullptr;
	size_t size = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
			| FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&messageBuffer, 0, nullptr);

	std::wstring message(messageBuffer, size);

	// Free the buffer.
	LocalFree(messageBuffer);

	return message;
}


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);

	auto wnd = FindWindow(nullptr, L"MainWindow");
	// auto winst = (HINSTANCE)GetWindowLongPtrW(HWND(winId()), GWLP_HINSTANCE);
	auto winst = (HINSTANCE)GetWindowLongPtrW(wnd, GWLP_HINSTANCE);
	// auto winst = GetModuleHandle(nullptr);
	OutputDebugString(GetLastErrorAsString().c_str());
	WNDCLASSEX wcex = {0};


	wcex.lpfnWndProc = WindowProc;
	wcex.hInstance = winst;
	wcex.cbSize = sizeof(wcex);
	wcex.lpszClassName = L"MainWindow";

	auto res = RegisterClassEx(&wcex);

	qDebug() << res;

	OutputDebugString(GetLastErrorAsString().c_str());

	auto kbSender = [] {
		/*
		LPARAM lp = {0};
		HWND hw = GetTopWindow(nullptr);
		DefWindowProc(hw, WM_KEYDOWN, 0x4a, lp);
		DefWindowProc(hw, WM_CHAR, L'j', lp);
		DefWindowProc(hw, WM_KEYUP, 0x4a, lp);
*/
		INPUT inp = {0};
		inp.type = INPUT_KEYBOARD;
		inp.ki.wVk = VK_VOLUME_UP;
		// inp.ki.wScan=KEYEVENTF_KEYUP;

		SendInput(1, &inp, sizeof(inp));
		inp.ki.wScan = KEYEVENTF_KEYUP;
		SendInput(1, &inp, sizeof(inp));
		OutputDebugString(GetLastErrorAsString().c_str());
	};

	auto msSender = [] {
		/*
		LPARAM lp = {0};
		HWND hw = GetTopWindow(nullptr);
		DefWindowProc(hw, WM_KEYDOWN, 0x4a, lp);
		DefWindowProc(hw, WM_CHAR, L'j', lp);
		DefWindowProc(hw, WM_KEYUP, 0x4a, lp);


		tagMOUSEINPUT {
			LONG      dx;
			LONG      dy;
			DWORD     mouseData;
			DWORD     dwFlags;
			DWORD     time;
			ULONG_PTR dwExtraInfo;
		}
		*/
		INPUT inp = {0};
		inp.type = INPUT_MOUSE;
		inp.mi.dx = 5;
		inp.mi.dy = 0;
		inp.mi.mouseData = 0;
		inp.mi.dwFlags = MOUSEEVENTF_MOVE;
		inp.mi.time = 0;
		inp.mi.dwExtraInfo = 0;

		// inp.ki.wScan=KEYEVENTF_KEYUP;

		SendInput(1, &inp, sizeof(inp));
		inp.mi.dx = 0;
		inp.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		SendInput(1, &inp, sizeof(inp));
		inp.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		SendInput(1, &inp, sizeof(inp));
		OutputDebugString(GetLastErrorAsString().c_str());
	};

	QTimer *timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, kbSender);
	timer->start(5000);

	g_uMsg = RegisterWindowMessage(L"HOOKTEST_MARY_HAD_A_LITTLE_LAMB");
	if (InstallKeyboardHook((HWND)this->winId(), g_uMsg))
		OutputDebugString(L"Hook installed OK\n");

	OutputDebugString(GetLastErrorAsString().c_str());
}

MainWindow::~MainWindow() { delete ui; }
