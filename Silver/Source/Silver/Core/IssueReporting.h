#pragma once
#include <iostream>
#include "Core.h"

// Always turn on asserts in Debug mode
#if defined(AG_DEBUG) && !defined(AG_ENABLE_ASSERTS)
#define AG_ENABLE_ASSERTS
#endif

// TODO(Milan):
/**** LOGGING! ****/

#ifdef AG_ENABLE_ASSERTS
void AssertFailedPrintMessage(const char* inExpression, const char* inFile, uint32_t inLine, const char* inMessage)
{
	std::cout << "Assertion failed!\n";
	std::cout << "Expression: '" << inExpression << "' failed!" << std::endl;
	std::cout << "File name: " << inFile << std::endl;
	std::cout << "Line number: " << inLine << std::endl;
	std::cout << inMessage << std::endl;
}

void AssertFailedPrintMessage(const char* inExpression, const char* inFile, uint32_t inLine)
{
	std::cout << "Assertion failed!\n";
	std::cout << "Expression: '" << inExpression << "' failed!" << std::endl;
	std::cout << "File name: " << inFile << std::endl;
	std::cout << "Line number: " << inLine << std::endl;
} 

#define AG_ASSERT(inExpression, ...)	if (!(inExpression)) { AssertFailedPrintMessage(#inExpression, __FILE__, uint32_t(__LINE__), ##__VA_ARGS__); AG_BREAKPOINT; }
#else
#define AG_ASSERT(inExpression, ...)
#endif // AG_ENABLE_ASSERTS