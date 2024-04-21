#include "GHTN/Condition.h"
#include "GHTN/Domain.h"
#include "GHTN/Effect.h"
#include "GHTN/Operation.h"
#include "GHTN/Planner.h"
#include "GHTN/Runner.h"
#include "GHTN/Sensor.h"
#include "GHTN/Task.h"
#include "GHTN/World.h"

#include "Test.h"

#include <memory>

namespace GHTNTest
{
    using namespace GHTN;
    using namespace GHTN::ConditionTreeBuilder;

    TEST(Find_plan_on_domain_with_a_task_without_conditions)
    {
        Operation operation;
        Task task(&operation);
        Domain domain(&task);
        World world;
        Plan plan = Planner::Find(domain, world);

        EXPECT(plan.size() == 1);
        EXPECT(plan[0] == &task);
    }

    TEST(Find_plan_on_domain_with_a_task_with_simple_valid_condition)
    {
        Operation operation;
        Task task(&operation);
        World::Property property = 1;
        task.SetConditions(Just(Condition(property, Predicate::equal, 37)));
        Domain domain(&task);
        World world;
        world.Set(property, 37);
        Plan plan = Planner::Find(domain, world);

        EXPECT(plan.size() == 1);
        EXPECT(plan[0] == &task);
    }

    TEST(Find_plan_on_domain_with_a_task_with_simple_invalid_condition)
    {
        Operation operation;
        Task task(&operation);
        World::Property property = 1;
        task.SetConditions(Just(Condition(property, Predicate::equal, 37)));
        Domain domain(&task);
        World world;
        Plan plan = Planner::Find(domain, world);

        EXPECT(plan.empty());
    }

    TEST(Find_plan_on_domain_with_a_task_requiring_all_subtasks_with_valid_conditons)
    {
        Operation operation;
        Task subtask1(&operation);
        Task subtask2(&operation);
        Task subtask3(&operation);
        Task task(Task::ALL, { &subtask1, &subtask2, &subtask3 });
        Domain domain(&task);
        World world;
        Plan plan = Planner::Find(domain, world);

        EXPECT(plan.size() == 3);
        EXPECT(plan[0] == &subtask1);
        EXPECT(plan[1] == &subtask2);
        EXPECT(plan[2] == &subtask3);
    }

    TEST(Find_plan_on_domain_with_task_requiring_all_subtasks_but_last_task_has_invalid_conditions)
    {
        Operation operation;
        Task subtask1(&operation);
        Task subtask2(&operation);
        Task subtask3(&operation);
        World::Property property = 1;
        subtask3.SetConditions(Just(Condition(property, Predicate::equal, 42)));
        Task task(Task::ALL, { &subtask1, &subtask2, &subtask3 });
        Domain domain(&task);
        World world;
        Plan plan = Planner::Find(domain, world);

        EXPECT(plan.empty());
    }

    TEST(Find_plan_on_domain_with_task_requiring_any_subtasks_with_valid_conditions)
    {
        Operation operation;
        Task subtask1(&operation);
        Task subtask2(&operation);
        Task subtask3(&operation);
        Task task(Task::ANY, { &subtask1, &subtask2, &subtask3 });
        Domain domain(&task);
        World world;
        Plan plan = Planner::Find(domain, world);

        EXPECT(plan.size() == 1);
        EXPECT(plan[0] == &subtask1);
    }

    TEST(Find_plan_on_domain_with_task_requiring_any_subtasks_but_all_have_invalid_conditions)
    {
        Operation operation;
        Task subtask1(&operation);
        Task subtask2(&operation);
        Task subtask3(&operation);
        World::Property property = 1;
        subtask1.SetConditions(Just(Condition(property, Predicate::equal, 37)));
        subtask2.SetConditions(Just(Condition(property, Predicate::equal, 42)));
        subtask3.SetConditions(Just(Condition(property, Predicate::equal, 88)));
        Task task(Task::ANY, { &subtask1, &subtask2, &subtask3 });
        Domain domain(&task);
        World world;
        Plan plan = Planner::Find(domain, world);

        EXPECT(plan.empty());
    }

