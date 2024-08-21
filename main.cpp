#include "Test.h"

int main()
{

	/* TokenizerTests */

	CL::TokenizerTests::test_getNumber();
	CL::TokenizerTests::test_nextToken();
	CL::ParserTests::test_parseObject();
	return 0;
}

