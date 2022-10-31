#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Silver {

	class Log
	{
	public:
		static void Init();
		static void Shutdown();

		inline static std::shared_ptr<spdlog::logger>& GetSilverLogger() { return s_SilverLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

		//LogMessageWithTag(const char* InTag, )
	private:
		static std::shared_ptr<spdlog::logger> s_SilverLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// I will do something else with those debugs..
// Core log macros
#define AG_CORE_TRACE(...)		::Silver::Log::GetSilverLogger()->trace(__VA_ARGS__)
// #define AG_CORE_DEBUG(...)		::Silver::Log::GetSilverLogger()->debug(__VA_ARGS__)
#define AG_CORE_INFO(...)		::Silver::Log::GetSilverLogger()->info(__VA_ARGS__)
#define AG_CORE_WARN(...)		::Silver::Log::GetSilverLogger()->warn(__VA_ARGS__)
#define AG_CORE_ERROR(...)		::Silver::Log::GetSilverLogger()->error(__VA_ARGS__)
#define AG_CORE_CRITICAL(...)   ::Silver::Log::GetSilverLogger()->critical(__VA_ARGS__)

// Client log macros
#define AG_TRACE(...)			::Silver::Log::GetClientLogger()->trace(__VA_ARGS__)
// #define HZ_DEBUG(...)			::Hazel::Log::GetClientLogger()->debug(__VA_ARGS__)
#define AG_INFO(...)			::Silver::Log::GetClientLogger()->info(__VA_ARGS__)
#define AG_WARN(...)			::Silver::Log::GetClientLogger()->warn(__VA_ARGS__)
#define AG_ERROR(...)			::Silver::Log::GetClientLogger()->error(__VA_ARGS__)
#define AG_CRITICAL(...)        ::Silver::Log::GetClientLogger()->critical(__VA_ARGS__)
