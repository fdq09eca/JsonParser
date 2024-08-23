#include <iostream>
#include "JsonParser.h"


namespace CL
{

bool JsonParser::_isNull() const
{
	return _lexer.isEquals(Token::Type::Identifier, "null");
}

bool JsonParser::_isBool() const
{
	return _lexer.isIdentifier("true") || _lexer.isIdentifier("false");
}

bool JsonParser::_isBool(const char* sz) const
{
	return _isBool() && 0 == strcmp(token().str.c_str(), sz);
}

void JsonParser::_getLines(Map<size_t, String>& outLines, size_t nLines) {

	size_t lineNumber = _lexer.lineNumber();
	const char* sz = _lexer.src();
	const char* p = _lexer.c();

	size_t getUntilLineNum = lineNumber > nLines ? lineNumber - nLines + 1 : 1;

	for (auto i = 0; i < lineNumber; i++) {
		auto n = lineNumber - i;
		if (n < getUntilLineNum) break;
		p = Util::getLine(sz, p, outLines[n]);
	}
}



void JsonParser::parseValue(JsonValue& v) {
	
	switch (token().type)
	{
	case Token::Type::Eof:
		return ;

	

	case Token::Type::Number: {
		double d{};
		readValue(d);
		v.setNumber(d);
		return;
	}

	case Token::Type::String: {
		auto* s = v.setToString();
		readValue(*s);
		return;
	} 

	case Token::Type::Identifier: {

		if (_isBool()) {
			bool b = false;
			readValue(b);
			v.setBoolean(b);
			return;
		}
		
		if (_isNull()) {
			v.setNull();
			nextToken();
			return;
		}
	} 

	case Token::Type::Op: {
		if		(_lexer.isOp("[")) parseArray(v);
		else if (_lexer.isOp("{")) parseObject(v);
		else throw MyError("unknown Op");
		return;
	}

	default:
		printf("parseValue failed: unknown Token %d, %s", token().type, token().str.c_str());
		throw MyError("[ERR] parseValue failed");
	}
}

void JsonParser::_expectOp(const char* op) {
		

	if (!_lexer.isOp(op)) throw _unExpectTokenError(op);
	_lexer.nextToken();
}

bool JsonParser::_matchOp(const char* op) {
	if (_lexer.isOp(op)) {
		_lexer.nextToken();
		return true;
	}
	return false;
}

MyError JsonParser::_unExpectTokenError(const char* expectedToken, size_t nLines) {
	Map<size_t, String> lines;
	_getLines(lines, nLines);

	size_t lineNumber = _lexer.lineNumber();
	auto* sz = _lexer.src();
	auto* _c = _lexer.c();

	auto eLineNum = lineNumber + 1;



	auto& eLine = lines[eLineNum];
	auto* p = Util::getLine(sz, _c, eLine);
	MY_ASSERT(p != nullptr);

	auto i = 0;

	while (true) {
		if (i == 0 && *p != eLine[0]) {
			p++;
			continue;
		}

		auto c = p + i;

		if (*c == '\0') {
			eLine[i] = '\n';
			break;
		}


		if (c  >  _c)	eLine[i] = '_';
		else if (c == _c)	eLine[i] = '^';
		else if (*c == '\t')	eLine[i] = '\t';
		else if (*c == '\n')	eLine[i] = '\n';
		else					eLine[i] = '_';

		i++;
	}

	String errMsg;
	size_t getUntilLineNum = lineNumber > nLines ? lineNumber - nLines + 1 : 1;
	size_t lastLineNum = eLineNum;


	errMsg += "Error: Unexpected token[" + token().str + "], " + String("expected [") + expectedToken;
	errMsg += "]\n";
	errMsg += "---------------------------";

	String line;
	for (auto n = getUntilLineNum; n <= lastLineNum; n++) {

		line.clear();
		if (n == lastLineNum) { // arrow Line
			// padding for line number
			auto npad = Util::ndigit(n) + strlen(": ");

			while (npad) {
				line += ' ';
				npad--;
			}

			line += lines[lastLineNum];
		}
		else {
			line += std::to_string(n) + ": " + lines[n];
		}
	}

	return MyError(errMsg);
}

void JsonParser::parseArray(JsonValue& v) {
	_expectOp("[");
	auto& arr = *v.setToArray();

	if (_matchOp("]")) return;

	while (true) {
		
		arr.emplace_back();
		auto& e = arr.back();
		parseValue(e);
		
		
		if (_matchOp(",")) continue;
		if (_matchOp("]")) break;
		throw _unExpectTokenError("] or ,");
	}
}

void JsonParser::parseObject(JsonValue& v) {
	
	_expectOp("{");
	auto& obj = *v.setToObject();
	
	if (_matchOp("}")) return;

	while (true) {

		String memberName;
		readValue(memberName);
		if (memberName.empty()) throw MyError("parseObject failed, memberName is empty");

		
		auto& val = obj[memberName];
		
		_expectOp(":");
		parseValue(val);

		if (_matchOp(",")) continue;
		if (_matchOp("}")) break;
		throw _unExpectTokenError("} or ,");
		
	}
}

void JsonParser::readValue(double& outValue) {

	if (!_lexer.isNumber())		throw MyError("readValue(double) failed");
	if (token().str.empty())	throw MyError("readValue(double) failed");


	if (1 != sscanf_s(token().str.c_str(), "%lf", &outValue))
		throw MyError("readValue failed");
	nextToken();
}

void JsonParser::readValue(String& outValue) {

	if (!_lexer.isType(Token::Type::String))	throw MyError("readValue(String) failed");


	outValue = token().str;
	nextToken();
}

void JsonParser::readValue(bool& outValue) {

	if (_lexer.isIdentifier("true"))
	{
		outValue = true;
		nextToken();
	}
	else if (_lexer.isIdentifier("false"))
	{
		outValue = false;
		nextToken();
	}
	else
	{
		throw MyError("readValue(bool) failed");
	}
}

} // namespace CL



