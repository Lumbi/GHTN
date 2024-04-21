#include "Sensor.h"

#include "World.h"

namespace GHTN
{
	std::unique_ptr<Sensor> GHTN::Sensor::From(std::function<void(World&)> lambda)
	{
		class SensorLambda : public Sensor
		{
		public:
			SensorLambda(std::function<void(World&)> lambda)
				: m_Lambda(std::move(lambda))
			{}

			void Update(World& world) override
			{
				m_Lambda(world);
			}

		private:
			std::function<void(World&)> m_Lambda;
		};

		return std::make_unique<SensorLambda>(std::move(lambda));
	}
}
