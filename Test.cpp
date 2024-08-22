#include "Test.h"


#if 0
class ParserTests
{
public:
	static void test_parseObject() {

		{
			const char* json = "{ \"key1\" : 123 }";
			JsonParser p(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Object);
			JsonObject* obj = o->getObject();
			TEST(obj != nullptr);
			TEST(obj->size() == 1);
			TEST(obj->find("key1") != obj->end()); // key1 exists
			auto* v = &obj->at("key1");
			TEST(v->getType() == JsonValue::EType::Number);
			TEST(v->getNumber() == 123);
		}

		{
			const char* json = "{ \"key1\" : 123, \"key2\" : \"Hello, World!\", \"key3\" : [1, true, false, \"hello\"] }";
			JsonParser p(json);
			auto* o = p.parseValue();
		}

		{
			const char* json = "{ \"key1\" : { \"key2\" : { \"key3\" : 123 } } }";
			JsonParser p(json);
			auto* o = p.parseValue();
		}
	};

	static void test_parseArray() {
		const char* json = "[null, true, false, 123, 123.456, \"Hello, World!\", [0, -2.1, 3e4, [1, 2, 3]]]";
		JsonParser p(json);
		auto* o = p.parseValue();
	}

	static void test_parseNull()
	{
		const char* json = "null";
		JsonParser p(json);
		auto* o = p.parseValue();
		TEST(o->isNull() == true);
		TEST(o->getType() == JsonValue::EType::Null);
	}

	static void test_parseBoolean()
	{
		const char* json = "true";
		JsonParser p(json);
		auto* o = p.parseValue();
		TEST(o->isNull() == false);
		TEST(o->getType() == JsonValue::EType::Boolean);
		TEST(o->getBoolean() == true);

		json = "false";
		p = JsonParser(json);
		o = p.parseValue();
		TEST(o->isNull() == false);
		TEST(o->getType() == JsonValue::EType::Boolean);
		TEST(o->getBoolean() == false);
	}

	static void test_parseNumber()
	{
		{
			const char* json = "0";
			JsonParser p(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Number);
			TEST(o->getNumber() == 0);
		}


		{
			const char* json = "0.05";
			JsonParser p(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Number);
			TEST(o->getNumber() == 0.05);
		}

		{
			const char* json = "-0.05";
			JsonParser p(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Number);
			TEST(o->getNumber() == -0.05);
		}

		{
			const char* json = "123";
			JsonParser p(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Number);
			TEST(o->getNumber() == 123);
		}

		{
			const char* json = "-123";
			auto p = JsonParser(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Number);
			TEST(o->getNumber() == -123);
		}

		{
			const char* json = "123.456";
			auto p = JsonParser(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Number);
			TEST(o->getNumber() == 123.456);
		}

		{
			const char* json = "-123.456";
			auto p = JsonParser(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Number);
			TEST(o->getNumber() == -123.456);
		}

		{
			const char* json = "123.456e123";
			auto p = JsonParser(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Number);
			TEST(o->getNumber() == 123.456e123);
		}

		{
			const char* json = "123.456e+123";
			auto p = JsonParser(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Number);
			TEST(o->getNumber() == 123.456e+123);
		}

		{
			const char* json = "123.456e-123";
			auto p = JsonParser(json);
			auto* o = p.parseValue();
			TEST(o->isNull() == false);
			TEST(o->getType() == JsonValue::EType::Number);
			TEST(o->getNumber() == 123.456e-123);
		}

	}
}; // class ParserTests
#endif