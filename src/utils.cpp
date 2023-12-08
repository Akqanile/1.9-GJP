#include "utils.h"

#include <sstream>
#include "libbase64.h"

NS_UTILS_BEGIN

void assign_addresses_to_functions(std::vector<std::pair<void* /*fn placeholder*/, void* /*fn addr*/>> map_fn)
{
	for (auto pair : map_fn)
	{
		void** placeholder = static_cast<void**>(pair.first);
		void* address = pair.second;

		*placeholder = address;
	}
}

#ifdef PLATFORM_ANDROID

#include <dlfcn.h>

void* get_sym_addr(const char* sym_name)
{
	auto handle = dlopen("libcocos2dcpp.so", RTLD_NOW);
	return dlsym(handle, sym_name);
}

#endif

std::vector<char> xor_cipher(const std::string& src, const std::string& key)
{
	std::vector<char> buf;

	for (size_t i = 0; i < src.size(); i++)
	{
		buf.push_back(src[i] ^ key[i % key.size()]);
	}

	return buf;
}

std::string gjp(const std::string& password) {
	std::vector<char> xored = xor_cipher(password, "37526");

	char* buf = static_cast<char*>(malloc(xored.size() * 2));
	size_t bytes_written = 0;

	base64_encode(xored.data(), xored.size(), buf, &bytes_written, 0);
	
	std::string result(buf, bytes_written);
	free(buf);

	return result;
}

NS_UTILS_END


NS_UTILS_URL_BEGIN

inline std::vector<std::string> split(std::string s, std::string delimiter, bool once) {
	std::vector<std::string> res{};

	size_t offset{};

	do
	{
		size_t pos = s.find(delimiter, offset);
		std::string str = s.substr(offset, pos - offset);

		res.push_back(str);

		if (once)
		{
			res.push_back(s.substr(pos + delimiter.size()));
			break;
		}

		if (pos == 0xFFFFFFFF)
			break;

		offset = pos + delimiter.size();
	} while (true);

	return res;
}

std::unordered_map<std::string, std::string> map_from_urlform(const std::string& form)
{
	std::unordered_map<std::string, std::string> res;

	for (auto _item : split(form, "&", false))
	{
		auto _item_split = split(_item, "=", true);

		auto key = _item_split.at(0);
		auto value = _item_split.at(1);

		res.emplace(key, value);
	}

	return res;
}


std::string urlform_from_map(const std::unordered_map<std::string, std::string>& map)
{
	std::stringstream ss;
	bool beginning = true;

	for (auto _pair : map)
	{
		auto key = _pair.first;
		auto value = _pair.second;

		if (!beginning)
			ss << "&";
#if PLATFORM_ANDROID
		ss << strdup(key.c_str()) << "=" << strdup(value.c_str());
#else
		ss << _strdup(key.c_str()) << "=" << _strdup(value.c_str());
#endif
		beginning = false;
	}

	auto res = ss.str();

	return res;
}

NS_UTILS_URL_END
