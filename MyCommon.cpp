#include "MyCommon.h"

namespace CL {

bool Util::isStartsWithSubString(const String& mainStr, const String& subStr)
{
	return Util::isStartsWithSubString(mainStr.c_str(), subStr.c_str());
}

const char* Util::isContainsSubString(const String& mainStr, const String& subStr)
{
	return Util::isContainsSubString(mainStr.c_str(), subStr.c_str());
}

bool Util::isStartsWithSubString(const char* sz, const char* sub_sz)
{
	return strncmp(sz, sub_sz, strlen(sub_sz)) == 0;
}

const char* Util::isContainsSubString(const char* sz, const char* sub_sz)
{
	if (strlen(sub_sz) == 0) {
		if (strlen(sz) > 0) {
			return nullptr;
		}
	}
	return strstr(sz, sub_sz);
}

String Util::toUpper(const String& s) {
	String o(s.begin(), s.end());
	for (auto& c : o) {
		c = ::toupper(c);
	}
	return o;
}

String Util::toLower(const String& s) {
	String o(s.begin(), s.end());

	for (auto& c : o) {
		c = ::tolower(c);
	}
	return o;
}

} // namespace CL