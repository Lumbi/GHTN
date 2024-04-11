#pragma once

#include "GHTN.h"

#include <string>

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

