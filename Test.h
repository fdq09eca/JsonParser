#pragma once
#include "JsonParser.h"

namespace CL {

class MyCommonTests {
public:
	static void test_getLine();
	static void test_rfind();
	static void test_ndigit();
};

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