    TEST(Find_plan_on_domain_where_the_effects_of_a_task_enable_another_task)
    {
        Operation operation;
        Task subtask1(&operation);
        Task subtask2(&operation);
        Task subtask3(&operation);
        World::Property property = 1;
        subtask1.AddEffect(Effects::Set { property, 17 });
        subtask3.SetConditions(Just(Condition(property, Predicate::equal, 17)));
        Task task(Task::ALL, { &subtask1, &subtask2, &subtask3 });
        Domain domain(&task);
        World world;
        Plan plan = Planner::Find(domain, world);

        EXPECT(plan.size() == 3);
        EXPECT(plan[0] == &subtask1);
        EXPECT(plan[1] == &subtask2);
        EXPECT(plan[2] == &subtask3);
    }

    TEST(Find_plan_on_domain_where_the_effects_of_a_task_prevent_another_task)
    {
        Operation operation;
        Task subtask1(&operation);
        Task subtask2(&operation);
        Task subtask3(&operation);
        World::Property property = 1;
        subtask1.AddEffect(Effects::Set { property, 17 });
        subtask3.SetConditions(Just(Condition(property, Predicate::equal, 17, true)));
        Task task(Task::ALL, { &subtask1, &subtask2, &subtask3 });
        Domain domain(&task);
        World world;
        Plan plan = Planner::Find(domain, world);

        EXPECT(plan.empty());
    }

    TEST(Find_plan_on_domain_where_the_expectations_of_a_task_enable_another_task)
    {
        Operation operation;
        Task task1(&operation);
        Task task2(&operation);
        Task task3(&operation);
        World::Property property = 1;
        task1.AddExpectation(property, 36);
        task3.SetConditions(Just(Condition(property, Predicate::greater, 17)));
        Task root(Task::ALL, { &task1, &task2, &task3 });
        Domain domain(&root);
        World world;
        Plan plan = Planner::Find(domain, world);

        EXPECT(plan.size() == 3);
        EXPECT(plan[0] == &task1);
        EXPECT(plan[1] == &task2);
        EXPECT(plan[2] == &task3);
    }

    TEST(Find_plan_on_domain_where_the_exectations_of_a_task_disable_another_task)
    {
        Operation operation;
        Task task1(&operation);
        Task task2(&operation);
        Task task3(&operation);
        World::Property property = 1;
        task1.AddExpectation(property, 42);
        task3.SetConditions(Just(Condition(property, Predicate::equal, 98)));
        Task root(Task::ALL, { &task1, &task2, &task3 });
        Domain domain(&root);
        World world;
        Plan plan = Planner::Find(domain, world);

        EXPECT(plan.empty());
    }

    TEST(Find_plan_on_domain_with_a_task_with_OR_condition)
    {
        Operation operation;
        Task task(&operation);
        World::Property property = 1;
        task.SetConditions(Or(Condition(property, Predicate::equal, 17), Condition(property, Predicate::equal, 36)));
        Domain domain(&task);
        
        {
            World unsolvableWorld;
            Plan plan = Planner::Find(domain, unsolvableWorld);

            EXPECT(plan.empty());
        }
        {
            World solvableWorld;
            solvableWorld.Set(property, 17);
            Plan plan = Planner::Find(domain, solvableWorld);
            
            EXPECT(plan.size() == 1);
            EXPECT(plan[0] == &task);
        }
        {
            World solvableWorld;
            solvableWorld.Set(property, 36);
            Plan plan = Planner::Find(domain, solvableWorld);

            EXPECT(plan.size() == 1);
            EXPECT(plan[0] == &task);
        }
    }

    TEST(Find_plan_on_domain_with_task_with_AND_condition)
    {
        Operation operation;
        Task task(&operation);
        World::Property property1 = 1;
        World::Property property2 = 2;
        task.SetConditions(And(Condition(property1, Predicate::equal, 17), Condition(property2, Predicate::equal, 36)));
        Domain domain(&task);

        {
            World unsolvableWorld;
            Plan plan = Planner::Find(domain, unsolvableWorld);

            EXPECT(plan.empty());
        }
        {
            World unsolvableWorld;
            unsolvableWorld.Set(property1, 17);
            Plan plan = Planner::Find(domain, unsolvableWorld);

            EXPECT(plan.empty());
        }
        {
            World unsolvableWorld;
            unsolvableWorld.Set(property2, 36);
            Plan plan = Planner::Find(domain, unsolvableWorld);

            EXPECT(plan.empty());
        }
        {
            World solvableWorld;
            solvableWorld.Set(property1, 17);
            solvableWorld.Set(property2, 36);
            Plan plan = Planner::Find(domain, solvableWorld);

            EXPECT(plan.size() == 1);
            EXPECT(plan[0] == &task);
        }
    }

