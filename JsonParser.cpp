// JsonParser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "JsonParser.h"


namespace CL
{

JsonValue* JsonParser::parseValue() {
	
	switch (token().type)
	{
	case Token::Type::Eof:
		return nullptr;

	case Token::Type::Null: {
		JsonValue* v = new JsonValue();
		v->setNull();
		return v;
	}

	case Token::Type::Number: {
		double d{};
		lexer.readValue(d);

		JsonValue* v = new JsonValue();
		v->setNumber(d);

		return v;
	}

	case Token::Type::String: {
		JsonValue* v = new JsonValue();
		auto* s = v->setToString();
		lexer.readValue(*s);
		return v;
	} break;

	case Token::Type::Identifier: {

		if (lexer.isBool()) {
			JsonValue* v = new JsonValue();
			bool b = false;
			lexer.readValue(b);
			v->setBoolean(b);
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
	
	while (lexer.nextToken() && lexer.isOp(",") && lexer.nextToken()) { //bug here
		_addJsonArrayElement(arr);	
	}

	if (lexer.isOp("]")) {
		auto* v = new JsonValue();
		v->setArray(arr);
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
		auto* v = parseValue();
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
		if (obj == nullptr) 
			obj = new JsonObject();
		_addJsonObjectMember(obj);
	}
	
	while (lexer.nextToken() && lexer.isOp(",") && lexer.nextToken()) {
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



