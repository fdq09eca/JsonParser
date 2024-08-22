#include <iostream>
#include "JsonParser.h"


namespace CL
{

void JsonParser::parseValue(JsonValue& v) {
	
	switch (token().type)
	{
	case Token::Type::Eof:
		return ;

	case Token::Type::Null: {
		v.setNull();
		nextToken();
		return;
	} 

	case Token::Type::Number: {
		double d{};
		readValue(d);
		v.setNumber(d);
		return;
	}

	case Token::Type::String: {
		auto* s = v.setToString();
		readValue(*s);
		return;
	} 

	case Token::Type::Identifier: {

		if (lexer.isBool()) {
			bool b = false;
			readValue(b);
			v.setBoolean(b);
			return;
		}

	} 

	case Token::Type::Op: {
		if		(lexer.isOp("[")) parseArray(v);
		else if (lexer.isOp("{")) parseObject(v);
		else throw MyError("unknown Op");
		return;
	}

	default:
		printf("parseValue failed: unknown Token %d, %s", token().type, token().str.c_str());
		throw MyError("[ERR] parseValue failed");
	}
}





void JsonParser::expectOp(const char* op) {
		
// Error: : Unexpected token[=]
// -------------- -
// 1 : #if ShaderInfo
// 2 : Properties{
// 3:   Color4f         color = (0, 0, 0.5, 1)
// 4 : f32                     shininess = 0.5 // = global.shininess
// 5 : Texture2D       tex0 "Texture0" =
// 
// 									  ^ --

		if (!lexer.isOp(op)) throw MyError("expectOp failed");
		lexer.nextToken();
}

bool JsonParser::matchOp(const char* op) {
	if (lexer.isOp(op)) {
		lexer.nextToken();
		return true;
	}
	return false;
}

void JsonParser::parseArray(JsonValue& v) {
	expectOp("[");
	auto& arr = *v.setToArray();

	if (matchOp("]")) return;

	while (true) {
		
		arr.emplace_back();
		auto& e = arr.back();
		parseValue(e);
		
		
		if (matchOp(",")) continue;
		if (matchOp("]")) break;
		throw MyError("parseArray failed");
	}
}

void JsonParser::parseObject(JsonValue& v) {
	
	expectOp("{");
	auto& obj = *v.setToObject();
	
	if (matchOp("}")) return;

	while (true) {

		String memberName;
		readValue(memberName);
		if (memberName.empty()) throw MyError("parseObject failed");

		
		auto& val = obj[memberName];
		
		expectOp(":");
		parseValue(val);

		if (matchOp(",")) continue;
		if (matchOp("}")) break;
		throw MyError("parseObject failed");
		
	}
}

void JsonParser::readValue(double& outValue) {

	if (!lexer.isNumber())		throw MyError("readValue(double) failed");
	if (token().str.empty()) throw MyError("readValue(double) failed");


	if (1 != sscanf_s(token().str.c_str(), "%lf", &outValue))
		throw MyError("readValue failed");
	nextToken();
}

void JsonParser::readValue(String& outValue) {

	if (!lexer.isType(Token::Type::String))	throw MyError("readValue(String) failed");


	outValue = token().str;
	nextToken();
}

void JsonParser::readValue(bool& outValue) {

	if (lexer.isIdentifier("true"))
	{
		outValue = true;
		nextToken();
	}
	else if (lexer.isIdentifier("false"))
	{
		outValue = false;
		nextToken();
	}
	else
	{
		throw MyError("readValue(bool) failed");
	}
}
} // namespace CL



