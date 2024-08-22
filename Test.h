#pragma once
#include "JsonParser.h"

namespace CL {

class ParserTests
{
public:
	static void test_parseObject() {
		{
			JsonParser p("{ \"key1\" : [null, true, false, 123] }");
			JsonValue jv;

			p.parseObject(jv);
			
			TEST(jv.getType() == JsonValue::EType::Object);

			auto* obj = jv.getObject();
			TEST(obj != nullptr);

			TEST(obj->size() == 1);
			TEST(obj->find("key1") != obj->end()); // key1 exists

			const auto& v = obj->at("key1"); // jv non copyable
			TEST(v.getType() == JsonValue::EType::Array);
			auto* arr = v.getArray();
			TEST(arr != nullptr);
			TEST(arr->size() == 4);
			TEST(arr->at(0).isNull());
			TEST(arr->at(1).getBoolean());
			TEST(arr->at(2).getBoolean() == false);
			TEST(arr->at(3).getNumber() == 123);
		}

		{
			JsonParser p("{ \"key1\" : 123 }");

			auto jv = JsonValue();
			p.parseValue(jv);
			
			TEST(jv.getType() == JsonValue::EType::Object);
			



		}
	}
};

class TokenizerTests {
	static void _getNumber(const char* v) {
		Tokenizer t(v);
		auto b = t.nextToken();
		TEST(b);
		
		if (!TEST(t.isNumber(v))) {
			printf("  [FAIL_VAL] v = %s\n", v);
		}
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

