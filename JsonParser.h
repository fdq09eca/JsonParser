#pragma once

#include "Tokenizer.h"

namespace CL
{
class JsonValue;
using JsonString = String;
using JsonArray = Vector<JsonValue>;		
using JsonObject = Map<String, JsonValue>;	
using JsonNumber = f64;
using JsonBoolean = bool;

class JsonValue : NonCopyable {

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
	~JsonValue();;



	JsonValue(JsonValue&& rhs) {
		operator=(std::move(rhs));
	}

	JsonValue& operator=(JsonValue&& rhs) {
		if (this != &rhs) {
			setNull();
			_type = rhs._type;
			_value = rhs._value;

			rhs._type = EType::Undefined;
		}
		return *this;
	}


	EType getType() const;


	bool isNull() const;
	void setNull();

	bool isUndefined() const;
	void setUndefined();

	JsonNumber  getNumber() const;
	void		setNumber(JsonNumber n);

	bool		getBoolean() const;
	void		setBoolean(bool b);

	JsonObject* getObject() const;
	JsonObject* setToObject();
	void		setObject(JsonObject* obj);

	JsonArray* getArray() const;
	JsonArray* setToArray();
	void		setArray(JsonArray* arr);

	JsonString* getString() const;
	void		setString(const char* sz);

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
		currentToken = lexer->nextToken();
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