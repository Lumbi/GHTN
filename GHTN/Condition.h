#pragma once

#include "GHTN.h"
#include "World.h"

#include <unordered_map>
#include <memory>
#include <variant>
#include <functional>

namespace GHTN
{
	enum class Predicate : char
	{
		equal,
		less,
		greater,
	};

	class Condition
	{
	public:
		GHTN_API Condition
		(
			World::Property property, 
			Predicate predicate, 
			World::State expected,
			bool negate = false // TODO: Add constexpr symbol name for negation
		);

	public:
		GHTN_API void Negate();

		GHTN_API bool Check(World const&) const;

	private:
		World::Property m_Property;
		Predicate m_Predicate;
		World::State m_Expected;
		bool m_Negate : 1;
	};
}

namespace GHTN
{
	namespace ConditionTreeBuilder
	{
		class NodeBuilder;
	}

	class ConditionTree
	{
	public:
		using Index = std::size_t;
		enum class Logic : bool { AND, OR };
		using Node = std::variant<std::nullptr_t, Logic, Condition>;
		static constexpr Index INVALID_INDEX = static_cast<Index>(-1);

	public:
		ConditionTree() = default;

		GHTN_API ConditionTree(ConditionTreeBuilder::NodeBuilder buildRoot);

	public:
		void Set(Index, Node);

		bool Check(World const&) const;

	private:
		bool RecursiveCheck(Index, World const&) const;

	private:
		std::unordered_map<Index, Node> m_Tree;
	};

	namespace ConditionTreeBuilder
	{
		GHTN_API NodeBuilder Just(Condition&&);

		GHTN_API NodeBuilder Not(Condition&&);

		GHTN_API NodeBuilder And(NodeBuilder&& buildLeft, NodeBuilder&& buildRight);

		GHTN_API NodeBuilder Or(NodeBuilder&& buildLeft, NodeBuilder&& buildRight);

		class NodeBuilder
		{
		public:
			GHTN_API NodeBuilder(Condition&& condition) { *this = Just(std::move(condition)); }

			GHTN_API NodeBuilder(std::function<ConditionTree::Node(ConditionTree::Index, ConditionTree&)>&& build) : m_Build(std::move(build)) {}

			GHTN_API NodeBuilder(NodeBuilder const&) = default;

			GHTN_API NodeBuilder& operator=(NodeBuilder const&) = default;

			GHTN_API NodeBuilder(NodeBuilder&&) = default;

			GHTN_API NodeBuilder& operator=(NodeBuilder&&) = default;

			ConditionTree::Node operator()(ConditionTree::Index index, ConditionTree& tree) const { return m_Build(index, tree); }

		private:
			std::function<ConditionTree::Node(ConditionTree::Index, ConditionTree&)> m_Build;
		};
	}
}
