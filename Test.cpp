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

void MyCommonTests::test_getLine(){
	auto* sz = "line1\nline2\nline3\nli@e4";
	auto* _c = sz + strlen(sz) - 3;
	
	{
		auto* p = _c;
		String line;
		p = Util::getLine(sz, p, line);
		TEST(p != nullptr);
		if (!TEST(line == "li@e4\n")) { printf("  [FAIL] line = [%s]\n", line.c_str()); }

		p = Util::getLine(sz, p, line);
		TEST(p != nullptr);
		if (!TEST(line == "line3\n")) { printf("  [FAIL] line = [%s]\n", line.c_str()); }

		p = Util::getLine(sz, p, line);
		TEST(p != nullptr);
		if (!TEST(line == "line2\n")) { printf("  [FAIL] line = [%s]\n", line.c_str()); }

		p = Util::getLine(sz, p, line);
		TEST(p != nullptr);
		if (!TEST(line == "line1\n")) { printf("  [FAIL] line = [%s]\n", line.c_str()); }

		p = Util::getLine(sz, p, line);
		TEST(p == nullptr);
	}

	{ // test print_lines

		auto* p = _c;
		size_t getNLine = 3;
		size_t lineNumber = 4;
		size_t getUntilLineNum = lineNumber > getNLine? lineNumber - getNLine + 1 : 1;
		
		Map<size_t, String> lines;
		for (auto i = 0; i < lineNumber; i++) {
			auto n = lineNumber - i;
			if (n < getUntilLineNum) break;
			p = Util::getLine(sz, p, lines[n]);
		}

		auto lastLineNum = lineNumber + 1;
		
		{	// set eLine
			auto& eLine = lines[lastLineNum];
			p = Util::getLine(sz, _c, eLine);
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


				if		( c >  _c  )	eLine[i] = '_';
				else if ( c == _c  )	eLine[i] = '^';
				else if (*c == '\t')	eLine[i] = '\t';
				else if (*c == '\n')	eLine[i] = '\n';
				else					eLine[i] = '_';

				i++;
			}
		} // set eLine

		{ // print lines
			printf("Error: Unexpected token[%c]\n", *_c);
			puts("---------------------------");

			for (auto n = getUntilLineNum; n <= lastLineNum; n++) {

				if (n == lastLineNum) { // arrow Line
					// padding for line number
					auto npad = Util::ndigit(lineNumber) + strlen(": ");

					while (npad) {
						putchar(' ');
						npad--;
					}

					printf("%s", lines[lastLineNum].c_str());
				}
				else {
					printf("%zu: %s", n, lines[n].c_str());
				}
			}
		} // print lines
	} // test print_lines

}

void MyCommonTests::test_rfind()
{
	{
		auto* sz = "line1\nline2\nline3\nline4";
		auto* _c = sz + strlen(sz) - 3; // 'n' of line4
		auto* p = _c;

		p = Util::rfind(sz, p, '\n');
		if (!TEST(p != nullptr))	{ printf("  [ACTUAL]	 p == %p\n", p); }
		if (!TEST(*p == '\n'))		{ printf("  [ACTUAL]	*p == %c\n", *p); }
		if (!TEST(*(p + 1) == 'l')) { printf("  [ACTUAL] (*p + 1) == %c\n", *(p + 1)); }
		if (!TEST(*(p - 1) == '3')) { printf("  [ACTUAL] (*p - 1) == %c\n", *(p - 1)); }


		p = Util::rfind(sz, --p, '\n');
		if (!TEST(p != nullptr))	{ printf("  [ACTUAL]	 p == %p\n", p); }
		if (!TEST(*p == '\n'))		{ printf("  [ACTUAL]	*p == %c\n", *p); }
		if (!TEST(*(p + 1) == 'l')) { printf("  [ACTUAL] (*p + 1) == %c\n", *(p + 1)); }
		if (!TEST(*(p - 1) == '2')) { printf("  [ACTUAL] (*p - 1) == %c\n", *(p - 1)); }

		p = Util::rfind(sz, --p, '\n');
		if (!TEST(p != nullptr))	{ printf("  [ACTUAL]	 p == %p\n", p); }
		if (!TEST(*p == '\n'))		{ printf("  [ACTUAL]	*p == %c\n", *p); }
		if (!TEST(*(p + 1) == 'l')) { printf("  [ACTUAL] (*p + 1) == %c\n", *(p + 1)); }
		if (!TEST(*(p - 1) == '1')) { printf("  [ACTUAL] (*p - 1) == %c\n", *(p - 1)); }


		p = Util::rfind(sz, --p, '\n');
		if (!TEST(p == nullptr))	{ printf("  [ACTUAL]	 p == %p\n", p); }
	}
	
	{  
		auto* sz = "line1\nline2\nline3\nline4";
		auto* _c = sz + strlen(sz) - 6; // '\n' of line3
		auto* p = _c;

		p = Util::rfind(sz, p, '\n');
		if (!TEST(p != nullptr))	{ printf("  [ACTUAL]	 p == %p\n", p); }
		if (!TEST(*p == '\n'))		{ printf("  [ACTUAL]	*p == %c\n", *p); }
		if (!TEST(*(p + 1) == 'l')) { printf("  [ACTUAL] (*p + 1) == %c\n", *(p + 1)); }
		if (!TEST(*(p - 1) == '3')) { printf("  [ACTUAL] (*p - 1) == %c\n", *(p - 1)); }


		p = Util::rfind(sz, --p, '\n');
		if (!TEST(p != nullptr))	{ printf("  [ACTUAL]	 p == %p\n", p); }
		if (!TEST(*p == '\n'))		{ printf("  [ACTUAL]	*p == %c\n", *p); }
		if (!TEST(*(p + 1) == 'l')) { printf("  [ACTUAL] (*p + 1) == %c\n", *(p + 1)); }
		if (!TEST(*(p - 1) == '2')) { printf("  [ACTUAL] (*p - 1) == %c\n", *(p - 1)); }

		p = Util::rfind(sz, --p, '\n');
		if (!TEST(p != nullptr))	{ printf("  [ACTUAL]	 p == %p\n", p); }
		if (!TEST(*p == '\n'))		{ printf("  [ACTUAL]	*p == %c\n", *p); }
		if (!TEST(*(p + 1) == 'l')) { printf("  [ACTUAL] (*p + 1) == %c\n", *(p + 1)); }
		if (!TEST(*(p - 1) == '1')) { printf("  [ACTUAL] (*p - 1) == %c\n", *(p - 1)); }


		p = Util::rfind(sz, --p, '\n');
		if (!TEST(p == nullptr))	{ printf("  [ACTUAL]	 p == %p\n", p); }
	}
}

void MyCommonTests::test_ndigit()
{
	size_t n = Util::ndigit(0);
	if (!TEST(n == 0)) { printf("  [FAIL] n = %zu\n", n); }
	
	n = Util::ndigit(111);
	if (!TEST(n == 3)) { printf("  [FAIL] n = %zu\n", n); }
	
	int v = -111;
	n = Util::ndigit(v);
	if (!TEST(n == 3)) { printf("  [FAIL] n = %zu\n", n); }

	

}

} // namespace CL