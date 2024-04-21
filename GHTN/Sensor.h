#pragma once

#include "GHTN.h"

namespace GHTN
{
	class World;
}

namespace GHTN
{
	class Sensor
	{
	public:
		GHTN_API static std::unique_ptr<Sensor> From(std::function<void(World&)>);

		virtual ~Sensor() {}

	public:
		virtual void Update(World&) = 0;
	};
}

