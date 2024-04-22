#include "Debug.h"

#include "Task.h"
#include "Operation.h"

#include <sstream>

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
			std::ostringstream stream;
			stream << "TASK: " << task.m_Name << "\tOP: " << GetDescription(*task.GetOperation());
			return stream.str();
		}
		else
		{
			std::ostringstream stream;
			stream << "TASK: " << task.m_Name;
			return stream.str();
		}
	}
}
