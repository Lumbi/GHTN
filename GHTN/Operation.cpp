#include "Operation.h"

namespace GHTN
{
	Operation::Operation()
		: m_Name()
	{
	}

	std::string const& Operation::GetName() const
	{
		return m_Name;
	}

	void Operation::SetName(std::string&& name)
	{
		m_Name = std::move(name);
	}
}
