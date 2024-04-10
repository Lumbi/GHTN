#pragma once

#include "GHTN.h"

#include <string>

namespace GHTN
{
	class Operation
	{
		friend class Debug;

	public:
		GHTN_API explicit Operation();

	public:
		GHTN_API std::string const& GetName() const;

		GHTN_API void SetName(std::string&&);

	private:
		std::string m_Name;
	};
}
