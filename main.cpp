#include <windows.h>
#include <fstream>
#include "nya_commonhooklib.h"

#include "fo1.h"
#include "../nya-common-fouc/fo2versioncheck.h"

void LogLUAErr(void* a1) {
	MessageBoxA(nullptr, (const char*)lua_tolstring(a1, lua_gettop(a1)), "Runtime error", 0x10);
	exit(0);
}

void* LogLUALoadErr(void* a1, int a2) {
	MessageBoxA(nullptr, (const char*)lua_tolstring(a1, a2), "Load error", 0x10);
	exit(0);
}

BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID) {
	switch( fdwReason ) {
		case DLL_PROCESS_ATTACH: {
			DoFlatOutVersionCheck(FO2Version::FO1_1_1);

			NyaHookLib::Patch(0x4D5AE5, &LogLUAErr);
			NyaHookLib::Patch(0x4D6ADB, &LogLUAErr);
			NyaHookLib::Patch(0x4D6B8F, &LogLUAErr);
			NyaHookLib::Patch(0x4D6C5C, &LogLUAErr);
			NyaHookLib::PatchRelative(NyaHookLib::CALL, 0x4D5D32, &LogLUALoadErr);
			NyaHookLib::PatchRelative(NyaHookLib::CALL, 0x4D5E93, &LogLUALoadErr);
		} break;
		default:
			break;
	}
	return TRUE;
}