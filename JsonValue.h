#pragma once
#include "MyCommon.h"

namespace CL {

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
	Value _value{};


public:
	JsonValue() = default;

	~JsonValue();

	JsonValue(JsonValue&& rhs) noexcept;

	JsonValue& operator=(JsonValue&& rhs);


	EType getType() const;


	bool isNull() const;
	void setNull();

	bool isUndefined() const;
	void setUndefined();

	JsonNumber  getNumber() const;
	void		setNumber(JsonNumber n);


	void		setBoolean(bool b);
	bool		getBoolean() const;

	void		setObject(JsonObject* obj);
	JsonObject* getObject() const;
	JsonObject* setToObject();

	void	   setArray(JsonArray* arr);
	JsonArray* getArray() const;
	JsonArray* setToArray();

	JsonString* getString() const;
	void		setString(const char* sz);
	JsonString* setToString();

}; // class JsonValue

} // namespace CL