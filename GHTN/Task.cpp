#include "Task.h"

#include <algorithm>

#include "Condition.h"
#include "Operation.h"

namespace GHTN
{
	Task::Task(Operation const* operation)
		: m_Content(operation)
		, m_Composition(Task::ALL)
		, m_Conditions()
		, m_Parameters()
		, m_Effects()
		, m_Name()
	{
	}

	Task::Task(Task::Composition composition, std::initializer_list<Task const*> subTasks)
		: m_Content(subTasks)
		, m_Composition(composition)
		, m_Conditions()
		, m_Parameters()
		, m_Effects()
		, m_Name()
	{
	}

	std::string const& Task::GetName() const
	{
		return m_Name;
	}

	void Task::SetName(std::string&& name)
	{
		m_Name = std::move(name);
	}

	void Task::SetConditions(ConditionTree&& conditions)
	{
		m_Conditions = std::move(conditions);
	}

	void Task::SetParameter(Parameter::Index index, Parameter::Value value)
	{
		// TODO: Assert if index out of bounds
		m_Parameters[index] = value;
	}

	void Task::AddEffect(Effect&& effect)
	{
		// TODO: Assert if composite node
		m_Effects.emplace_back(std::move(effect));
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
		return m_Parameters;
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
}
