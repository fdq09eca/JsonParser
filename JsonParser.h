#pragma once

#include "Tokenizer.h"
#include "JsonValue.h"

namespace CL
{


class JsonParser {

private:
	using Lexer = Tokenizer;

	Lexer _lexer;
	
	void _expectOp(const char* op);
	bool _matchOp(const char* op);	
	bool _isNull() const; 
	bool _isBool() const;
	bool _isBool(const char* sz) const; 

public:
	inline JsonParser(const char* sz) : _lexer(sz) { _lexer.nextToken(); }

	inline const Token& token() const	{ return _lexer.token(); }
	inline bool nextToken()				{ return _lexer.nextToken(); }

	void parseArray(JsonValue& v);
	void parseObject(JsonValue& v);
	void parseValue(JsonValue& v);

	void readValue(double& outValue); 
	void readValue(String& outValue); 
	void readValue(bool& outValue);	  

}; // class JsonParser


} // namespace CL