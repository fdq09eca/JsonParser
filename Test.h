#pragma once
#include "JsonParser.h"

namespace CL {

class ParserTests
{
public:
	static void test_parseObject() {
		JsonParser p("{ \"key1\" : [null, true, false, 123] }");
		auto obj = p.parseObject();
		TEST(obj != nullptr);
		TEST(obj->getType() == JsonValue::EType::Object);
	}
};

class TokenizerTests {
	static void _getNumber(const char* v) { // TODO: pls teach me how to use marco and print out the v value
		Tokenizer t(v);
		auto b = t.nextToken();
		TEST(t.isNumber(v));
	}

public:
	static void test_nextToken() {
		Tokenizer t("{ \"key1\" : [null, true, false, 123] }");

		auto b = t.nextToken();
		TEST(b);
		TEST(t.isOp("{"));


		b = t.nextToken();
		TEST(b);
		TEST(t.isString("key1"));

		b = t.nextToken();
		TEST(b);
		TEST(t.isOp(":"));


		b = t.nextToken();
		TEST(b);
		TEST(t.isOp("["));

		b = t.nextToken();
		TEST(b);
		TEST(t.isNull());

		b = t.nextToken();
		TEST(b);
		TEST(t.isOp(","));

		b = t.nextToken();
		TEST(b);
		TEST(t.isBool("true"));

		b = t.nextToken();
		TEST(b);
		TEST(t.isOp(","));

		b = t.nextToken();
		TEST(b);
		TEST(t.isBool("false"));

		b = t.nextToken();
		TEST(b);
		TEST(t.isOp(","));

		b = t.nextToken();
		TEST(b);
		TEST(t.isNumber("123"));
	}


	static void test_getNumber() {
		// zero
		_getNumber("0");
		_getNumber("0.0");
		_getNumber("0.00");
		_getNumber("0.000");
		_getNumber("0.0e0");
		_getNumber("0.0e+0");
		_getNumber("0.0e-0");
		_getNumber("0.0E0");
		_getNumber("0.0E+0");
		_getNumber("0.0E-0");

		//positive
		_getNumber("1");
		_getNumber("1.0");
		_getNumber("1.00");
		_getNumber("1.000");
		_getNumber("1.1");
		_getNumber("1.11");
		_getNumber("1.123");
		_getNumber("1.0e0");
		_getNumber("1.0e+0");
		_getNumber("1.0e-0");
		_getNumber("1.123e0");
		_getNumber("1.123e+0");
		_getNumber("1.123e-0");
		_getNumber("1.0E0");
		_getNumber("1.0E+0");
		_getNumber("1.0E-0");

		_getNumber("12");
		_getNumber("12.0");
		_getNumber("12.00");
		_getNumber("12.000");
		_getNumber("12.1");
		_getNumber("12.11");
		_getNumber("12.123");
		_getNumber("12.0e0");
		_getNumber("12.0e+0");
		_getNumber("12.0e-0");
		_getNumber("12.123e0");
		_getNumber("12.123e+0");
		_getNumber("12.123e-0");

		_getNumber("12.123e123");
		_getNumber("12.123e+123");
		_getNumber("12.123e-123");
		_getNumber("12.0E0");
		_getNumber("12.0E+123");
		_getNumber("12.0E-123");

		//positive
		_getNumber("-1");
		_getNumber("-1.0");
		_getNumber("-1.00");
		_getNumber("-1.000");
		_getNumber("-1.1");
		_getNumber("-1.11");
		_getNumber("-1.123");
		_getNumber("-1.0e0");
		_getNumber("-1.0e+0");
		_getNumber("-1.0e-0");
		_getNumber("-1.123e0");
		_getNumber("-1.123e+0");
		_getNumber("-1.123e-0");
		_getNumber("-1.0E0");
		_getNumber("-1.0E+0");
		_getNumber("-1.0E-0");
	}
}; // class TokenizerTests



} // namespace CL

