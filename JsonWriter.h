#pragma once
#include "JsonValue.h"
#include <fstream>

namespace CL {

class JsonWriter
{
private:
	std::ofstream _ofs;
	const char* _filePath = nullptr;

public:
	JsonWriter(const char* filePath) : _filePath(filePath) {}

	void write(const JsonValue& v) {
		if (!_ofs.is_open()) { 
			_ofs.open("test.txt", std::ofstream::out | std::ofstream::app);
		}
		
		switch (v.getType())
		{
		default:							{ throw MyError("[ERR] Unknown type");			} break;
		case JsonValue::EType::Undefined:	{ throw MyError("Undefined type");				} break;
		case JsonValue::EType::Null:		{ _ofs << "null";								} break;
		case JsonValue::EType::Boolean:		{ _ofs << (v.getBoolean() ? "true" : "false");	} break;
		case JsonValue::EType::Number:		{ _ofs <<  v.getNumber();						} break;
		case JsonValue::EType::String:		{ _ofs <<  v.getString();						} break;
		case JsonValue::EType::Array:
		{
			_ofs << "[";
			for (const auto& a : *v.getArray())
			{
				write(a);
				_ofs << ", ";
			}
			_ofs << "]";
		} break;
		case JsonValue::EType::Object:
		{
			_ofs << "{";
			for (const auto& o : *v.getObject())
			{
				_ofs << "\"" << o.first << "\"" << ": ";
				write(o.second);
				_ofs << ", ";
			}
			_ofs << "}";
		} break;
		}
	}
}; // class JsonWriter


} // namespace CL