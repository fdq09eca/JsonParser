#include "Test.h"

namespace CL {


void ParserTests::test_parseObject() {
	{
		JsonParser p("{ \"key1\" : [null, true, false, 123] }");
		JsonValue jv;

		p.parseObject(jv);

		TEST(jv.getType() == JsonValue::EType::Object);

		auto* obj = jv.getObject();
		TEST(obj != nullptr);

		TEST(obj->size() == 1);
		TEST(obj->find("key1") != obj->end()); // key1 exists

		const auto& v = obj->at("key1"); // jv non copyable
		TEST(v.getType() == JsonValue::EType::Array);
		auto* arr = v.getArray();
		TEST(arr != nullptr);
		TEST(arr->size() == 4);
		TEST(arr->at(0).isNull());
		TEST(arr->at(1).getBoolean());
		TEST(arr->at(2).getBoolean() == false);
		TEST(arr->at(3).getNumber() == 123);
	}

	{
		JsonParser p("{ \"key1\" : 123 }");

		auto jv = JsonValue();
		p.parseValue(jv);

		TEST(jv.getType() == JsonValue::EType::Object);
		TEST(jv.getObject()->size() == 1);
		TEST(jv.getObject()->find("key1") != jv.getObject()->end());
		TEST(jv.getObject()->at("key1").getNumber() == 123);

	}

	{
		JsonParser p("{ \"key1\" : 123, \"key2\": 321 }");

		auto jv = JsonValue();
		p.parseValue(jv);

		TEST(jv.getType() == JsonValue::EType::Object);
		TEST(jv.getObject()->size() == 2);
		TEST(jv.getObject()->find("key1") != jv.getObject()->end());
		TEST(jv.getObject()->at("key1").getNumber() == 123);

		TEST(jv.getObject()->find("key2") != jv.getObject()->end());
		TEST(jv.getObject()->at("key2").getNumber() == 321);

	}

	{
		JsonParser p("{ \"key1\" : {\"key2\" : 123} }");

		auto jv = JsonValue();
		p.parseValue(jv);

		TEST(jv.getType() == JsonValue::EType::Object);
		TEST(jv.getObject()->size() == 1);
		TEST(jv.getObject()->find("key1") != jv.getObject()->end());
		TEST(jv.getObject()->at("key1").getType() == JsonValue::EType::Object);
		TEST(jv.getObject()->at("key1").getObject()->size() == 1);
		TEST(jv.getObject()->at("key1").getObject()->find("key2") != jv.getObject()->at("key1").getObject()->end());
		TEST(jv.getObject()->at("key1").getObject()->at("key2").getNumber() == 123);
	}

	{
		JsonParser p("{ \"key1\" : {\"key2\" : [123, null, false]} }");
		auto jv = JsonValue();
		p.parseValue(jv);
		TEST(jv.getType() == JsonValue::EType::Object);
		TEST(jv.getObject()->size() == 1);
		TEST(jv.getObject()->find("key1") != jv.getObject()->end());
		TEST(jv.getObject()->at("key1").getType() == JsonValue::EType::Object);
		TEST(jv.getObject()->at("key1").getObject()->size() == 1);
		TEST(jv.getObject()->at("key1").getObject()->find("key2") != jv.getObject()->at("key1").getObject()->end());
		TEST(jv.getObject()->at("key1").getObject()->at("key2").getType() == JsonValue::EType::Array);
		TEST(jv.getObject()->at("key1").getObject()->at("key2").getArray()->size() == 3);
		TEST(jv.getObject()->at("key1").getObject()->at("key2").getArray()->at(0).getNumber() == 123);
		TEST(jv.getObject()->at("key1").getObject()->at("key2").getArray()->at(1).isNull());
		TEST(jv.getObject()->at("key1").getObject()->at("key2").getArray()->at(2).getBoolean() == false);
	}
} 

void TokenizerTests::_getNumber(const char* v) {
	Tokenizer t(v);
	auto b = t.nextToken();
	TEST(b);

	if (!TEST(t.isEquals(Token::Type::Number, v))) {
		printf("  [FAIL_VAL] v = %s\n", v);
	}
}

void TokenizerTests::test_nextToken() {
	Tokenizer t("{ \"key1\" : [null, true, false, 123] }");

	auto b = t.nextToken();
	TEST(b);
	TEST(t.isOp("{"));


	b = t.nextToken();
	TEST(b);
	TEST(t.isString("key1"));

	b = t.nextToken();
	TEST(b);
	TEST(t.isOp(":"));


	b = t.nextToken();
	TEST(b);
	TEST(t.isOp("["));

	b = t.nextToken();
	TEST(b);
	TEST(t.isEquals(Token::Type::Identifier, "null"));

	b = t.nextToken();
	TEST(b);
	TEST(t.isOp(","));

	b = t.nextToken();
	TEST(b);
	TEST(t.isIdentifier("true"));

	b = t.nextToken();
	TEST(b);
	TEST(t.isOp(","));

	b = t.nextToken();
	TEST(b);
	TEST(t.isIdentifier("false"));

	b = t.nextToken();
	TEST(b);
	TEST(t.isOp(","));

	b = t.nextToken();
	TEST(b);
	TEST(t.isEquals(Token::Type::Number, "123"));
}

void TokenizerTests::test_getNumber() {
	// zero
	_getNumber("0");
	_getNumber("0.0");
	_getNumber("0.00");
	_getNumber("0.000");
	_getNumber("0.0e0");
	_getNumber("0.0e+0");
	_getNumber("0.0e-0");
	_getNumber("0.0E0");
	_getNumber("0.0E+0");
	_getNumber("0.0E-0");

	//positive
	_getNumber("1");
	_getNumber("1.0");
	_getNumber("1.00");
	_getNumber("1.000");
	_getNumber("1.1");
	_getNumber("1.11");
	_getNumber("1.123");
	_getNumber("1.0e0");
	_getNumber("1.0e+0");
	_getNumber("1.0e-0");
	_getNumber("1.123e0");
	_getNumber("1.123e+0");
	_getNumber("1.123e-0");
	_getNumber("1.0E0");
	_getNumber("1.0E+0");
	_getNumber("1.0E-0");

	_getNumber("12");
	_getNumber("12.0");
	_getNumber("12.00");
	_getNumber("12.000");
	_getNumber("12.1");
	_getNumber("12.11");
	_getNumber("12.123");
	_getNumber("12.0e0");
	_getNumber("12.0e+0");
	_getNumber("12.0e-0");
	_getNumber("12.123e0");
	_getNumber("12.123e+0");
	_getNumber("12.123e-0");

	_getNumber("12.123e123");
	_getNumber("12.123e+123");
	_getNumber("12.123e-123");
	_getNumber("12.0E0");
	_getNumber("12.0E+123");
	_getNumber("12.0E-123");

	//positive
	_getNumber("-1");
	_getNumber("-1.0");
	_getNumber("-1.00");
	_getNumber("-1.000");
	_getNumber("-1.1");
	_getNumber("-1.11");
	_getNumber("-1.123");
	_getNumber("-1.0e0");
	_getNumber("-1.0e+0");
	_getNumber("-1.0e-0");
	_getNumber("-1.123e0");
	_getNumber("-1.123e+0");
	_getNumber("-1.123e-0");
	_getNumber("-1.0E0");
	_getNumber("-1.0E+0");
	_getNumber("-1.0E-0");
}

void MyCommonTests::test_rfind()
{
	auto lineNumber = 4;
	auto printNLine = 3;
	auto* sz = "line1\nline2\nline3\nline4";
	auto* _c = sz + strlen(sz) - 1;

	auto* q = _c;
	auto* p = q;

	std::deque<String> lines;


	while (true) {
		p = Util::rfind(sz, q, '\n');
		lines.emplace_front();
		auto& line = lines.front();
		if (!p) {
			p = sz;
			line.assign(p, q + 1);
			line.append("\n");
			line.insert(0, std::to_string(lineNumber) + ": ");
			break;
		}
		else
		{
			line.assign(p + 1, q + 1);
			line.append("\n");
			line.insert(0, std::to_string(lineNumber) + ": ");
			q = p - 1;
			lineNumber--;
			if (printNLine-- == 0) break;
		}
	}

	const auto& lastLine = lines.back();
	lines.emplace_back();
	auto& eLine = lines.back();
	eLine.resize(lastLine.size());


	for (auto& line : lines) {
		printf(line.c_str());
	}

}

} // namespace CL