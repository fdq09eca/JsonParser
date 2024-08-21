#include "Tokenizer.h"
namespace CL {

bool Tokenizer::isEnd() const {
	MY_ASSERT(_c != nullptr);
	return (*_c == '\0');
}

Tokenizer::Tokenizer(const char* sz) : _c(sz), _src(sz) {
	if (_c) {
		_dst = _c + strlen(_c);
	}
	else {
		throw MyError("Tokenizer(const char* sz)");
	}
}

Tokenizer::~Tokenizer() {
	_reset();
}

const Token& Tokenizer::token() const {
	return _token;
}

char Tokenizer::nextChar() {
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

void Tokenizer::skipSpaces() {
	while (!isEnd() && isspace(*_c)) {
		nextChar();
	}
}

bool Tokenizer::nextToken() {

	skipSpaces();

	// number
	if (*_c == '-' || isdigit(*_c)) {

		auto* p = _c;

		if (*_c == '-') {
			nextChar();
			if (!isdigit(*_c))
				throw MyError("[ERR] Tokenizer::nextToken() Invalid number.");
		}

		if (*_c == '0') {
			nextChar();
			if (isdigit(*_c))
				throw MyError("[ERR] Tokenizer::nextToken() Invalid number.");
		}

		else
		{
			if (!isdigit(*_c))
				throw MyError("[ERR] Tokenizer::nextToken() Invalid number.");

			while (isdigit(*_c)) {
				nextChar();
			}
		}

		if (*_c == '.') {
			nextChar();
			if (!isdigit(*_c))
				throw MyError("[ERR] Tokenizer::nextToken() Invalid number.");

			while (isdigit(*_c)) {
				nextChar();
			}
		}

		if (*_c == 'e' || *_c == 'E') {

			nextChar();
			if (*_c == '+' || *_c == '-')
				nextChar();

			if (!isdigit(*_c)) throw MyError("[ERR] Tokenizer::nextToken() Invalid number.");

			while (isdigit(*_c)) {
				nextChar();
			}
		}

		_token.type = Token::Type::Number;
		_token.str.assign(p, _c - p);

		return true;
	}


	switch (*_c)
	{

	case '\0': return false;

	case '[': case ',': case ']':
	case '{': case ':': case '}':
	{
		_token.type = Token::Type::Op;
		_token.str = *_c;
		nextChar();
		return true;
	}

	case '"': {
		nextChar(); // skip open '"'

		auto* p = _c;

		while (true) {

			if (isEnd()) throw MyError("getString failed");

			if (*_c == '"' && *(_c - 1) != '\\') {

				_token.type = Token::Type::String;
				_token.str.assign(p, _c - p);
				nextChar(); // skip close '"'
				return true;
			}

			nextChar();
		}
	}


	case 'n': {
		if (0 == strncmp(_c, "null", 4)) {
			_token.type = Token::Type::Null;
			_token.str = "null";
			for (int i = 0; i < 4; i++) nextChar();
			return true;
		}
		else {
			throw MyError("[ERR] unknown Identifier");
			return false;
		}
	}

	case 't':
	case 'f': {
		if (0 == strncmp(_c, "true", 4)) {
			_token.type = Token::Type::Identifier;
			_token.str = "true";
			for (int i = 0; i < 4; i++) nextChar();
			return true;
		}
		else if (0 == strncmp(_c, "false", 5)) {
			_token.type = Token::Type::Identifier;
			_token.str = "false";
			for (int i = 0; i < 5; i++) nextChar();
			return true;
		}
		else {
			throw MyError("[ERR] unknown Identifier");
			return false;
		}
	}

	default: {
		throw MyError("nextToken failed");
	}

	}

	return false;
}

bool Tokenizer::isEquals(Type t, const char* sz) const {
	return isType(t) && 0 == strcmp(_token.str.c_str(), sz);
}

bool Tokenizer::isNull() const
{
	return isEquals(Type::Null, "null");
}

bool Tokenizer::isBool() const
{
	return isIdentifier("true") || isIdentifier("false");
}

bool Tokenizer::isBool(const char* sz) const
{
	return isBool() && 0 == strcmp(_token.str.c_str(), sz);
}

bool Tokenizer::isType(Type t) const {
	return _token.type == t;
}

bool Tokenizer::isString() const
{
	return isType(Type::String);
}

bool Tokenizer::isString(const char* sz) const
{
	return isEquals(Type::String, sz);
}

bool Tokenizer::isNumber() const {
	return isType(Type::Number);
}

bool Tokenizer::isNumber(const char* sz) const {
	return isEquals(Type::Number, sz);
}

bool Tokenizer::isOp() const {
	return isType(Type::Op);
}

bool Tokenizer::isOp(const char* sz) const {
	return isEquals(Type::Op, sz);
}

bool Tokenizer::isIdentifier() const
{
	return isType(Type::Identifier);
}


bool Tokenizer::isIdentifier(const char* sz) const {
	return isEquals(Type::Identifier, sz);
}

void Tokenizer::readValue(double& outValue) {

	if (!isNumber())		throw MyError("readValue(double) failed");
	if (_token.str.empty()) throw MyError("readValue(double) failed");


	if (1 != sscanf_s(_token.str.c_str(), "%lf", &outValue))
		throw MyError("readValue failed");
}

void Tokenizer::readValue(String& outValue) {

	if (!isType(Type::String))	throw MyError("readValue(String) failed");
	if (_token.str.empty())		throw MyError("readValue(String) failed");

	outValue = _token.str;
}

void Tokenizer::readValue(bool& outValue) {

	if (isIdentifier("true"))		outValue = true;
	else if (isIdentifier("false")) outValue = false;
	else							throw MyError("readValue(bool) failed");
}

} // namespace CL