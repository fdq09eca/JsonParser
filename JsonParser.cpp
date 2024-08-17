// JsonParser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//#include "JsonParser.h"
#include "Tokenizer.h"




int main()
{
	CL::Tokenizer t("{ \"key1\" : [null, true, false, 123] }");

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
	
	return 0;
}



