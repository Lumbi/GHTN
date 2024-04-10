#include "Debug.h"

#include <format>

#include "Task.h"
#include "Operation.h"

namespace GHTN
{
	std::string Debug::GetDescription(Operation const& operation)
	{
		return operation.m_Name;
	}

	std::string Debug::GetDescription(Task const& task)
	{
		if (task.IsPrimitive())
		{
			return std::format("TASK: {}\tOP: {}", task.m_Name, GetDescription(*task.GetOperation()));
		}
		else
		{
			return std::format("TASK: {}", task.m_Name);
		}
	}
}
