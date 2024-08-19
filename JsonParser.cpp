// JsonParser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//#include "JsonParser.h"
#include "Tokenizer.h"

namespace CL
{


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
};

} // namespace CL

int main()
{
	CL::TokenizerTests::test_getNumber();
	return 0;
}



