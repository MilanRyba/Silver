#pragma once
#include "Core.h"
#include "Logging.h"

// Always turn on asserts in Debug mode
#if defined(AG_DEBUG) && !defined(AG_ENABLE_ASSERTS)
#define AG_ENABLE_ASSERTS
#endif

#ifdef AG_ENABLE_ASSERTS
inline void AssertFailedPrintMessage(const char* InExpression, const char* InFile, uint32_t InLine, const char* InMessage)
{
	AG_CORE_ERROR("Expression: '{0}' Failed!", InExpression);
	AG_CORE_INFO("File name: {0}", InFile);
	AG_CORE_INFO("Line number: {0}", InLine);
	AG_CORE_ERROR("{0}", InMessage);
}

inline void AssertFailedPrintMessage(const char* InExpression, const char* InFile, uint32_t InLine)
{
	AG_CORE_ERROR("Expression: '{0}' Failed!", InExpression);
	AG_CORE_INFO("File name: {0}", InFile);
	AG_CORE_INFO("Line number: {0}", InLine);
} 

#define AG_ASSERT(InExpression, ...)	if (!(InExpression)) { AssertFailedPrintMessage(#InExpression, __FILE__, uint32_t(__LINE__), ##__VA_ARGS__); AG_BREAKPOINT; }
#else
#define AG_ASSERT(inExpression, ...) // Note(Milan): Instead of nothing maybe just inExpression in non-debug?
#endif // AG_ENABLE_ASSERTS