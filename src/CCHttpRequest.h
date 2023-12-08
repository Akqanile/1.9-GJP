#pragma once

#include <string>
#include <vector>

struct CCHttpRequest {
	typedef enum
	{
		kHttpGet,
		kHttpPost,
		kHttpPut,
		kHttpDelete,
		kHttpUnkown,
	} HttpRequestType;

#ifdef PLATFORM_ANDROID
	char pad[24];
#elif PLATFORM_WINDOWS
	char pad[22];
#endif

	HttpRequestType             _requestType;
	std::string                 _url;
	std::vector<char>           _requestData;
};
