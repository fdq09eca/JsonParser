#pragma once

#include "Tokenizer.h"

namespace CL
{
class JsonValue;
using JsonString = String;
using JsonArray = Vector<JsonValue*>;		// why not const ref?
using JsonObject = Map<String, JsonValue*>;	// why not const ref?
using JsonNumber = f64;
using JsonBoolean = bool;

class JsonValue {

public:
	enum class EType
	{
		Undefined = 0,
		Null,
		Number,
		Boolean,
		Array,
		String,
		Object,
	};

private:
	union Value
	{
		JsonNumber number;
		JsonBoolean boolean;
		JsonString* string;
		JsonArray* array;
		JsonObject* object;
	};

	EType _type = EType::Undefined;
	Value _value;


public:
	~JsonValue();

	EType getType() const;

	bool getBoolean() const;

	bool isNull() const;

	bool isUndefined() const;

	void setNull();

	JsonNumber getNumber() const;

	void setNumber(JsonNumber n);

	void setBoolean(JsonBoolean b);


	JsonObject* getObject() const;

	void setObject(JsonObject* obj);

	JsonObject* setToObject();

	JsonArray* getArray() const;

	void setArray(JsonArray* arr);

	JsonArray* setToArray();

	JsonString* getString() const;

	void setString(const char* sz);
}; // class JsonValue

class JsonParser {

private:
	using Lexer = Tokenizer;

	Lexer* lexer = nullptr;
	Token* currentToken = nullptr;

	void _addJsonObjectMember(JsonObject* obj);

	void _addJsonArrayElement(JsonArray* arr);

public:
	JsonParser(const char* sz) {
		lexer = new Lexer(sz);
		currentToken = lexer->getNextToken();
	}

	~JsonParser()
	{
		delete lexer;
		delete currentToken;
	}

	bool tryAdvance(Token::Type expected_type);

	JsonValue* parseArray(JsonArray* arr = nullptr);

	JsonValue* parseObject(JsonObject* obj = nullptr);

	JsonValue* parseValue();;
}; // class JsonParser


} // namespace CL