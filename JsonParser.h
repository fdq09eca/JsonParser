#pragma once

#include "MyCommon.h"

namespace CL
{
class JsonValue;
using JsonString = String;
using JsonArray = Vector<JsonValue>;
using JsonObject = Map<String, JsonValue>;
using JsonNumber = f64;
using JsonBoolean = bool;

class JsonValue {
private:
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



public:
	static const char* parse(const char* sz, JsonString& str) {
		if (sz == nullptr) {
			throw MyError("sz == nullptr");
		}

		const char* p = sz;
		if (*p != '"') {
			throw MyError("Invalid character");
		}

		p++;


		const char* pEnd = Util::findNextChar(p, '"', '\\');





		if (pEnd == nullptr) {
			throw MyError("Invalid character");
		}

		str.assign(p, pEnd - p);
		return pEnd + 1;
	}

	static JsonValue parse(const char* sz) {
		if (sz == nullptr) {
			throw MyError("sz == nullptr");
		}

		JsonValue root;

		while (true) {
			const char* p = sz;

			if (*p == '\0') {
				break;
			}

			else if (*p == '{') {

				JsonString key;
				JsonValue val;

				p = strchr(p, '"');


				auto* obj = root.setToObject();
				if (obj == nullptr) {
					throw MyError("obj == nullptr");
				}
				break;
			}

			else if (*p == '}') {
				break;
			}

			else if (*p == '[') {
				auto* arr = root.setToArray();
				if (arr == nullptr) {
					throw MyError("arr == nullptr");
				}
				break;
			}

			else if (*p == ']') {
				break;
			}

			else if (*p == '"') {
				root.setString(sz);
				break;
			}

			else if (*p == ',') {
				break;
			}

			else if (*p == ':') {
				break;
			}

			else if (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') {
				sz++;
			}

			else {
				throw MyError("Invalid character");
			}


		}
		return root;
	};




};


} // namespace CL