#include "Planner.h"

#include "Debug.h"
#include "Domain.h"
#include "Effect.h"
#include "Log.h"
#include "Operation.h"
#include "Task.h"
#include "World.h"

namespace GHTN
{
	std::tuple<World, Planner::PartialPlan> Planner::RecursivePartialPlan(Task const* task, World const world)
	{
		if (task->CanExecute(world))
		{
			if (task->IsPrimitive())
			{
				World updatedWorld = world;
				task->ApplyEffects(updatedWorld);
				task->AssumeExpectations(updatedWorld);
				return std::make_tuple(std::move(updatedWorld), PartialPlan{task});
			}
			else
			{
				GHTN::Task::SubTaskContainer const& subTasks = *task->GetSubTasks();
				GHTN_ASSERT(!subTasks.empty(), "Composite task must have at least one subtask");
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
