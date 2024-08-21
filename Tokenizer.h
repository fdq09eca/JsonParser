#pragma once
#include "MyCommon.h"
namespace CL {

struct Token {
	enum class Type {
		Undefined,		 // undefined		[x]
		Null,			 // null			[x] None
		Identifier,      // id				[x] true, false
		Number,			 // 0 - 9			[x] 
		String,			 // "Hello, World!"	[x]
		Op,				 // [ ] { } : ,		[x]
		Eof,			 // end of file		[x]
	};

	Type type = Type::Undefined;
	String str;

	inline void print() const {
		printf("Token: (%d, %s)\n", type, str.c_str());
	}
};

class Tokenizer
{
	const char* _c = nullptr;
	const char* _src = nullptr;
	const char* _dst = nullptr;

	size_t		_lineNumber = 1;
	size_t		_columnNumber = 1;

	using Type = Token::Type;
	Token _token;

	void _reset() {
		_c = nullptr;
		_src = nullptr;
		_dst = nullptr;
	}

public:
	Tokenizer() = default;

	Tokenizer(const char* sz);

	~Tokenizer();

	const Token& token() const;;

	char nextChar();
	bool nextToken();


	bool isType(Type t) const;
	bool isEquals(Type t, const char* sz) const;

	bool isNull() const;

	bool isBool() const;
	bool isBool(const char* sz) const;

	bool isString() const;
	bool isString(const char* sz) const;

	bool isNumber() const;
	bool isNumber(const char* sz) const;

	bool isOp() const;
	bool isOp(const char* sz) const;

	bool isIdentifier() const;
	bool isIdentifier(const char* sz) const;

	void readValue(double& outValue);
	void readValue(String& outValue);
	void readValue(bool& outValue);

	bool isEnd() const;

	void skipSpaces();
};



} // namespace CL