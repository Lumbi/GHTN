#pragma once

#include "GHTN.h"

namespace GHTN
{
	class Operation
	{
		friend class Debug;

	public:
		using ID = uint16_t;

		enum class Result
		{
			running,
			success,
			failure
		};

	public:
		GHTN_API explicit Operation();

	public:
		GHTN_API ID GetID() const;

		GHTN_API char const* GetName() const;

		GHTN_API void SetName(char const*);

	private:
		static ID s_ID;
		ID m_ID;
		std::string m_Name;
	};
}
