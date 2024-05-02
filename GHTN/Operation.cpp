#include "Operation.h"

namespace GHTN
{
	Operation::Operation()
		: m_Name()
	{
	}

	char const* Operation::GetName() const
	{
		return m_Name.c_str();
	}

	void Operation::SetName(char const* name)
	{
		m_Name = std::string(name);
	}
}
