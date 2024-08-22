#include "Tokenizer.h"
namespace CL {



Tokenizer::Tokenizer(const char* sz) : _c(sz), _src(sz) {
	if (_c) 
		_dst = _c + strlen(_c);
	else 
		throw MyError("Tokenizer(const char* sz)");
}

Tokenizer::~Tokenizer() {
	_reset();
}

char Tokenizer::_nextChar(size_t n) {
	for (int i = 0; i < n; i++) _nextChar();
	return *_c;
}

char Tokenizer::_nextChar() {
	MY_ASSERT(_c != nullptr
		&& _src != nullptr
		&& _dst != nullptr
		&& _dst > _src);

	if (_c >= _src && _c < _dst) {
		_c++;

		if (*_c == '\n') {
			_lineNumber++;
			_columnNumber = 1;
		}
		else {
			_columnNumber++;
		}
		return *_c;
	}
	throw MyError("Tokenizer::nextChar() out of range");
}

void Tokenizer::_skipSpaces() {
	while (!isEnd() && isspace(*_c)) {
		_nextChar();
	}
}

bool Tokenizer::nextToken() {

	_skipSpaces();

	// number
	if (*_c == '-' || isdigit(*_c)) {

		auto* p = _c;

		if (*_c == '-') {
			_nextChar();
			if (!isdigit(*_c))
				throw MyError("[ERR] Tokenizer::nextToken() Invalid number.");
		}

		if (*_c == '0') {
			_nextChar();
			if (isdigit(*_c))
				throw MyError("[ERR] Tokenizer::nextToken() Invalid number.");
		}

		else
		{
			if (!isdigit(*_c))
				throw MyError("[ERR] Tokenizer::nextToken() Invalid number.");

			while (isdigit(*_c)) {
				_nextChar();
			}
		}

		if (*_c == '.') {
			_nextChar();
			if (!isdigit(*_c))
				throw MyError("[ERR] Tokenizer::nextToken() Invalid number.");

			while (isdigit(*_c)) {
				_nextChar();
			}
		}

		if (*_c == 'e' || *_c == 'E') {

			_nextChar();
			if (*_c == '+' || *_c == '-')
				_nextChar();

			if (!isdigit(*_c)) throw MyError("[ERR] Tokenizer::nextToken() Invalid number.");

			while (isdigit(*_c)) {
				_nextChar();
			}
		}

		_token.type = Token::Type::Number;
		_token.str.assign(p, _c - p);

		return true;
	}


	switch (*_c)
	{

	case '\0':
		_token.type = Token::Type::Eof;
		_token.str = *_c;
		return false;

	case '[': case ',': case ']':
	case '{': case ':': case '}':
	{
		_token.type = Token::Type::Op;
		_token.str = *_c;
		_nextChar();
		return true;
	}

	case '"': {
		_nextChar(); // skip open '"'

		auto* p = _c;

		while (true) {

			if (isEnd()) throw MyError("getString failed");

			if (*_c == '"' && *(_c - 1) != '\\') {

				_token.type = Token::Type::String;
				_token.str.assign(p, _c - p);
				_nextChar(); // skip close '"'
				return true;
			}

			_nextChar();
		}
	}


	case 'n': {
		if (0 == strncmp(_c, "null", 4)) {
			_token.type = Token::Type::Identifier;
			_token.str = "null";
			_nextChar(4);
			return true;
		}
		else {
			throw MyError("[ERR] unknown Identifier");
		}
	}

	case 't':
	case 'f': {
		if (0 == strncmp(_c, "true", 4)) {
			_token.type = Token::Type::Identifier;
			_token.str = "true";
			_nextChar(4);
			return true;
		}
		
		if (0 == strncmp(_c, "false", 5)) {
			_token.type = Token::Type::Identifier;
			_token.str = "false";
			_nextChar(5);
			return true;
		}

		throw MyError("[ERR] unknown Identifier");
		
	}

	default: throw MyError("nextToken failed"); 
	
	}
}

bool Tokenizer::isEnd()	const { 
	MY_ASSERT(_c != nullptr); 
	return (*_c == '\0'); 
}

bool Tokenizer::isEquals(Type t, const char* sz) const { 
	MY_ASSERT(sz != nullptr); 
	return isType(t) && 0 == strcmp(_token.str.c_str(), sz); 
}

bool Tokenizer::isType(Type t) const { return _token.type == t; }

bool Tokenizer::isOp()			const { return isType(Type::Op);			}
bool Tokenizer::isString()		const { return isType(Type::String);		}
bool Tokenizer::isNumber()		const { return isType(Type::Number);		}
bool Tokenizer::isIdentifier()	const { return isType(Type::Identifier);	}

bool Tokenizer::isOp(const char* sz)			const { return isEquals(Type::Op, sz);			}
bool Tokenizer::isString(const char* sz)		const { return isEquals(Type::String, sz);		}
bool Tokenizer::isIdentifier(const char* sz)	const { return isEquals(Type::Identifier, sz);	}

} // namespace CL