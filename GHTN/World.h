#pragma once

#include <vector>

#include "GHTN.h"

namespace GHTN
{
	class Condition;
}

namespace GHTN
{
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
		std::vector<State> m_Properties;
	};
}

