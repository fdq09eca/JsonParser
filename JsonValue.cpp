#include "JsonValue.h"

namespace CL
{



void JsonValue::setUndefined() {
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
	_type = EType::Undefined;
}

JsonValue::~JsonValue() {
	setUndefined();
}

JsonValue::JsonValue(JsonValue&& rhs) noexcept {
	operator=(std::move(rhs));
}

JsonValue& JsonValue::operator=(JsonValue&& rhs) {
	if (this != &rhs) {
		setNull();
		_type = rhs._type;
		_value = rhs._value;

		rhs._type = EType::Undefined;
	}
	return *this;
}

JsonValue::EType JsonValue::getType() const {
	return _type;
}

bool JsonValue::getBoolean() const {
	return _value.boolean;
}

void JsonValue::setBoolean(bool b)
{
	if (_type != EType::Boolean) {
		setNull();
		_type = EType::Boolean;
	}
	_value.boolean = b;
}

bool JsonValue::isNull() const {
	return _type == EType::Null;
}

bool JsonValue::isUndefined() const {
	return _type == EType::Undefined;
}

void JsonValue::setNull() {
	setUndefined();
	_type = EType::Null;
}

JsonNumber JsonValue::getNumber() const {
	if (_type != EType::Number)
		throw MyError("JsonValue::getNumber failed");
	return _value.number;
}

void JsonValue::setNumber(JsonNumber n) {
	if (_type != EType::Number) {
		setNull();
		_type = EType::Number;
	}
	_value.number = n;
}

JsonObject* JsonValue::getObject() const {
	if (_type != EType::Object)
		return nullptr;
	return _value.object;
}

void JsonValue::setObject(JsonObject* obj) {
	setNull();
	_type = EType::Object;
	_value.object = obj;
}

JsonObject* JsonValue::setToObject() {
	auto* p = getObject();

	if (p) return p;

	setNull();
	_type = EType::Object;
	_value.object = new JsonObject();

	return _value.object;
}

JsonArray* JsonValue::getArray() const {
	if (_type != EType::Array)
		return nullptr;
	return _value.array;
}

void JsonValue::setArray(JsonArray* arr) {
	setNull();
	_type = EType::Array;
	_value.array = arr;
}

JsonArray* JsonValue::setToArray() {
	auto* p = getArray();
	if (p) return p;

	setNull();
	_type = EType::Array;
	_value.array = new JsonArray();

	return _value.array;
}

JsonString* JsonValue::getString() const {
	if (_type != EType::String)
		return nullptr;
	return _value.string;
}

JsonString* JsonValue::setToString() {
	auto* p = getString();
	if (p) return p;

	setNull();
	_type = EType::String;
	_value.string = new JsonString();

	return _value.string;
}

void JsonValue::setString(const char* sz) {
	auto* s = setToString();
	if (sz) *s = sz;
}
} // namespace CL