#pragma once

#include "utils.h"

#ifdef PLATFORM_ANDROID
	#define __thiscall
	#define __fastcall
#endif

struct CCHttpRequest;


#ifdef PLATFORM_ANDROID
	extern void* (*GJAccountManager_sharedInstance)();

	extern void (*CCHttpClient_send)(void*, void*);
	extern void CCHttpClient_send_H(void* self, CCHttpRequest* request);
#elif PLATFORM_WINDOWS
	extern void* (__fastcall* GJAccountManager_sharedInstance)();

	extern void (__thiscall* CCHttpRequest_setRequestData)(CCHttpRequest*, const char* h, unsigned int dd);
	extern void (__thiscall* CCHttpClient_send)(void*, void*);
	extern void __fastcall CCHttpClient_send_H(void* self, void* edx, CCHttpRequest* request);
#endif
