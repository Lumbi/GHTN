#pragma once

#ifdef GHTN_ENABLE_LOG

#include <cstdio>

namespace GHTN::Log
{
	enum class Level
	{
		ERROR,
		WARNING,
		ALL,
	};

#if defined(GHTN_LOG_LEVEL_ERROR)
	constexpr Level level = Level::ERROR;
#elif defined(GHTN_LOG_LEVEL_WARNING)
	constexpr Level level = Level::WARNING;
#elif defined(GHTN_LOG_LEVEL_ALL)
	constexpr Level level = Level::ALL;
#endif
}

#define GHTN_LOG(log_level, log_args) \
{ \
	if (GHTN::Log::Level::log_level <= GHTN::Log::level) \
	{ \
		printf("[%s] %s | ", #log_level, __FUNCTION__); \
		printf log_args; \
		std::putchar('\n'); \
	} \
}
#else // !GHTN_ENABLE_LOG

#define GHTN_LOG(log_level, log_args) (void)0;

#endif
