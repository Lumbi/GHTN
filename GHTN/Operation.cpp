#include "Operation.h"

namespace GHTN
{
	Operation::ID Operation::s_ID = 0;

	Operation::Operation()
		: m_ID(s_ID)
		, m_Name()
	{
		++s_ID;
	}

	Operation::ID Operation::GetID() const
	{
		return m_ID;
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
