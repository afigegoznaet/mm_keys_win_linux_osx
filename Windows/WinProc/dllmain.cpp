// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include "khook.h"
static UINT uMsg = 0;
static HWND hWndMain = nullptr;
static HHOOK hKeyHook = nullptr;

static HINSTANCE hInstance = nullptr;
static HOOKPROC lpfnHookProc = nullptr;


//---------------------------------------------------------------------------
// Protoypes:


//---------------------------------------------------------------------------
// Here's where the hook gets installed:

BOOL InstallKeyboardHook(HWND hWnd, UINT uMyMsg) {
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

//---------------------------------------------------------------------------
// Here's a skeletal keyboardproc:

LRESULT KeyboardFunc(int nCode, WPARAM wParam, LPARAM lParam) {
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
		bPassThrough = TRUE;
		break;
	}

	if (bPassThrough) {
		return (CallNextHookEx(hKeyHook, nCode, wParam, lParam));
	} else
		return TRUE; // We have processed this key
}
