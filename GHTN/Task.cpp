#include "Task.h"

#include "Condition.h"
#include "Debug.h"
#include "Operation.h"

namespace GHTN
{
	Task::Task(Operation const* operation)
		: m_Content(operation)
		, m_Composition(Task::ALL)
		, m_Conditions()
		, m_Parameters(std::make_unique<ParameterContainer>())
		, m_Effects()
		, m_Name()
	{
	}

	Task::Task(Task::Composition composition)
		: m_Content(SubTaskContainer())
		, m_Composition(composition)
		, m_Conditions()
		, m_Parameters(std::make_unique<ParameterContainer>())
		, m_Effects()
		, m_Name()
	{
	}

	char const* Task::GetName() const
	{
		return m_Name.c_str();
	}

	void Task::SetName(char const* name)
	{
		m_Name = std::string(name);
	}

	void Task::AddSubTask(Task const* subTask)
	{
		if (SubTaskContainer* subTasks = std::get_if<SubTaskContainer>(&m_Content))
		{
			subTasks->emplace_back(subTask);
		} 
		else
		{
			GHTN_ASSERT_FAIL("Only composite tasks can have sub tasks.");
		}
	}

	void Task::SetConditions(ConditionTree&& conditions)
	{
		m_Conditions = std::move(conditions);
	}

	void Task::SetParameter(Parameter::Index index, Parameter::Value value)
	{
		GHTN_ASSERT(index < m_Parameters->size(), "Parameter out of bounds");
		(*m_Parameters)[index] = value;
	}

	void Task::AddEffect(Effect&& effect)
	{
		GHTN_ASSERT(IsPrimitive(), "Only primitive tasks can have effects");
		m_Effects.emplace_back(std::move(effect));
	}

	void Task::AddExpectation(World::Property property, World::State state)
	{
		m_Expectations[property] = state;
	}

	bool Task::IsPrimitive() const
	{
		return std::holds_alternative<Operation const*>(m_Content);
	}

	Operation const* Task::GetOperation() const
	{
		return *std::get_if<Operation const*>(&m_Content);
	}

	Task::ParameterContainer const& Task::GetParameters() const
	{
		return *m_Parameters;
	}

	bool Task::IsComposite() const
	{
		return std::holds_alternative<SubTaskContainer>(m_Content);
	}

	Task::Composition Task::GetComposition() const
	{
		return m_Composition;
	}

	Task::SubTaskContainer const* Task::GetSubTasks() const
	{
		return std::get_if<SubTaskContainer>(&m_Content);
	}

	bool Task::CanExecute(World const& world) const
	{
		return m_Conditions.Check(world);
	}

	void Task::ApplyEffects(World& world) const
	{
		for (auto&& effect : m_Effects)
		{
			std::visit([&](auto&& each) { each(world); }, effect);
		}
	}

	void Task::AssumeExpectations(World& world) const
	{
		for (auto&& expectation : m_Expectations)
		{
			auto&& [property, state] = expectation;
			world.Set(property, state);
		}
	}
}
