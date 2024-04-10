#include "Planner.h"

#include "Task.h"

#include "Domain.h"
#include "Operation.h"
#include "Effect.h"
#include "World.h"
#include "Task.h"
#include "Log.h"
#include "Debug.h"

#include <deque>
#include <variant>
#include <algorithm>

namespace GHTN
{
	bool Planner::CanExecute(Task const& task, World const& world)
	{
		return task.m_Conditions.Check(world);
	}

	World Planner::Apply(Task const& task, World world)
	{
		for (auto&& effect : task.m_Effects)
		{
			std::visit([&](auto&& each) { each(world); }, effect);
		}
		return world;
	}

	std::tuple<World, Planner::PartialPlan> Planner::RecursivePartialPlan(Task const* task, World const world)
	{
		if (CanExecute(*task, world))
		{
			if (task->IsPrimitive())
			{
				return std::make_tuple(Apply(*task, world), PartialPlan { task });
			}
			else
			{
				GHTN::Task::SubTaskContainer const& subTasks = *task->GetSubTasks();
				// TODO: check that subtasks is not empty
				switch (task->GetComposition())
				{
				case GHTN::Task::Composition::all:
				{
					World updatedWorld = world;
					PartialPlan partialPlan;
					for (auto&& subTask : subTasks)
					{
						auto [resultWorld, resultPlan] = RecursivePartialPlan(subTask, updatedWorld);
						if (resultPlan.empty())
						{
							return std::make_tuple(world, PartialPlan {});
						}
						else
						{
							updatedWorld = resultWorld;
							partialPlan.insert(std::end(partialPlan), std::begin(resultPlan), std::end(resultPlan));
						}
					}

					return std::make_tuple(updatedWorld, partialPlan);
				}
				case GHTN::Task::Composition::any:
				{
					for (auto&& subTask : subTasks)
					{
						auto result = RecursivePartialPlan(subTask, world);
						auto&& [_, resultPlan] = result;
						if (!resultPlan.empty())
						{
							return result;
						}
					}
					return std::make_tuple(world, PartialPlan {});
				}
				default:
					throw;
				}
			}
		}
		else
		{
			return std::make_tuple(world, PartialPlan {});
		}
	}

	Plan Planner::Find(Domain const& domain, World world)
	{
		Task const* root = domain.GetRoot();
		auto [_, plan] = RecursivePartialPlan(root, std::move(world));

		if (plan.empty())
		{
			GHTN_LOG(ALL, ("No plan found."));
		}
#ifdef GHTN_ENABLE_LOG
		else
		{
			GHTN_LOG(ALL, ("Plan found:"));
			for (auto&& task : plan)
			{
				GHTN_LOG(ALL, ("\t%s", Debug::GetDescription(*task).c_str()));
			}
		}
#endif

		return plan;
	}
}
