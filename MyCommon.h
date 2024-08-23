#pragma once

#include "pch.h"
#include <type_traits>

inline bool my_testfunc(bool x, const char* expr_str, int lineNumber) {
	if (!x)
		printf("[FAIL] %d: %s \n", lineNumber,expr_str);
	else {
		printf("[OK] %d: %s\n", lineNumber,expr_str);
	}
	return x;
}

#define TEST(EXPR) my_testfunc(EXPR, #EXPR, __LINE__)

#define MY_DUMP(X)                         \
    do                                     \
    {                                      \
        std::cout << #X ": " << X << "\n"; \
    } while (false)                        \
	// ------------------

inline void my_assert(bool x, const char* msg, int lineNumber, const char* filename)
{
	assert(x);
	if (!x)
		printf("[ASSERT_ERR] %s:%d : %s", filename, lineNumber, msg);
}

#define MY_ASSERT(x) my_assert(x, #x, __LINE__, __FILE__)

namespace CL
{
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;
using f128 = long double;

using String = std::string;

template <class T> using UPtr = std::unique_ptr<T>;
template <class T> using SPtr = std::shared_ptr<T>;
template <class T> using WPtr = std::weak_ptr<T>;
template <class T> using Vector = std::vector<T>;

template <class K, class V> using Map = std::map<K, V>;

class NonCopyable {
private:
	NonCopyable(const NonCopyable&) = delete;
	void operator=(const NonCopyable&) = delete;

public:
	NonCopyable() = default;

};

class MyError : public std::exception
{
public:
	MyError(const char* msg)
	{
		perror(msg);
		exit(errno);
	}

	MyError(const String& msg) : MyError(msg.c_str()) {}
	
};



namespace Util
{


size_t ndigit(size_t v);
size_t ndigit(int v);



const char* rfind(const char* sz, const char* p, char c);

const char* getLine(const char* sz, const char* c, String& outLine);

const char* skipWhiteSpace(const char* sz);

const char* findNextChar(const char* sz, char c, char ec = NULL);

bool isStartsWithSubString(const String& mainStr, const String& subStr);

const char* isContainsSubString(const String& mainStr, const String& subStr);

bool isStartsWithSubString(const char* sz, const char* sub_sz);

const char* isContainsSubString(const char* sz, const char* sub_sz);

String toUpper(const String& s);

String toLower(const String& s);

template <class E>
inline typename std::underlying_type<E>::type enumToInt(E e)
{
	return static_cast<typename std::underlying_type<E>::type>(e);
}

template <class K, class V>
bool hasKey(const Map<K, V>& map, K k) {
	return map.find(k) != map.end();
}

template <class T, class P>
void removeEraseIf(std::vector<T>& vec, P pred)
{
	vec.erase(std::remove_if(vec.begin(), vec.end(), pred), vec.end());
}

template <class T, class P>
void findAllIf(std::vector<T>& outBuff, const std::vector<T>& vec, P pred)
{
	if (&outBuff == &vec) {
		assert(false);
		return;
	}

	for (const auto& i : vec)
	{
		if (pred(i))
		{
			outBuff.emplace_back(i);
		}
	}
}

} // namespace Util
} // namespace CL