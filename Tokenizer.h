#pragma once
#include "MyCommon.h"
namespace CL {

struct Token {
	enum class Type {
		Null,			// null				[x]
		Boolean,		// true, false		[x]
		Number,			// 0 - 9			[x]
		String,			// "Hello, World!"	[x]
		Comma,			// ,				[x]
		Colon,			// :				[x]
		OpenBrace,		// [				[x]
		CloseBrace,		// ]				[x]
		OpenBracket,	// {				[x]
		CloseBracket,	// }				[x]
		Eof,		    // EOF				[ ]
	};

	Type type = Type::Null;
	String value{};

	Token() = default;
	Token(Type t, const String& v) : type(t), value(v) {}

	inline void print() const {
		printf("Token: (%d, %s)\n", type, value.c_str());
	}
};

class Tokenizer
{
	const char* c = nullptr;
	const char* src = nullptr;
	const char* dst = nullptr;

	void _reset() {
		c = nullptr;
		src = nullptr;
		dst = nullptr;
	}

public:
	Tokenizer() = default;

	Tokenizer(const char* sz) : c(sz), src(sz) {
		dst = c + strlen(c);
	}

	~Tokenizer() {
		_reset();
	}

	Token* getNextToken();


	Token* getNull();
	Token* getNumber();
	Token* getString();
	Token* getBoolean();

	bool isEnd() const;

	bool peek(const char* cmp_str) const;

	void advance(size_t n = 1);
	CL::Token* advanceAndGetToken(Token::Type type, size_t n = 1);

	void skipSpaces();

};


} // namespace CL