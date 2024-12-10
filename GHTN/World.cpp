#include "World.h"

#include "Debug.h"

namespace GHTN
{
	World::World()
		: m_Properties()
	{
	}

	World::State World::Get(Property property) const
	{
		GHTN_ASSERT(property < MAX_WORLD_PROPERTY_COUNT, "World property index out of bounds");
		return m_Properties[static_cast<std::size_t>(property)];
	}

	void World::Set(Property property, State state)
	{
		GHTN_ASSERT(property < MAX_WORLD_PROPERTY_COUNT, "World property index out of bounds");
		m_Properties[static_cast<std::size_t>(property)] = state;
	}
}
