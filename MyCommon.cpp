#include "MyCommon.h"

namespace CL {
size_t Util::ndigit(size_t v) {
	if (v == 0) return 1;
	
	int n = 0;
	while (v) {
		v /= 10;
		n++;
	}
	return n;
}

size_t Util::ndigit(int v)
{
	v = abs(v);
	return Util::ndigit(static_cast<size_t>(v));
}

const char* Util::rfind(const char* sz, const char* p, char c) {
	if (!sz || !p) return nullptr;
	MY_ASSERT(p > sz);

	while (true) {
		if (p < sz)   return nullptr;
		if (*p == c)  return p;
		p--;
	}
}

const char* Util::getLine(const char* sz, const char* c, String& outLine) {
	if (!sz || !c || c == sz) return nullptr;
	auto* e = sz + strlen(sz);
	MY_ASSERT(*e == '\0');

	auto* q = strchr(c, '\n');
	if (!q) q = e; // end of string


	auto* p = Util::rfind(sz, q - 1, '\n');
	if (!p) p = sz;

	MY_ASSERT(p >= sz && q <= e);

	if (p == sz) {
		outLine.assign(p, q);
		outLine.append("\n");
		return p;
	}
	else
	{
		outLine.assign(p + 1, q);
		outLine.append("\n");
		return p - 1;
	}
}

const char* Util::skipWhiteSpace(const char* sz) {
	if (sz == nullptr) {
		return nullptr;
	}


	while (isspace(*sz)) {
		++sz;
	}
	return sz;
}

const char* Util::findNextChar(const char* sz, char c, char ec) {

	auto* p = strchr(sz, c);

	if (!p)
		return nullptr;

	if (!ec)
		return p + 1;

	if (*(p - 1) == ec) {
		p++;
		return findNextChar(p, c, ec);
	}

	return p + 1;
}


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
		c = static_cast<char>(::toupper(c));
	}
	
	return o;
}

String Util::toLower(const String& s) {
	String o(s.begin(), s.end());

	for (auto& c : o) {
		c = static_cast<char>(::tolower(c));
	}
	return o;
}

} // namespace CL