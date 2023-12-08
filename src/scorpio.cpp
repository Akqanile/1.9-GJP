#include "scorpio.h"

#include <sstream>
#include "utils.h"
#include "CCHttpRequest.h"

void* (__fastcall* GJAccountManager_sharedInstance)();

#ifdef PLATFORM_ANDROID
	void (*CCHttpClient_send)(void*, void*);
	void CCHttpClient_send_H(void* self, CCHttpRequest* request)
#elif PLATFORM_WINDOWS
	void(__thiscall* CCHttpRequest_setRequestData)(CCHttpRequest*, const char* h, unsigned int dd);
	void (__thiscall* CCHttpClient_send)(void*, void*);
	void __fastcall CCHttpClient_send_H(void* self, void* edx, CCHttpRequest* request)
#endif
{
	auto url = &request->_url;

	auto requestData = std::string(request->_requestData.data(), request->_requestData.size());

	if (requestData.empty())
		return CCHttpClient_send(self, (void*)request);;

	auto form = utils::url::map_from_urlform(requestData);

	std::stringstream ss;
	ss << requestData;

	if (form.find("accountID") != form.end())
	{
		std::string password = *PTR_TO_MEMBER_BY_OFFSET(std::string, GJAccountManager_sharedInstance(), 0xec);
		ss << "&gjp=" << utils::gjp(password);
	}
	auto new_req_data = ss.str();

#if PLATFORM_WINDOWS
	CCHttpRequest_setRequestData(request, new_req_data.c_str(), new_req_data.size());
#elif PLATFORM_ANDROID
	request->_requestData.assign(new_req_data.c_str(), new_req_data.c_str() + new_req_data.length());
#endif

	CCHttpClient_send(self, (void*)request);
}
