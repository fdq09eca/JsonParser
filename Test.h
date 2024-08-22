#pragma once
#include "JsonParser.h"

namespace CL {

class ParserTests
{
public:
	static void test_parseObject();
}; // class ParserTests

class TokenizerTests {
	static void _getNumber(const char* v);

public:
	static void test_nextToken();
	static void test_getNumber();
}; // class TokenizerTests

} // namespace CL

