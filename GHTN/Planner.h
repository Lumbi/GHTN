#pragma once

#include "GHTN.h"

#include <vector>
#include <tuple>

namespace GHTN
{
	class Domain;
	class Task;
	class World;
}

namespace GHTN
{
	using Plan = std::vector<Task const*>;

	class Planner
	{
	public:
		GHTN_API static Plan Find(Domain const&, World);

	private:
		Planner() = delete;
		Planner(Planner const&) = delete;
		Planner& operator=(Planner const&) = delete;

	private:
		using PartialPlan = std::vector<Task const*>;

		static std::tuple<World, PartialPlan> RecursivePartialPlan(Task const*, World);
		static World Apply(Task const& task, World world);
	};
}
