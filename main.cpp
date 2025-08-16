#include <windows.h>
#include <fstream>
#include "nya_commonhooklib.h"

#include "fo1.h"
#include "../nya-common-fouc/fo2versioncheck.h"

std::string GetLUAErrorLocation(void* a1) {
	tLUAStack stack = {}; // esp+0x64
	if (!lua_getstack(a1, 1, &stack)) return "";
	if (!lua_getinfo(a1, "Snl", &stack)) return "";
	return std::format("{}:{}", stack.sFilename, stack.nLineNumber);
}

void LogLUAErr(void* a1) {
	std::string str = std::format("{}\n{}", (const char*)lua_tolstring(a1, lua_gettop(a1)), GetLUAErrorLocation(a1));
	MessageBoxA(nullptr, str.c_str(), "Runtime error", 0x10);
	exit(0);
}

void LogLUAFatalErr(void* a1) {
	std::string str = std::format("{}\n{}", (const char*)lua_tolstring(a1, lua_gettop(a1)), GetLUAErrorLocation(a1));
	MessageBoxA(nullptr, str.c_str(), "Fatal error", 0x10);
	exit(0);
}

void* LogLUALoadErr(void* a1, int a2) {
	std::string str = std::format("{}\n{}", (const char*)lua_tolstring(a1, lua_gettop(a1)), GetLUAErrorLocation(a1));
	MessageBoxA(nullptr, str.c_str(), "Load error", 0x10);
	exit(0);
}

const char* aFatalError1 = nullptr;
const char* aFatalError2 = nullptr;
void LogLUAFatalErrNew() {
	MessageBoxA(nullptr, std::format("{}\n{}", aFatalError1, aFatalError2).c_str(), "Fatal error", 0x10);
	exit(0);
}

uintptr_t sub_50E4D0 = 0x50E4D0;
uintptr_t sub_50E090 = 0x50E090;
uintptr_t sub_4D84A0 = 0x4D84A0;
void __attribute__((naked)) LogLUAFatalErrASM() {
	__asm__ (
		"push 0xFFFFFFFF\n\t"
		"push esi\n\t"
		"call %3\n\t"
		"mov %1, eax\n\t"
		"push 0xFFFFFFFF\n\t"
		"push esi\n\t"
		"call %4\n\t"
		"call %5\n\t"
		"push 0xFFFFFFFF\n\t"
		"push esi\n\t"
		"call %3\n\t"
		"mov %2, eax\n\t"
		"jmp %0\n\t"
			:
			: "i" (LogLUAFatalErrNew), "m" (aFatalError1), "m" (aFatalError2), "m" (sub_50E4D0), "m" (sub_50E090), "m" (sub_4D84A0)
	);
}

BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID) {
	switch( fdwReason ) {
		case DLL_PROCESS_ATTACH: {
			DoFlatOutVersionCheck(FO2Version::FO1_1_1);

			NyaHookLib::Patch(0x4D5AE5, &LogLUAErr);
			NyaHookLib::Patch(0x4D6ADB, &LogLUAErr);
			NyaHookLib::Patch(0x4D6B8F, &LogLUAErr);
			NyaHookLib::Patch(0x4D6C5C, &LogLUAErr);
			//NyaHookLib::PatchRelative(NyaHookLib::JMP, 0x4D7D03, &LogLUAFatalErrASM);
			NyaHookLib::PatchRelative(NyaHookLib::JMP, 0x4D7CA0, &LogLUAFatalErr);
			NyaHookLib::PatchRelative(NyaHookLib::CALL, 0x4D5D32, &LogLUALoadErr);
			NyaHookLib::PatchRelative(NyaHookLib::CALL, 0x4D5E93, &LogLUALoadErr);
		} break;
		default:
			break;
	}
	return TRUE;
}