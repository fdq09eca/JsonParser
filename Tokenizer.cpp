#include "Tokenizer.h"
namespace CL {

bool Tokenizer::isEnd() const {
	MY_ASSERT(c != nullptr);
	return *c == '\0';
}
bool Tokenizer::peek(const char* cmp_str) const {
	MY_ASSERT(c != nullptr && cmp_str != nullptr);
	return 0 == strncmp(c, cmp_str, strlen(cmp_str));
}

void Tokenizer::advance(size_t n) {
	MY_ASSERT(c != nullptr && src != nullptr && dst != nullptr);

	if (c >= src && c < dst) {
		c += n;
		return;
	}
	throw MyError("Tokenizer::advance() out of range");
}

void Tokenizer::skipSpaces() {
	while (isspace(*c)) {
		advance();
	}
}

Token* Tokenizer::getNull() {
	if (peek("null")) {
		Token* t = new Token();
		t->type = Token::Type::Null;
		t->value = "null";
		advance(4);
		return t;
	}
	throw MyError("getNull failed");
}

Token* Tokenizer::getString() {
	advance();
	auto* p = c;

	while (c) {
		if (isEnd()) throw MyError("getString failed");


		if (*c == '"') {
			if (*(c - 1) != '\\') // not skip escaped character
			{
				auto* t = new Token(Token::Type::String, String(p, c - p));
				advance();
				return t;
			}
		}
		advance();
	}
}

Token* Tokenizer::getNextToken() {



	skipSpaces();

	if (*c == '-' || isdigit(*c)) {
		return getNumber();
	}


	switch (*c)
	{

	case '\0': return advanceAndGetToken(Token::Type::Eof);
	case '[': return advanceAndGetToken(Token::Type::OpenBracket);
	case ',': return advanceAndGetToken(Token::Type::Comma);
	case ']': return advanceAndGetToken(Token::Type::CloseBracket);

	case '{': return advanceAndGetToken(Token::Type::OpenBrace);
	case ':': return advanceAndGetToken(Token::Type::Colon);
	case '}': return advanceAndGetToken(Token::Type::CloseBrace);

	case 'n': return getNull();
	case '"': return getString();
	case 't': case 'f': return getBoolean();

	default: {

		printf("getNextToken failed: unknown Token %c", *c);
		throw MyError("getNextToken failed");
	} break;

	}

	return nullptr;
}

CL::Token* Tokenizer::advanceAndGetToken(Token::Type type, size_t n)
{
	if (!c) {
		throw MyError("advanceAndGetToken failed");
	}

	auto* t = new Token(type, { *c });

	if (*c != '\0')
		advance(n);
	return t;
}

CL::Token* Tokenizer::getNumber()
{
	if (c == nullptr || *c == '\0')
		return nullptr;

	auto* p = c;


	if (*c == '-') {
		advance();
		if (!isdigit(*c)) throw MyError("Invalid number.");

	}

	if (*c == '0') {
		advance();
		if (isdigit(*c)) throw MyError("Invalid number.");

	}
	else {
		if (!isdigit(*c)) throw MyError("Invalid number.");
		while (isdigit(*c)) {
			advance();
		}
	}

	if (*c == '.') {
		advance();
		if (!isdigit(*c)) throw MyError("Invalid number.");
		while (isdigit(*c)) {
			advance();
		}
	}

	if (*c == 'e' || *c == 'E') {
		advance();

		if (*c == '+' || *c == '-') advance();
		if (!isdigit(*c)) throw MyError("Invalid number.");

		while (isdigit(*c)) {
			advance();
		}
	}

	return new Token(Token::Type::Number, String(p, c - p));
}

Token* Tokenizer::getBoolean()
{
	const char* b = nullptr;

	if (peek("true"))
	{
		b = "true";
	}
	else if (peek("false"))
	{
		b = "false";
	}
	else
	{
		throw MyError("getBoolean failed.");
	}

	Token* t = new Token();
	t->type = Token::Type::Boolean;
	t->value = b;
	advance(t->value.length());

	return t;
}

} // namespace CL