#pragma once

#include "GHTN.h"

namespace GHTN
{
	class Operation
	{
		friend class Debug;

	public:
		enum class Result
		{
			running,
			success,
			failure
		};

	public:
		GHTN_API explicit Operation();

	public:
		GHTN_API char const* GetName() const;

		GHTN_API void SetName(char const*);

	private:
		std::string m_Name;
	};
}
