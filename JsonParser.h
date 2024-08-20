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
	~JsonValue()
	{
		setNull();
	}

	EType getType() const {
		return _type;
	}

	bool getBoolean() const {
		return _value.boolean;
	}

	bool isNull() const {
		return _type == EType::Null;
	}

	bool isUndefined() const {
		return _type == EType::Undefined;
	}

	void setNull() {
		switch (_type)
		{
		case JsonValue::EType::String:
			delete _value.string;
			break;
		case JsonValue::EType::Array:
			delete _value.array;
			break;
		case JsonValue::EType::Object:
			delete _value.object;
			break;
		default:
			break;
		}
		_type = EType::Null;
	}

	JsonNumber getNumber() const {
		if (_type != EType::Number)
			throw MyError("JsonValue::getNumber failed");
		return _value.number;
	}

	void setNumber(JsonNumber n) {
		if (_type != EType::Number) {
			setNull();
			_type = EType::Number;
		}
		_value.number = n;
	}

	void setBoolean(JsonBoolean b) {
		if (_type != EType::Boolean) {
			setNull();
			_type = EType::Boolean;
		}
		_value.boolean = b;
	}


	JsonObject* getObject() const {
		if (_type != EType::Object)
			return nullptr;
		return _value.object;
	}

	void setObject(JsonObject* obj) {
		setNull();
		_type = EType::Object;
		_value.object = obj;
	}

	JsonObject* setToObject() {
		auto* p = getObject();

		if (p) return p;

		setNull();
		_type = EType::Object;
		_value.object = new JsonObject();

		return _value.object;
	}

	JsonArray* getArray() const {
		if (_type != EType::Array)
			return nullptr;
		return _value.array;
	}

	void setArray(JsonArray* arr) {
		setNull();
		_type = EType::Array;
		_value.array = arr;
	}

	JsonArray* setToArray() {
		auto* p = getArray();
		if (p) return p;

		setNull();
		_type = EType::Array;
		_value.array = new JsonArray();

		return _value.array;
	}

	JsonString* getString() const {
		if (_type != EType::String)
			return nullptr;
		return _value.string;
	}

	void setString(const char* sz) {
		auto* p = getString();

		if (!p) {
			setNull();
			_type = EType::String;
			_value.string = new JsonString();
		}

		*_value.string = sz;
	}
};

class JsonParser {

private:
	using Lexer = Tokenizer;

	Lexer* lexer = nullptr;
	Token* currentToken = nullptr;

	void _addJsonObjectMember(JsonObject* obj) {
		auto key = currentToken->value;
		if (!tryAdvance(Token::Type::String))	throw MyError("parseObject failed");
		if (!tryAdvance(Token::Type::Colon))	throw MyError("parseObject failed");
		auto* v = parseValue();
		if (!v) {
			throw MyError("parseObject failed");
		}
		obj->emplace(key, v);
	}

	void _addJsonArrayElement(JsonArray* arr) {
		auto* e = parseValue();
		if (!e) throw MyError("parseArray failed");
		arr->emplace_back(e);
	}

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

	bool tryAdvance(Token::Type expected_type) {
		if (currentToken->type != expected_type)
			return false;
		//delete currentToken; // someone still holding its reference


		currentToken = lexer->getNextToken();
		return true;
	}

	JsonValue* parseArray(JsonArray* arr = nullptr) {
		auto b = tryAdvance(Token::Type::OpenBracket);
		MY_ASSERT(b);

		if (arr == nullptr) {
			arr = new JsonArray();
		}

		_addJsonArrayElement(arr);


		while (tryAdvance(Token::Type::Comma)) {
			_addJsonArrayElement(arr);
		}

		if (tryAdvance(Token::Type::CloseBracket)) {
			auto* v = new JsonValue();
			v->setArray(arr);
			return v;
		}

	}



	JsonValue* parseObject(JsonObject* obj = nullptr) {
		auto b = tryAdvance(Token::Type::OpenBrace);
		MY_ASSERT(b);

		if (obj == nullptr) {
			obj = new JsonObject();
		}

		_addJsonObjectMember(obj);

		while (tryAdvance(Token::Type::Comma)) {
			_addJsonObjectMember(obj);
		}

		if (tryAdvance(Token::Type::CloseBrace)) {
			auto* v = new JsonValue();
			v->setObject(obj);
			return v;
		}
	}

	JsonValue* parseValue() {

		switch (currentToken->type)
		{
		case Token::Type::Eof:
			return nullptr;

		case Token::Type::Null: {
			JsonValue* v = new JsonValue();
			auto b = tryAdvance(Token::Type::Null);
			MY_ASSERT(b);
			v->setNull();
			return v;
		}

		case Token::Type::Number: {
			const auto& tv = currentToken->value;
			MY_ASSERT(tryAdvance(Token::Type::Number));
			JsonValue* v = new JsonValue();
			v->setNumber(std::stod(tv));
			return v;
		}

		case Token::Type::String: {

			const auto& tv = currentToken->value;
			MY_ASSERT(tryAdvance(Token::Type::String));

			JsonValue* v = new JsonValue();
			v->setString(tv.c_str());
			return v;
		} break;

		case Token::Type::Boolean: {
			const auto& tv = currentToken->value;
			MY_ASSERT(tryAdvance(Token::Type::Boolean));
			JsonValue* v = new JsonValue();
			v->setBoolean(tv == "true");
			return v;
		} break;

		case Token::Type::OpenBrace: {
			return parseObject();
		} break;

		case Token::Type::OpenBracket: {
			return parseArray();
		}

		default:
			printf("parseValue failed: unknown Token %d, %s", currentToken->type, currentToken->value.c_str());
			throw MyError("[ERR] parseValue failed");
		}

		return nullptr;
	};




};


} // namespace CL