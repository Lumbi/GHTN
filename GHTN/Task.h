#pragma once

#include <initializer_list>
#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "GHTN.h"
#include "Condition.h"
#include "Effect.h"

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
		friend class Planner;
		friend class Debug;

	public:
		enum class Composition
		{
			all,
			any,
		};
		static constexpr Composition ALL = Composition::all;
		static constexpr Composition ANY = Composition::any;

		using SubTaskContainer = std::vector<Task const*>;
		using EffectContainer = std::vector<Effect>;

	public:
		GHTN_API explicit Task(Operation const*);

		GHTN_API explicit Task(Composition, std::initializer_list<Task const*> subTasks);

	public:
		GHTN_API std::string const& GetName() const;

		GHTN_API void SetName(std::string&&);

		GHTN_API void SetConditions(ConditionTree&&);

		GHTN_API void AddEffect(Effect&&);

	public:
		bool IsPrimitive() const;

		bool IsComposite() const;

		Composition GetComposition() const;

		Operation const* GetOperation() const;

		SubTaskContainer const* GetSubTasks() const;

	private:
		std::variant<Operation const*, SubTaskContainer> m_Content;
		Composition m_Composition;
		ConditionTree m_Conditions;
		EffectContainer m_Effects;
		std::string m_Name;
	};
}
