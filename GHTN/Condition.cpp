#include "Condition.h"

namespace GHTN
{
	Condition::Condition
	(
		World::Property property,
		Predicate predicate,
		World::State expected,
		bool negate
	)
		: m_Property(property)
		, m_Predicate(predicate)
		, m_Expected(expected)
		, m_Negate(negate)
	{
	}

	void Condition::Negate()
	{
		m_Negate = !m_Negate;
	}

	bool Condition::Check(World const& world) const
	{
		switch (m_Predicate)
		{
		case Predicate::equal: return m_Negate ^ (world.Get(m_Property) == m_Expected);
		case Predicate::less: return m_Negate ^ (world.Get(m_Property) < m_Expected);
		case Predicate::greater: return m_Negate ^ (world.Get(m_Property) > m_Expected);
		default: throw;
		}
	}
}

namespace GHTN
{
	static constexpr ConditionTree::Index Left(ConditionTree::Index index) noexcept { return 2 * index + 1; };
	static constexpr ConditionTree::Index Right(ConditionTree::Index index) noexcept { return 2 * index + 2; };

	ConditionTree::ConditionTree(ConditionTreeBuilder::NodeBuilder buildRoot)
	{
		m_Tree[0] = buildRoot(0, *this);
	}

	void ConditionTree::Set(Index index, Node node)
	{
		m_Tree[index] = std::move(node);
	}

	bool ConditionTree::Check(World const& world) const
	{
		if (m_Tree.empty()) 
		{
			return true; 
		}
		else
		{
			return RecursiveCheck(0, world);
		}
	}

	bool ConditionTree::RecursiveCheck(Index index, World const& world) const
	{
		auto&& found = m_Tree.find(index);
		if (found != m_Tree.end())
		{
			Node const& node = found->second;
			auto visitor = [this, index, &world](auto&& value) -> bool {
				using ValueType = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<ValueType, std::nullptr_t>)
				{
					return false;
				}
				else if constexpr (std::is_same_v<ValueType, Condition>)
				{
					return value.Check(world);
				}
				else if constexpr (std::is_same_v<ValueType, Logic>)
				{
					switch (value)
					{
					case Logic::AND: 
						return RecursiveCheck(Left(index), world) && RecursiveCheck(Right(index), world);
					case Logic::OR:
						return RecursiveCheck(Left(index), world) || RecursiveCheck(Right(index), world);
					default: 
						// TODO: Assert
						return false;
					}
				}
			};
			return std::visit(visitor, node);
		}
		else
		{
			// TODO: Assert
			return false;
		}
	}
}

namespace GHTN::ConditionTreeBuilder
{
	NodeBuilder Just(Condition&& condition)
	{
		return NodeBuilder
			(
				[condition = std::move(condition)]
				(ConditionTree::Index index, ConditionTree& tree) -> ConditionTree::Node
				{
					tree.Set(index, condition);
					return condition;
				}
			);
	}

	NodeBuilder Not(Condition&& condition)
	{
		condition.Negate();
		return Just(std::move(condition));
	}

	NodeBuilder And(NodeBuilder&& buildLeft, NodeBuilder&& buildRight)
	{
		return NodeBuilder
			(
				[buildLeft = std::move(buildLeft), buildRight = std::move(buildRight)]
				(ConditionTree::Index index, ConditionTree& tree) -> ConditionTree::Node
				{
					tree.Set(index, ConditionTree::Logic::AND);
					tree.Set(Left(index), buildLeft(Left(index), tree));
					tree.Set(Right(index), buildRight(Right(index), tree));
					return ConditionTree::Logic::AND;
				}
			);
	}

	NodeBuilder Or(NodeBuilder&& buildLeft, NodeBuilder&& buildRight)
	{
		return NodeBuilder
			(
				[buildLeft = std::move(buildLeft), buildRight = std::move(buildRight)]
				(ConditionTree::Index index, ConditionTree& tree) -> ConditionTree::Node
				{
					tree.Set(index, ConditionTree::Logic::OR);
					tree.Set(Left(index), buildLeft(Left(index), tree));
					tree.Set(Right(index), buildRight(Right(index), tree));
					return ConditionTree::Logic::OR;
				}
			);
	}
}