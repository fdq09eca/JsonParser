#include <iostream>
#include "JsonParser.h"


namespace CL
{

bool JsonParser::_isNull() const
{
	return _lexer.isEquals(Token::Type::Identifier, "null");
}

bool JsonParser::_isBool() const
{
	return _lexer.isIdentifier("true") || _lexer.isIdentifier("false");
}

bool JsonParser::_isBool(const char* sz) const
{
	return _isBool() && 0 == strcmp(token().str.c_str(), sz);
}



void JsonParser::parseValue(JsonValue& v) {
	
	switch (token().type)
	{
	case Token::Type::Eof:
		return ;

	

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

		if (_isBool()) {
			bool b = false;
			readValue(b);
			v.setBoolean(b);
			return;
		}
		
		if (_isNull()) {
			v.setNull();
			nextToken();
			return;
		}
	} 

	case Token::Type::Op: {
		if		(_lexer.isOp("[")) parseArray(v);
		else if (_lexer.isOp("{")) parseObject(v);
		else throw MyError("unknown Op");
		return;
	}

	default:
		printf("parseValue failed: unknown Token %d, %s", token().type, token().str.c_str());
		throw MyError("[ERR] parseValue failed");
	}
}





void JsonParser::_expectOp(const char* op) {
		

	if (!_lexer.isOp(op)) {
		String errMsg = "Error: : Unexpected token[" + token().str + "]\n-------------- - \n";
		
		//lines = _lexer.getLine();






		// Error: : Unexpected token[=]
		// -------------- -
		// 1 : #if ShaderInfo
		// 2 : Properties{
		// 3:   Color4f         color = (0, 0, 0.5, 1)
		// 4 : f32                     shininess = 0.5 // = global.shininess
		// 5 : Texture2D       tex0 "Texture0" =
		// 
		// 									  ^ --

		throw MyError("expectOp failed");
	}

	_lexer.nextToken();
}

bool JsonParser::_matchOp(const char* op) {
	if (_lexer.isOp(op)) {
		_lexer.nextToken();
		return true;
	}
	return false;
}

void JsonParser::parseArray(JsonValue& v) {
	_expectOp("[");
	auto& arr = *v.setToArray();

	if (_matchOp("]")) return;

	while (true) {
		
		arr.emplace_back();
		auto& e = arr.back();
		parseValue(e);
		
		
		if (_matchOp(",")) continue;
		if (_matchOp("]")) break;
		throw MyError("parseArray failed");
	}
}

void JsonParser::parseObject(JsonValue& v) {
	
	_expectOp("{");
	auto& obj = *v.setToObject();
	
	if (_matchOp("}")) return;

	while (true) {

		String memberName;
		readValue(memberName);
		if (memberName.empty()) throw MyError("parseObject failed");

		
		auto& val = obj[memberName];
		
		_expectOp(":");
		parseValue(val);

		if (_matchOp(",")) continue;
		if (_matchOp("}")) break;
		throw MyError("parseObject failed");
		
	}
}

void JsonParser::readValue(double& outValue) {

	if (!_lexer.isNumber())		throw MyError("readValue(double) failed");
	if (token().str.empty()) throw MyError("readValue(double) failed");


	if (1 != sscanf_s(token().str.c_str(), "%lf", &outValue))
		throw MyError("readValue failed");
	nextToken();
}

void JsonParser::readValue(String& outValue) {

	if (!_lexer.isType(Token::Type::String))	throw MyError("readValue(String) failed");


	outValue = token().str;
	nextToken();
}

void JsonParser::readValue(bool& outValue) {

	if (_lexer.isIdentifier("true"))
	{
		outValue = true;
		nextToken();
	}
	else if (_lexer.isIdentifier("false"))
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



