#pragma once

#include "Tokenizer.h"
#include "JsonValue.h"

namespace CL
{


class JsonParser {

private:
	using Lexer = Tokenizer;

	Lexer lexer;

	void _addJsonObjectMember(JsonObject* obj);

	void _addJsonArrayElement(JsonArray* arr);

public:
	inline JsonParser(const char* sz) : lexer(sz) {
		lexer.nextToken();
	}

	inline const Token& token() const {
		return lexer.token();
	}

	inline bool nextToken() { return lexer.nextToken(); }

	JsonValue* parseArray(JsonArray* arr = nullptr);

	JsonValue* parseObject(JsonObject* obj = nullptr);

	JsonValue* parseValue();
}; // class JsonParser


} // namespace CL