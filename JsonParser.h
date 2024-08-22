#pragma once

#include "Tokenizer.h"
#include "JsonValue.h"

namespace CL
{


class JsonParser {

private:
	using Lexer = Tokenizer;

	Lexer lexer;
	
	void expectOp(const char* op);

	bool tryGetOp(const char* op);	

public:
	inline JsonParser(const char* sz) : lexer(sz) {
		lexer.nextToken();
	}

	inline const Token& token() const {
		return lexer.token();
	}

	inline bool nextToken() { return lexer.nextToken(); }

	void parseArray(JsonValue& v);

	void parseObject(JsonValue& v);

	void parseValue(JsonValue& v);
}; // class JsonParser


} // namespace CL