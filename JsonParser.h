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

	bool matchOp(const char* op);	

public:
	inline JsonParser(const char* sz) : lexer(sz) { lexer.nextToken(); }

	inline const Token& token() const { return lexer.token(); }

	inline bool nextToken() { return lexer.nextToken(); }

	void parseArray(JsonValue& v);

	void parseObject(JsonValue& v);

	void parseValue(JsonValue& v);

	void readValue(double& outValue); 
	void readValue(String& outValue); 
	void readValue(bool& outValue);	  

}; // class JsonParser


} // namespace CL