#pragma once

#include "GHTN.h"

#include "Effecting.h"

namespace GHTN
{
	namespace Effects
	{
		struct GHTN_API Identity { void operator()(World&) const {}; };

		struct GHTN_API Set
		{
			World::Property property;
			World::State value;

			void operator()(World& world) const;
		};

		struct GHTN_API Add
		{
			World::Property property;
			World::State value;

			void operator()(World& world) const;
		};

		struct GHTN_API Substract
		{
			World::Property property;
			World::State value;

			void operator()(World& world) const;
		};
	}
}

namespace GHTN
{
	using Effect = EffectingVariant
		<
			Effects::Identity,
			Effects::Set,
			Effects::Add,
			Effects::Substract
		>;
}
