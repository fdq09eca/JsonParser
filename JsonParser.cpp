// JsonParser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "JsonParser.h"
#include "Tokenizer.h"

namespace CL
{

class ParserTests
{
public:
	static void test_parseObject() {

		{
			const char* json = "{ \"key1\" : 123 }";
			JsonParser p(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Object);
			JsonObject* obj = o->getObject();
			TEST(obj != nullptr);
			TEST(obj->size() == 1);
			TEST(obj->find("key1") != obj->end()); // key1 exists
			auto* v = obj->at("key1");
			TEST(v->getType() == JsonValue::EType::Number);
			TEST(v->getNumber() == 123);
		}

		{
			const char* json = "{ \"key1\" : 123, \"key2\" : \"Hello, World!\" }";
			JsonParser p(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Object);
			JsonObject* obj = o->getObject();
			TEST(obj != nullptr);
			TEST(obj->size() == 2);
			TEST(obj->find("key1") != obj->end()); // key1 exists
			auto* v = obj->at("key1");
			TEST(v->getType() == JsonValue::EType::Number);
			TEST(v->getNumber() == 123);

			TEST(obj->find("key2") != obj->end()); // key2 exists
			v = obj->at("key2");
			TEST(v->getType() == JsonValue::EType::String);
			TEST(*v->getString() == "Hello, World!");
		}
	};

	static void test_parseArray() {
		const char* json = "[null, true, false, 123, 123.456, \"Hello, World!\", [0, -2.1, 3e4, [1, 2, 3]]]";
		JsonParser p(json);
		auto* o = p.parseValue();
		TEST(o->isNull() == false);
		TEST(o->getType() == JsonValue::EType::Array);
		JsonArray* vec = o->getArray();
		TEST(vec != nullptr);
		TEST(vec->size() == 7);



		TEST(vec->at(0)->getType() == JsonValue::EType::Null);
		TEST(vec->at(0)->isNull() == true);

		TEST(vec->at(1)->getType() == JsonValue::EType::Boolean);
		TEST(vec->at(1)->getBoolean() == true);

		TEST(vec->at(2)->getType() == JsonValue::EType::Boolean);
		TEST(vec->at(2)->getBoolean() == false);

		TEST(vec->at(3)->getType() == JsonValue::EType::Number);
		TEST(vec->at(3)->getNumber() == 123);

		TEST(vec->at(4)->getType() == JsonValue::EType::Number);
		TEST(vec->at(4)->getNumber() == 123.456);

		TEST(vec->at(5)->getType() == JsonValue::EType::String);
		const auto& s = *(vec->at(5)->getString());
		TEST(s == "Hello, World!");


		TEST(vec->at(6)->getType() == JsonValue::EType::Array);
		auto* vec2 = vec->at(6)->getArray();
		TEST(vec2 != nullptr);
		TEST(vec2->size() == 4);

		TEST(vec2->at(0)->getType() == JsonValue::EType::Number);
		TEST(vec2->at(0)->getNumber() == 0);

		TEST(vec2->at(1)->getType() == JsonValue::EType::Number);
		TEST(vec2->at(1)->getNumber() == -2.1);

		TEST(vec2->at(2)->getType() == JsonValue::EType::Number);
		TEST(vec2->at(2)->getNumber() == 3e4);

		TEST(vec2->at(3)->getType() == JsonValue::EType::Array);
		auto* vec3 = vec2->at(3)->getArray();
		TEST(vec3->size() == 3);

		TEST(vec3->at(0)->getType() == JsonValue::EType::Number);
		TEST(vec3->at(0)->getNumber() == 1);

		TEST(vec3->at(1)->getType() == JsonValue::EType::Number);
		TEST(vec3->at(1)->getNumber() == 2);

		TEST(vec3->at(2)->getType() == JsonValue::EType::Number);
		TEST(vec3->at(2)->getNumber() == 3);
	}

	static void test_parseNull()
	{
		const char* json = "null";
		JsonParser p(json);
		auto* o = p.parseValue();
		TEST(o->isNull() == true);
		TEST(o->getType() == JsonValue::EType::Null);
	}

