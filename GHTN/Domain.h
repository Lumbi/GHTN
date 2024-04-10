#pragma once

#include <memory>
#include <vector>
#include <unordered_set>

#include "GHTN.h"

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
