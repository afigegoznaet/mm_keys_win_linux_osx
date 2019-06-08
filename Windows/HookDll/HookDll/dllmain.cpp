// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <Windows.h>

#include "khook.h"

#pragma data_seg(".SHOOK")
static UINT uMsg = 0;
static HWND hWndMain = nullptr;
static HHOOK hKeyHook = nullptr;
#pragma data_seg()

__declspec(allocate(".SHOOK")) int varx = 1;


static HINSTANCE hInstance = 0;
static HOOKPROC lpfnHookProc = 0;


#pragma comment(linker, "/section:.SHOOK,RWS")


//---------------------------------------------------------------------------
// Protoypes:


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
					  LPVOID lpReserved) {
	hInstance = hModule;
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


//---------------------------------------------------------------------------
// Here's a skeletal keyboardproc:

LRESULT __stdcall KeyboardFunc(int nCode, WPARAM wParam, LPARAM lParam) {
	BOOL bPassThrough;
	wchar_t szDebug[100];

	// Check for exception cases...
	if (nCode < 0)
		return (CallNextHookEx(hKeyHook, nCode, wParam, lParam));

	if (nCode == HC_NOREMOVE)
		return (CallNextHookEx(hKeyHook, nCode, wParam, lParam));

	switch (wParam) {
	case VK_F2:
		wsprintf(szDebug, L"F2 key message, lparam = 0x%X\n", lParam);
		OutputDebugString(szDebug);

		// only send on keydown, not keyup
		if (HIWORD(lParam) & 0xC000) {
			OutputDebugString(L"F2 Keyup\n");
		} else {
			wsprintf(szDebug, L"Sending F2 keydown message %d to hwnd 0x%X\n",
					 uMsg, hWndMain);
			OutputDebugString(szDebug);
			PostMessage(hWndMain, uMsg, 0, 0);
		}
		bPassThrough = FALSE;
		break;


	default:
		wsprintf(szDebug, L"Uknown key message, wparam = 0x%X, lparam = 0x%X\n", wParam, lParam);
		OutputDebugString(szDebug);
		bPassThrough = TRUE;
		break;
	}

	if (bPassThrough) {
		return (CallNextHookEx(hKeyHook, nCode, wParam, lParam));
	} else
		return TRUE; // We have processed this key
}

//---------------------------------------------------------------------------
// Here's where the hook gets installed:

DLLEXPORT BOOL __stdcall InstallKeyboardHook(HWND hWnd, UINT uMyMsg) {
	wchar_t szDebug[100];

	wsprintf(szDebug, L"installing hook for client window 0x%X\n", hWnd);
	OutputDebugString(szDebug);

	uMsg = uMyMsg;
	lpfnHookProc = (HOOKPROC)KeyboardFunc;
	hKeyHook = SetWindowsHookEx(WH_KEYBOARD, lpfnHookProc, hInstance, NULL);
	hWndMain = hWnd;

	if (hKeyHook)
		return TRUE;
	else
		return FALSE;
}