    TEST(Find_plan_on_domain_with_task_with_EQUAL_condition)
    {
        Operation operation;
        Task task(&operation);
        World::Property property = 1;
        task.SetConditions(Just(Condition(property, Predicate::equal, 17)));
        Domain domain(&task);

        {
            World unsolvableWorld;
            unsolvableWorld.Set(property, 42);
            Plan plan = Planner::Find(domain, unsolvableWorld);

            EXPECT(plan.empty());
        }
        {
            World solvableWorld;
            solvableWorld.Set(property, 17);
            Plan plan = Planner::Find(domain, solvableWorld);

            EXPECT(plan.size() == 1);
            EXPECT(plan[0] == &task);
        }
    }

    TEST(Find_plan_on_domain_with_task_with_GREATER_condition)
    {
        Operation operation;
        Task task(&operation);
        World::Property property = 1;
        task.SetConditions(Just(Condition(property, Predicate::greater, 17)));
        Domain domain(&task);

        {
            World unsolvableWorld;
            unsolvableWorld.Set(property, 16);
            Plan plan = Planner::Find(domain, unsolvableWorld);

            EXPECT(plan.empty());
        }
        {
            World unsolvableWorld;
            unsolvableWorld.Set(property, 17);
            Plan plan = Planner::Find(domain, unsolvableWorld);

            EXPECT(plan.empty());
        }
        {
            World solvableWorld;
            solvableWorld.Set(property, 18);
            Plan plan = Planner::Find(domain, solvableWorld);

            EXPECT(plan.size() == 1);
            EXPECT(plan[0] == &task);
        }
    }

    TEST(Find_plan_on_domain_with_task_with_LESS_condition)
    {
        Operation operation;
        Task task(&operation);
        World::Property property = 1;
        task.SetConditions(Just(Condition(property, Predicate::less, 42)));
        Domain domain(&task);

        {
            World unsolvableWorld;
            unsolvableWorld.Set(property, 42);
            Plan plan = Planner::Find(domain, unsolvableWorld);

            EXPECT(plan.empty());
        }
        {
            World unsolvableWorld;
            unsolvableWorld.Set(property, 43);
            Plan plan = Planner::Find(domain, unsolvableWorld);

            EXPECT(plan.empty());
        }
        {
            World solvableWorld;
            solvableWorld.Set(property, 41);
            Plan plan = Planner::Find(domain, solvableWorld);

            EXPECT(plan.size() == 1);
            EXPECT(plan[0] == &task);
        }
    }

    TEST(Find_plan_on_domain_with_task_with_negated_condition)
    {
        Operation operation;
        Task task(&operation);
        World::Property property = 1;
        task.SetConditions(Not(Condition(property, Predicate::equal, 42)));
        Domain domain(&task);

        {
            World unsolvableWorld;
            unsolvableWorld.Set(property, 42);
            Plan plan = Planner::Find(domain, unsolvableWorld);

            EXPECT(plan.empty());
        }
        {
            World solvableWorld;
            solvableWorld.Set(property, 17);
            Plan plan = Planner::Find(domain, solvableWorld);

            EXPECT(plan.size());
            EXPECT(plan[0] == &task);
        }
    }

