#pragma once

#include <Windows.h>

#if DLL_DECL
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif


DLLEXPORT BOOL __stdcall InstallKeyboardHook(HWND hWnd, UINT uMyMsg);

#ifdef __cplusplus
}
#endif
