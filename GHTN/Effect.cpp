#include "Effect.h"

namespace GHTN
{
	void Effects::Set::operator()(World& world) const
	{
		world.Set(property, value);
	}

	void Effects::Add::operator()(World& world) const
	{
		world.Set(property, world.Get(property) + value);
	}

	void Effects::Substract::operator()(World& world) const
	{
		world.Set(property, world.Get(property) - value);
	}
}
