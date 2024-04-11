#pragma once

#include "GHTN.h"

#include "Planner.h"
#include "Operation.h"

namespace GHTN
{
	class OperationExecutorInterface
	{
	public:
		virtual Operation::Result Execute(Operation const&) = 0;

		virtual void Abort(Operation const&) = 0;
	};

	class Runner
	{
	public:
		GHTN_API Runner(OperationExecutorInterface*);

		GHTN_API void Run(Plan const*);

		GHTN_API void Abort();

		GHTN_API bool IsRunning() const;

		GHTN_API void Update(World const&);

	private:

		void AdvanceToNextTask();

	private:
		Plan const* m_Plan = nullptr;
		Plan::const_iterator m_CurrentTask;
		Operation const* m_CurrentOperation = nullptr;
		OperationExecutorInterface* m_OperationExecutor = nullptr;
	};
}