    TEST(Find_plan_on_domain_with_task_with_nested_OR_conditions)
    {
        Operation operation;
        Task task(&operation);
        World::Property property = 1;
        task.SetConditions
            (
                Or
                (
                    Or(Condition(property, Predicate::equal, 17), Condition(property, Predicate::equal, 18)),
                    Condition(property, Predicate::equal, 19)
                )
            );
        Domain domain(&task);

        {
            World unsolvableWorld;
            Plan plan = Planner::Find(domain, unsolvableWorld);

            EXPECT(plan.empty());
        }
        {
            World solvableWorld;
            solvableWorld.Set(property, 17);
            Plan plan = Planner::Find(domain, solvableWorld);

            EXPECT(plan.size() == 1);
            EXPECT(plan[0] == &task);
        }
        {
            World solvableWorld;
            solvableWorld.Set(property, 18);
            Plan plan = Planner::Find(domain, solvableWorld);

            EXPECT(plan.size() == 1);
            EXPECT(plan[0] == &task);
        }
        {
            World solvableWorld;
            solvableWorld.Set(property, 19);
            Plan plan = Planner::Find(domain, solvableWorld);

            EXPECT(plan.size() == 1);
            EXPECT(plan[0] == &task);
        }
    }

    TEST(Find_plan_on_domain_with_task_with_nested_AND_conditions)
    {
        Operation operation;
        Task task(&operation);
        World::Property property = 1;
        task.SetConditions
            (
                And
                (
                    And(Condition(property, Predicate::greater, 36), Condition(property, Predicate::less, 42)),
                    Not(Condition(property, Predicate::equal, 37))
                )
            );
        Domain domain(&task);

        {
            World unsolvableWorld;
            Plan plan = Planner::Find(domain, unsolvableWorld);

            EXPECT(plan.empty());
        }
        {
            World unsolvableWorld;
            unsolvableWorld.Set(property, 37);
            Plan plan = Planner::Find(domain, unsolvableWorld);

            EXPECT(plan.empty());
        }
        {
            World unsolvableWorld;
            unsolvableWorld.Set(property, 100);
            Plan plan = Planner::Find(domain, unsolvableWorld);

            EXPECT(plan.empty());
        }
        {
            World solvableWorld;
            solvableWorld.Set(property, 39);
            Plan plan = Planner::Find(domain, solvableWorld);

            EXPECT(plan.size() == 1);
            EXPECT(plan[0] == &task);
        }
    }

    TEST(Find_plan_on_domain_with_recursive_tasks)
    {
        Operation operation;
        Task task(&operation);
        Task check(&operation);
        Task increment(&operation);
        World::Property property = 1;
        check.SetConditions(Just(Condition(property, Predicate::greater, 2)));
        increment.AddEffect(Effects::Add { property, 1 });
        Task increment_and_recurse(&operation);
        Task root(Task::ANY, { &check, &increment_and_recurse });
        increment_and_recurse = Task(Task::ALL, { &increment, &root });
        Domain domain(&root);
        World world;
        Plan plan = Planner::Find(domain, world);

        EXPECT(plan.size() == 4);
        EXPECT(plan[0] == &increment);
        EXPECT(plan[1] == &increment);
        EXPECT(plan[2] == &increment);
        EXPECT(plan[3] == &check);
    }

    namespace MockExecutor
    {
        struct Base : public OperationExecutorInterface
        {
            void Execute(Operation const& operation, Parameters parameters) override
            {
                m_ExecutedOperations.emplace_back(&operation);
                std::copy(parameters.begin(), parameters.end(), m_OperationParameters[&operation].begin());
            }

            void Stop(Operation const& operation) override
            {
                m_StoppedOperations.emplace_back(&operation);
            }

            std::vector<Operation const*> m_ExecutedOperations;
            std::unordered_map<Operation const*, std::array<Parameter::Value, Parameter::MAX_COUNT>> m_OperationParameters;
            std::vector<Operation const*> m_StoppedOperations;
        };

        struct AlwaysSucceed : public Base
        {
            Operation::Result Check(Operation const&) override
            {
                return Operation::Result::success;
            }
        };

        struct AlwaysFail : public Base
        {
            Operation::Result Check(Operation const&) override
            {
                return Operation::Result::failure;
            }
        };

        struct AlwaysRunning : public Base
        {
            Operation::Result Check(Operation const&) override
            {
                return Operation::Result::running;
            }
        };
    }

