// JsonParser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "JsonParser.h"


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


#endif






JsonValue* JsonParser::parseValue() {
	
	switch (token().type)
	{
	case Token::Type::Eof:
		return nullptr;

	case Token::Type::Null: {
		JsonValue* v = new JsonValue();
		v->setNull();
		nextToken();
		return v;
	}

	case Token::Type::Number: {
		double d{};
		lexer.readValue(d);

		JsonValue* v = new JsonValue();
		v->setNumber(d);
		nextToken();

		return v;
	}

	case Token::Type::String: {
		JsonValue* v = new JsonValue();
		auto* s = v->setToString();
		lexer.readValue(*s);
		nextToken();

		return v;
	} break;

	case Token::Type::Identifier: {

		if (lexer.isBool()) {
			JsonValue* v = new JsonValue();
			bool b = false;
			lexer.readValue(b);
			v->setBoolean(b);
			nextToken();
			return v;
		}

	} break;

	case Token::Type::Op: {
		if		(lexer.isOp("[")) return parseArray();
		else if (lexer.isOp("{")) return parseObject();
	}

	default:
		printf("parseValue failed: unknown Token %d, %s", token().type, token().str.c_str());
		throw MyError("[ERR] parseValue failed");
	}

	return nullptr;
}

void JsonParser::_addJsonArrayElement(JsonArray* arr) {
	auto* e = parseValue();
	if (!e) throw MyError("parseArray failed");
	arr->emplace_back(std::move(*e)); // JsonValue copy!!
}

JsonValue* JsonParser::parseArray(JsonArray* arr) {
	if (lexer.isOp("[") && lexer.nextToken()) {
		if (arr == nullptr) {
			arr = new JsonArray();
		}
		_addJsonArrayElement(arr);
	}
	else {
		throw MyError("parseArray failed");
	}
	
	while (lexer.nextToken() && lexer.isOp(",")) {
		_addJsonArrayElement(arr);	
	}

	if (lexer.isOp("]")) {
		auto* v = new JsonValue();
		v->setArray(arr);
		lexer.nextToken();
		return v;
	}
	else 
	{
		throw MyError("parseArray failed");
	}
}

void JsonParser::_addJsonObjectMember(JsonObject* obj) {
	if (!lexer.isString()) throw MyError("parseObject failed");
	String key;
	lexer.readValue(key);
	
	if (lexer.nextToken() && lexer.isOp(":")) {
		lexer.nextToken();
		auto v = parseValue();
		if (!v) throw MyError("parseObject failed");
		obj->emplace(key, std::move(*v));
	}
	else 
	{
		throw MyError("parseObject failed");
	}
}

JsonValue* JsonParser::parseObject(JsonObject* obj) {
	if (lexer.isOp("{") && lexer.nextToken() && lexer.isString()) {
		_addJsonObjectMember(obj);
	}
	
	while (lexer.nextToken() && lexer.isOp(",")) {
		_addJsonObjectMember(obj);
	}

	if (lexer.isOp("}")) {
		auto* v = new JsonValue();
		v->setObject(obj);
		return v;
	} 
	else 
	{
		throw MyError("parseObject failed");
	}

	return nullptr;
}
} // namespace CL



