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
		lexer.readValue(d);
		v.setNumber(d);
		return;
	}

	case Token::Type::String: {
		auto* s = v.setToString();
		lexer.readValue(*s);
		return;
	} 

	case Token::Type::Identifier: {

		if (lexer.isBool()) {
			bool b = false;
			lexer.readValue(b);
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

bool JsonParser::tryGetOp(const char* op) {
	if (lexer.isOp(op)) {
		lexer.nextToken();
		return true;
	}
	return false;
}

void JsonParser::parseArray(JsonValue& v) {
	expectOp("[");
	auto& arr = *v.setToArray();

	if (tryGetOp("]")) return;

	while (true) {
		
		arr.emplace_back();
		auto& e = arr.back();
		parseValue(e);
		
		
		if (tryGetOp(",")) continue;
		if (tryGetOp("]")) break;
		throw MyError("parseArray failed");
	}
}

void JsonParser::parseObject(JsonValue& v) {
	
	expectOp("{");
	auto& obj = *v.setToObject();
	
	if (tryGetOp("}")) return;

	while (true) {

		String memberName;
		lexer.readValue(memberName);
		if (memberName.empty()) throw MyError("parseObject failed");

		
		auto& val = obj[memberName];
		
		expectOp(":");
		parseValue(val);

		if (tryGetOp(",")) continue;
		if (tryGetOp("}")) break;
		throw MyError("parseObject failed");
		
	}
}
} // namespace CL