    TEST(Plan_not_running_before_calling_Run)
    {
        Operation operation;
        Task task(&operation);
        Domain domain(&task);
        World world;
        Plan plan = Planner::Find(domain, world);
        MockExecutor::AlwaysRunning executor;
        Runner runner(&executor);

        EXPECT(!runner.IsRunning());
    }

    TEST(Run_plan_aborted_while_running)
    {
        Operation operation;
        Task task(&operation);
        Domain domain(&task);
        World world;
        Plan plan = Planner::Find(domain, world);
        MockExecutor::AlwaysRunning executor;
        Runner runner(&executor);
        runner.Run(&plan);

        EXPECT(runner.IsRunning());
        {
            runner.Update(world);
            EXPECT(runner.IsRunning());
            EXPECT(executor.m_ExecutedOperations.size() == 1);
            EXPECT(executor.m_ExecutedOperations[0] == &operation);
            EXPECT(executor.m_StoppedOperations.empty());
        }        
        {
            runner.Abort();
            EXPECT(!runner.IsRunning());
            EXPECT(executor.m_ExecutedOperations.size() == 1);
            EXPECT(executor.m_ExecutedOperations[0] == &operation);
            EXPECT(executor.m_StoppedOperations.size() == 1);
            EXPECT(executor.m_StoppedOperations[0] == &operation);
        }
        {
            runner.Update(world);
            EXPECT(!runner.IsRunning());
            EXPECT(executor.m_ExecutedOperations.size() == 1);
            EXPECT(executor.m_ExecutedOperations[0] == &operation);
            EXPECT(executor.m_StoppedOperations.size() == 1);
            EXPECT(executor.m_StoppedOperations[0] == &operation);
        }
        {
            runner.Abort();
            EXPECT(!runner.IsRunning());
            EXPECT(executor.m_ExecutedOperations.size() == 1);
            EXPECT(executor.m_ExecutedOperations[0] == &operation);
            EXPECT(executor.m_StoppedOperations.size() == 1);
            EXPECT(executor.m_StoppedOperations[0] == &operation);
        }
    }

    TEST(Run_plan_with_simple_tasks_and_succeeding_operations)
    {
        Operation operation1;
        Operation operation2;
        Operation operation3;
        Task task1(&operation1);
        Task task2(&operation2);
        Task task3(&operation3);
        Task root(Task::ALL, { &task1, &task2, &task3 });
        Domain domain(&root);
        World world;
        Plan plan = Planner::Find(domain, world);
        MockExecutor::AlwaysSucceed executor;
        Runner runner(&executor);
        runner.Run(&plan);

        EXPECT(runner.IsRunning());
        {
            runner.Update(world);
            EXPECT(runner.IsRunning());
            EXPECT(executor.m_ExecutedOperations.size() == 1);
            EXPECT(executor.m_ExecutedOperations[0] == &operation1);
            EXPECT(executor.m_StoppedOperations.size() == 1);
            EXPECT(executor.m_StoppedOperations[0] == &operation1);
        }
        {
            runner.Update(world);
            EXPECT(runner.IsRunning());
            EXPECT(executor.m_ExecutedOperations.size() == 2);
            EXPECT(executor.m_ExecutedOperations[1] == &operation2);
            EXPECT(executor.m_StoppedOperations.size() == 2);
            EXPECT(executor.m_StoppedOperations[1] == &operation2);
        }
        {
            runner.Update(world);
            EXPECT(!runner.IsRunning());
            EXPECT(executor.m_ExecutedOperations.size() == 3);
            EXPECT(executor.m_ExecutedOperations[2] == &operation3);
            EXPECT(executor.m_StoppedOperations.size() == 3);
            EXPECT(executor.m_StoppedOperations[2] == &operation3);
        }
        {
            runner.Update(world);
            EXPECT(!runner.IsRunning());
            EXPECT(executor.m_ExecutedOperations.size() == 3);
            EXPECT(executor.m_ExecutedOperations[2] == &operation3);
            EXPECT(executor.m_StoppedOperations.size() == 3);
            EXPECT(executor.m_StoppedOperations[2] == &operation3);
        }
    }