	static void test_parseBoolean()
	{
		const char* json = "true";
		JsonParser p(json);
		auto* o = p.parseValue();
		TEST(o->isNull() == false);
		TEST(o->getType() == JsonValue::EType::Boolean);
		TEST(o->getBoolean() == true);

		json = "false";
		p = JsonParser(json);
		o = p.parseValue();
		TEST(o->isNull() == false);
		TEST(o->getType() == JsonValue::EType::Boolean);
		TEST(o->getBoolean() == false);
	}

	static void test_parseNumber()
	{
		{
			const char* json = "0";
			JsonParser p(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Number);
			TEST(o->getNumber() == 0);
		}


		{
			const char* json = "0.05";
			JsonParser p(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Number);
			TEST(o->getNumber() == 0.05);
		}

		{
			const char* json = "-0.05";
			JsonParser p(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Number);
			TEST(o->getNumber() == -0.05);
		}

		{
			const char* json = "123";
			JsonParser p(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Number);
			TEST(o->getNumber() == 123);
		}

		{
			const char* json = "-123";
			auto p = JsonParser(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Number);
			TEST(o->getNumber() == -123);
		}

		{
			const char* json = "123.456";
			auto p = JsonParser(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Number);
			TEST(o->getNumber() == 123.456);
		}

		{
			const char* json = "-123.456";
			auto p = JsonParser(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Number);
			TEST(o->getNumber() == -123.456);
		}

		{
			const char* json = "123.456e123";
			auto p = JsonParser(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Number);
			TEST(o->getNumber() == 123.456e123);
		}

		{
			const char* json = "123.456e+123";
			auto p = JsonParser(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Number);
			TEST(o->getNumber() == 123.456e+123);
		}

		{
			const char* json = "123.456e-123";
			auto p = JsonParser(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Number);
			TEST(o->getNumber() == 123.456e-123);
		}

	}
}; // class ParserTests

class TokenizerTests {
	static void _getNumber(const char* v) {
		Tokenizer t(v);
		auto* p = t.getNumber();
		TEST(p->type == Token::Type::Number);
		TEST(p->value == v);
	}

public:
	static void test_getNextToken() {
		Tokenizer t("{ \"key1\" : [null, true, false, 123] }");

		auto* p = t.getNextToken();
		TEST(p->type == CL::Token::Type::OpenBrace);
		TEST(p->value == "{");

		p = t.getNextToken();
		TEST(p->type == CL::Token::Type::String);
		TEST(p->value == "key1");

		p = t.getNextToken();
		TEST(p->type == CL::Token::Type::Colon);
		TEST(p->value == ":");

		p = t.getNextToken();
		TEST(p->type == CL::Token::Type::OpenBracket);
		TEST(p->value == "[");

		p = t.getNextToken();
		TEST(p->type == CL::Token::Type::Null);
		TEST(p->value == "null");

		p = t.getNextToken();
		TEST(p->type == CL::Token::Type::Comma);
		TEST(p->value == ",");

		p = t.getNextToken();
		TEST(p->type == CL::Token::Type::Boolean);
		TEST(p->value == "true");

		p = t.getNextToken();
		TEST(p->type == CL::Token::Type::Comma);
		TEST(p->value == ",");

		p = t.getNextToken();
		TEST(p->type == CL::Token::Type::Boolean);
		TEST(p->value == "false");

		p = t.getNextToken();
		TEST(p->type == CL::Token::Type::Comma);
		TEST(p->value == ",");

		p = t.getNextToken();
		TEST(p->type == CL::Token::Type::Number);
		TEST(p->value == "123");

		p = t.getNextToken();
		TEST(p->type == CL::Token::Type::CloseBracket);
		TEST(p->value == "]");

		p = t.getNextToken();
		TEST(p->type == CL::Token::Type::CloseBrace);
		TEST(p->value == "}");

		p = t.getNextToken();
		TEST(p == nullptr);
		TEST(t.isEnd() == true);
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


}// namespace CL

int main()
{

	/* TokenizerTests */

	// CL::TokenizerTests::test_getNumber();
	// CL::TokenizerTests::test_getNextToken();

	/* ParserTests */

	// CL::ParserTests::test_parseNumber();
	// CL::ParserTests::test_parseBoolean();
	// CL::ParserTests::test_parseNull();

	CL::ParserTests::test_parseArray();
	//CL::ParserTests::test_parseObject();








	return 0;
}



