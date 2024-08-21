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
	JsonParser(const char* sz) : lexer(sz) {

	}

	const Token& token() const {
		return lexer.token();
	}

	bool tryAdvance(Token::Type expected_type);

	JsonValue* parseArray(JsonArray* arr = nullptr);

	JsonValue* parseObject(JsonObject* obj = nullptr);

	JsonValue* parseValue();
}; // class JsonParser


} // namespace CL