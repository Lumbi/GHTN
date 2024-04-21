#pragma once

#include "GHTN.h"

#ifdef DEBUG
#include <cassert>
#endif

#ifdef DEBUG
#define GHTN_ASSERT(expression, message) assert((message, expression))
#else
#define GHTN_ASSERT(...) (void)0
#endif

#ifdef DEBUG
#define GHTN_ASSERT_FAIL(message) GHTN_ASSERT(false, message)
#else
#define GHTN_ASSERT_FAIL(...) (void)0
#endif

namespace GHTN
{
	class Operation;
	class Task;
}

namespace GHTN
{
	class Debug
	{
	private:
		Debug() = delete;
		Debug(Debug const&) = delete;
		Debug& operator=(Debug const&) = delete;

	public:
		static std::string GetDescription(Operation const&);

		static std::string GetDescription(Task const&);		
	};
}

