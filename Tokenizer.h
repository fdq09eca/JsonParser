#pragma once
#include "MyCommon.h"
namespace CL {

struct Token {
	enum class Type {
		Undefined,		 // undefined		[x]
		Identifier,      // id				[x] true, false, null
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
	char _nextChar();
	char _nextChar(size_t n);
	
	void _skipSpaces();

public:
	Tokenizer() = default;

	Tokenizer(const char* sz);

	~Tokenizer();


	inline const Token& token()			const { return _token;}
	inline const size_t lineNumber()	const { return _lineNumber; }
	inline const size_t columnNumber()	const { return _columnNumber; }


	bool nextToken();

	bool isEnd() const;

	bool isType(Type t) const;
	bool isEquals(Type t, const char* sz) const;

	bool isOp()			const;
	bool isString()		const;
	bool isNumber()		const;
	bool isIdentifier() const;

	bool isOp(const char* sz)			const;
	bool isString(const char* sz)		const;
	bool isIdentifier(const char* sz)	const;

};



} // namespace CL