    TEST(Run_plan_with_simple_tasks_and_failing_operations)
    {
        Operation operation1;
        Operation operation2;
        Operation operation3;
        Task task1(&operation1);
        Task task2(&operation2);
        Task task3(&operation3);
        Task root(Task::ALL, { &task1, &task2, &task3 });
        Domain domain(&root);
        World world;
        Plan plan = Planner::Find(domain, world);
        MockExecutor::AlwaysFail executor;
        Runner runner(&executor);
        runner.Run(&plan);

        EXPECT(runner.IsRunning());
        {
            runner.Update(world);
            EXPECT(!runner.IsRunning());
            EXPECT(executor.m_ExecutedOperations.size() == 1);
            EXPECT(executor.m_ExecutedOperations[0] == &operation1);
            EXPECT(executor.m_StoppedOperations.size() == 1);
            EXPECT(executor.m_StoppedOperations[0] == &operation1);
        }
    }

    TEST(Run_plan_with_task_and_parameters)
    {
        Operation operation;
        Task task(&operation);
        task.SetParameter(0, 17);
        task.SetParameter(4, 99);
        task.SetParameter(Parameter::MAX_COUNT - 1, 36);
        Domain domain(&task);
        World world;
        Plan plan = Planner::Find(domain, world);
        MockExecutor::AlwaysSucceed executor;
        Runner runner(&executor);
        runner.Run(&plan);

        EXPECT(runner.IsRunning());
        {
            runner.Update(world);
            EXPECT(executor.m_OperationParameters.size() == 1);
            EXPECT(executor.m_OperationParameters.find(&operation) != executor.m_OperationParameters.end());
            EXPECT(executor.m_OperationParameters.at(&operation).front() == 17);
            for (int i = 1; i < Parameter::MAX_COUNT - 1; ++i)
            {
                EXPECT(executor.m_OperationParameters.at(&operation)[i] == (i == 4 ? 99 : 0));
            }
            EXPECT(executor.m_OperationParameters.at(&operation).back() == 36);
        }
    }

    TEST(Run_plan_while_sensor_enables_a_task)
    {
        Operation operation;
        Task task1(&operation);
        Task task2(&operation);
        World::Property property = 1;
        task1.AddExpectation(property, 17);
        Task root(Task::ALL, { &task1, &task2 });
        Domain domain(&root);
        World world;
        Plan plan = Planner::Find(domain, world);
        std::unique_ptr<Sensor> sensor = Sensor::From([=](World& world) { world.Set(property, 17); });
        MockExecutor::AlwaysSucceed executor;
        Runner runner(&executor);
        runner.Run(&plan);

        EXPECT(runner.IsRunning());
        {
            runner.Update(world);
            EXPECT(world.Get(property) == 0);
        }
        {
            sensor->Update(world);
            EXPECT(world.Get(property) == 17);
        }
        {
            runner.Update(world);
            EXPECT(!runner.IsRunning());
            EXPECT(executor.m_ExecutedOperations.size() == 2);
            EXPECT(executor.m_StoppedOperations.size() == 2);
        }
    }

    TEST(Run_plan_while_sensor_disables_a_task)
    {
        Operation operation;
        Task task1(&operation);
        Task task2(&operation);
        World::Property property = 1;
        task2.SetConditions(Just(Condition(property, Predicate::equal, 17)));
        Task root(Task::ALL, { &task1, &task2 });
        Domain domain(&root);
        World world;
        world.Set(property, 17);
        Plan plan = Planner::Find(domain, world);
        std::unique_ptr<Sensor> sensor = Sensor::From([=](World& world) { world.Set(property, 36); });
        MockExecutor::AlwaysSucceed executor;
        Runner runner(&executor);
        runner.Run(&plan);

        EXPECT(runner.IsRunning());
        {
            runner.Update(world);
            EXPECT(world.Get(property) == 17);
        }
        {
            sensor->Update(world);
            EXPECT(world.Get(property) != 17);
        }
        {
            runner.Update(world);
            EXPECT(!runner.IsRunning());
            EXPECT(executor.m_ExecutedOperations.size() == 1);
            EXPECT(executor.m_StoppedOperations.size() == 1);
        }
    }
}

int main()
{
    GHTNTest::RunAllTests();
}
