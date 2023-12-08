#include <string>
#include <vector>
#include <utils.h>

#include "scorpio.h"

#if PLATFORM_ANDROID

#include <dobby.h>
#include "KittyMemory.hpp"

[[gnu::constructor]]
void constructor() {
	KittyMemory::ProcMap cocos2dcppBaseMap;

	do
	{
		cocos2dcppBaseMap = KittyMemory::getElfBaseMap("libcocos2dcpp.so");
	} while (!cocos2dcppBaseMap.isValid());

	utils::assign_addresses_to_functions({
		std::make_pair(&GJAccountManager_sharedInstance, ADDR_FROM_SYM("_ZN16GJAccountManager11sharedStateEv")),
	});

	DobbyHook(ADDR_FROM_SYM("_ZN7cocos2d9extension12CCHttpClient4sendEPNS0_13CCHttpRequestE"), (void*)&CCHttpClient_send_H, (void**)&CCHttpClient_send);
}

#elif PLATFORM_WINDOWS

#include <MinHook.h>

DWORD WINAPI LoadHooks(void* hinstDll)
{
	if (MH_Initialize() != MH_OK)
		FreeLibraryAndExitThread((HMODULE)hinstDll, 727);

	auto GeometryDash = GetModuleHandle(0);
	auto libExtensions = GetModuleHandle("libExtensions.dll");

	uintptr_t GAM_sI = 0;

	if constexpr (GD_VERSION == "1.91")
	{
		GAM_sI = 0x79430;
	}
	else if constexpr (GD_VERSION == "1.92")
	{
		GAM_sI = 0x79A80;
	}
	else
	{
		static_assert(GD_VERSION == "1.91" || GD_VERSION == "1.92", "Invalid value for GD_VERSION");
	}

	utils::assign_addresses_to_functions({
		std::make_pair(&CCHttpRequest_setRequestData, GetProcAddress(libExtensions, "?setRequestData@CCHttpRequest@extension@cocos2d@@QAEXPBDI@Z")),
		std::make_pair(&GJAccountManager_sharedInstance, ADD_BASE_ADDR(GeometryDash, GAM_sI)),
	});

	MH_CreateHook(
		(LPVOID)GetProcAddress(libExtensions, "?send@CCHttpClient@extension@cocos2d@@QAEXPAVCCHttpRequest@23@@Z"),
		(LPVOID)&CCHttpClient_send_H,
		reinterpret_cast<LPVOID*>(&CCHttpClient_send)
	);

	MH_EnableHook(MH_ALL_HOOKS);

	return 0;
}

__declspec(dllexport) BOOL WINAPI DllMain(
	HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved
) {
	static_assert(sizeof(std::string) == 24,
		"Size of std::string is incorrect, swtich to release to fix.");

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0x1000, LoadHooks, hinstDLL, 0, 0);
		break;
	default:
		break;
	}

	return true;
}

#endif
