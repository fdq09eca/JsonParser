#pragma once
#include "MyCommon.h"
namespace CL {

struct Token {
	enum class Type {
		Null,			 // null				[x] None
		Identifier,      // id				[x]
		Number,			 // 0 - 9			[x] 
		String,			 // "Hello, World!"	[x]
		Op,
		Eof,		     // EOF				[x]
	};

	Type type = Type::Null;
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

	Token _token;

	void _reset() {
		_c = nullptr;
		_src = nullptr;
		_dst = nullptr;
	}

public:
	Tokenizer() = default;

	Tokenizer(const char* sz) : _c(sz), _src(sz) {
		if (_c) {
			_dst = _c + strlen(_c);
		}
	}

	~Tokenizer() {
		_reset();
	}

	char nextChar();

	bool nextToken();

	using Type = Token::Type;

	bool isEquals(Type t, const char* sz) const {
		return _token.type == t && 0 == strcmp(_token.str.c_str(), sz);
	}

	bool isNumber() const { return _token.type == Token::Type::Number; };

	bool isOp(const char* sz) const {
		return isEquals(Type::Op, sz);
	}

	bool isIdentifier(const char* sz) const {
		return isEquals(Type::Identifier, sz);
	}

	bool isString(const char* sz) const {
		return isEquals(Type::String, sz);
	}


	void readValue(int& outValue) {
		if (!isNumber()) {
			throw MyError("readValue failed");
		}

		if (_token.str.empty()) {
			throw MyError("readValue failed");
		}

		if (1 != sscanf(_token.str.c_str(), "%d", &outValue)) {
			throw MyError("readValue failed");
		}

		nextToken();
	};




	bool isEnd() const;

	CL::Token* advanceAndGetToken(Token::Type type, size_t n = 1);

	void skipSpaces();

};


} // namespace CL