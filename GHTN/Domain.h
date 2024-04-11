#pragma once

#include "GHTN.h"

#include <memory>
#include <vector>
#include <unordered_set>

namespace GHTN
{
	class Task;
	class Operation;
}

namespace GHTN
{
	class Domain
	{

	public:
		GHTN_API explicit Domain(Task const* root);

		Task const* GetRoot() const;

	private:
		Task const* m_Root = nullptr;
	};
}
