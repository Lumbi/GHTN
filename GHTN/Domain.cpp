#include "Domain.h"

#include "Task.h"
#include "Operation.h"

namespace GHTN
{
	Domain::Domain(Task const* root)
		: m_Root(root)
	{
	}

	Task const* Domain::GetRoot() const
	{
		return m_Root;
	}
}

