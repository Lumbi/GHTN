#pragma once

#include "GHTN.h"

#include "Condition.h"
#include "Effect.h"
#include "Parameter.h"

namespace GHTN
{
	class Operation;
	class Condition;
	class Task;
	class World;
}

namespace GHTN
{
	class Task
	{
		friend class Debug;

	public:
		enum class Composition { all, any };
		static constexpr Composition ALL = Composition::all;
		static constexpr Composition ANY = Composition::any;

		using SubTaskContainer = std::vector<Task const*>;
		using ParameterContainer = std::array<Parameter::Value, Parameter::MAX_COUNT>;
		using EffectContainer = std::vector<Effect>;
		using ExpectationContainer = std::unordered_map<World::Property, World::State>;

	public:
		GHTN_API explicit Task(Operation const*);

	private:
		GHTN_API explicit Task(Composition);

	public:
		template<typename TaskContainerType= std::initializer_list<Task const*>>
		Task(Task::Composition composition, TaskContainerType subTasks)
			: Task(composition)
		{
			m_Content = SubTaskContainer(std::begin(subTasks), std::end(subTasks));
		}

		~Task() = default;

	public:
		GHTN_API Task(Task&&) noexcept = default;

		GHTN_API Task& operator=(Task&&) noexcept = default;

	public:
		GHTN_API char const* GetName() const;

		GHTN_API void SetName(char const*);

		GHTN_API void SetConditions(ConditionTree&&);

		GHTN_API void SetParameter(Parameter::Index, Parameter::Value);

		GHTN_API void AddEffect(Effect&&);

		GHTN_API void AddExpectation(World::Property, World::State);

	public:
		bool IsPrimitive() const;

		Operation const* GetOperation() const;

		ParameterContainer const& GetParameters() const;

		bool IsComposite() const;

		Composition GetComposition() const;

		SubTaskContainer const* GetSubTasks() const;

	public:
		bool CanExecute(World const&) const;

		void ApplyEffects(World&) const;

		void AssumeExpectations(World&) const;

	private:
		std::variant<Operation const*, SubTaskContainer> m_Content;
		Composition m_Composition;
		ConditionTree m_Conditions;
		std::unique_ptr<ParameterContainer> m_Parameters;
		EffectContainer m_Effects;
		ExpectationContainer m_Expectations;
		std::string m_Name;
	};
}
