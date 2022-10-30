#pragma once

// Determine platform
#if defined(_WIN32)
	#if defined(_WIN64)
		#define AG_PLATFORM_WINDOWS
	#else 
		#error Silver only supports 64-bit Windows platforms
	#endif
#elif defined(__ANDROID__) || defined(__linux__) || defined(__APPLE__)
	#error Right now Silver only supports Windows
#endif

// Determine compiler
#if defined(_MSC_VER)
	#define AG_COMPILER_MSVC
#elif defined(__GNUC__)
	#define AG_COMPILER_GCC
#elif defined(__MINGW64__) || defined (__MINGW32__)
	#define AG_COMPILER_GCC
	#define AG_COMPILER_MINGW
#elif defined(__clang__)
	#define AG_COMPILER_CLANG
#endif

#if defined(AG_PLATFORM_WINDOWS)
	#define AG_BREAKPOINT __debugbreak();
#endif

// Crashes the application
#define AG_CRASH int *ptr = nullptr; *ptr = 0;

// Macro to get the current function name
#if defined(AG_COMPILER_MSVC)
	#define AG_FUNCTION_NAME __FUNCTION__
#elif defined(AG_COMPILER_GCC) || defined(AG_COMPILER_CLANG)
	#define AG_FUNCTION_NAME __PRETTY_FUNCTION__
#else
	#error Undefined
#endif

#define AG_BIND_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }