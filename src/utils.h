#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <stdlib.h>

#define NS_UTILS_BEGIN namespace utils {
#define NS_UTILS_URL_BEGIN namespace utils { namespace url {
#define NS_UTILS_END }
#define NS_UTILS_URL_END }}

#ifdef PLATFORM_ANDROID
	#define ADDR_FROM_SYM(sym) (void*)utils::get_sym_addr(sym)
#elif PLATFORM_WINDOWS
	#define ADDR_FROM_SYM(handle, sym) (void*)GetProcAddress(handle, sym)
#endif

#define ADD_BASE_ADDR(handle, addr) reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(handle) + addr)
#define PTR_TO_MEMBER_BY_OFFSET(type, obj, offset) reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(obj) + offset)

NS_UTILS_BEGIN

void assign_addresses_to_functions(std::vector<std::pair<void* /*fn placeholder*/, void* /*fn addr*/>> map_fn);

#if PLATFORM_ANDROID
void* get_sym_addr(const char* sym_name);
#endif

template <typename... args>
std::string format(const char* format, args... v)
{
	size_t buf_size = static_cast<size_t>(snprintf(nullptr, 0, format, v...)) + 1;
	char* buf = static_cast<char*>(malloc(buf_size));

	snprintf(buf, buf_size, format, v...);

	std::string res(buf, buf_size - 1);
	free(buf);

	return res;
}
std::string gjp(const std::string& password);
std::vector<char> xor_cipher(const std::string& src, const std::string& key);

NS_UTILS_END


NS_UTILS_URL_BEGIN

std::unordered_map<std::string, std::string> map_from_urlform(const std::string& form);
std::string urlform_from_map(const std::unordered_map<std::string, std::string>& map);

NS_UTILS_URL_END
