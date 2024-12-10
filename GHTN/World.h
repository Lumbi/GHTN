#pragma once

#include "GHTN.h"

namespace GHTN
{
	class Condition;
}

namespace GHTN
{
	static constexpr std::size_t MAX_WORLD_PROPERTY_COUNT = 32;

	class World
	{
	public:
		using Property = std::uint8_t;
		using State = std::int8_t;

	public:
		GHTN_API World();

	public:
		GHTN_API State Get(Property) const;
		GHTN_API void Set(Property, State);

	private:
		std::array<State, MAX_WORLD_PROPERTY_COUNT> m_Properties;
	};
}
