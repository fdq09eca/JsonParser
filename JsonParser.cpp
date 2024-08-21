// JsonParser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "JsonParser.h"
#include "Tokenizer.h"

namespace CL
{


#if 0



void JsonParser::_addJsonObjectMember(JsonObject* obj) {
	auto key = currentToken->value;
	if (!tryAdvance(Token::Type::String))	throw MyError("parseObject failed");
	if (!tryAdvance(Token::Type::Colon))	throw MyError("parseObject failed");
	auto* v = parseValue();
	if (!v) {
		throw MyError("parseObject failed");
	}
	obj->emplace(key, v);
}

void JsonParser::_addJsonArrayElement(JsonArray* arr) {
	auto* e = parseValue();
	if (!e) throw MyError("parseArray failed");
	arr->emplace_back(e);
}

bool JsonParser::tryAdvance(Token::Type expected_type) {
	if (currentToken->type != expected_type)
		return false;
	//delete currentToken; // someone still holding its reference


	currentToken = lexer->nextToken();
	return true;
}

JsonValue* JsonParser::parseArray(JsonArray* arr) {
	auto b = tryAdvance(Token::Type::OpenBracket);
	MY_ASSERT(b);

	if (arr == nullptr) {
		arr = new JsonArray();
	}

	_addJsonArrayElement(arr);


	while (tryAdvance(Token::Type::Comma)) {
		_addJsonArrayElement(arr);
	}

	if (tryAdvance(Token::Type::CloseBracket)) {
		auto* v = new JsonValue();
		v->setArray(arr);
		return v;
	}
}

JsonValue* JsonParser::parseObject(JsonObject* obj) {
	auto b = tryAdvance(Token::Type::OpenBrace);
	MY_ASSERT(b);

	if (obj == nullptr) {
		obj = new JsonObject();
	}

	_addJsonObjectMember(obj);

	while (tryAdvance(Token::Type::Comma)) {
		_addJsonObjectMember(obj);
	}

	if (tryAdvance(Token::Type::CloseBrace)) {
		auto* v = new JsonValue();
		v->setObject(obj);
		return v;
	}
}

JsonValue* JsonParser::parseValue() {

	switch (currentToken->type)
	{
	case Token::Type::Eof:
		return nullptr;

	case Token::Type::Null: {
		JsonValue* v = new JsonValue();
		auto b = tryAdvance(Token::Type::Null);
		MY_ASSERT(b);
		v->setNull();
		return v;
	}

	case Token::Type::Number: {
		const auto& tv = currentToken->value;
		MY_ASSERT(tryAdvance(Token::Type::Number));
		JsonValue* v = new JsonValue();
		v->setNumber(std::stod(tv));
		return v;
	}

	case Token::Type::String: {

		const auto& tv = currentToken->value;
		MY_ASSERT(tryAdvance(Token::Type::String));

		JsonValue* v = new JsonValue();
		v->setString(tv.c_str());
		return v;
	} break;

	case Token::Type::Boolean: {
		const auto& tv = currentToken->value;
		MY_ASSERT(tryAdvance(Token::Type::Boolean));
		JsonValue* v = new JsonValue();
		v->setBoolean(tv == "true");
		return v;
	} break;

	case Token::Type::OpenBrace: {
		return parseObject();
	} break;

	case Token::Type::OpenBracket: {
		return parseArray();
	}

	default:
		printf("parseValue failed: unknown Token %d, %s", currentToken->type, currentToken->value.c_str());
		throw MyError("[ERR] parseValue failed");
	}

	return nullptr;
}
#endif

} // namespace CL



