#include "AgPCH.h"
#include "Logging.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include <spdlog/sinks/basic_file_sink.h>

namespace Silver {

	std::shared_ptr<spdlog::logger> Log::s_SilverLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_SilverLogger = spdlog::stdout_color_mt("Silver");
		s_SilverLogger->set_level(spdlog::level::trace);
		
		s_ClientLogger = spdlog::stdout_color_mt("App");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

	void Log::Shutdown()
	{
		s_SilverLogger.reset();
		s_ClientLogger.reset();
	}

}