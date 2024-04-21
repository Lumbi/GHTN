#include "World.h"

namespace GHTN
{
	World::World()
		: m_Properties(32) // TODO: Remove magic number and use array instead of vector
	{
	}

	World::State World::Get(Property property) const
	{
		return m_Properties[static_cast<std::size_t>(property)];
	}

	void World::Set(Property property, State state)
	{
		m_Properties[static_cast<std::size_t>(property)] = state;
	}
}
