#include "Runner.h"

#include "Debug.h"
#include "Planner.h"
#include "Task.h"
#include "World.h"

namespace GHTN
{
	Runner::Runner(OperationExecutorInterface* operationExecutor)
		: m_OperationExecutor(operationExecutor)
	{
		GHTN_ASSERT(m_OperationExecutor != nullptr, "Must have a valid operation executor");
	}

	void Runner::Run(Plan const* plan)
	{
		Abort();
		m_Plan = plan;
		m_CurrentTask = m_Plan->cbegin();
		m_CurrentOperation = nullptr;
	}

	void Runner::Abort()
	{
		if (m_CurrentOperation)
		{
			m_OperationExecutor->Stop(*m_CurrentOperation);
			m_CurrentOperation = nullptr;
		}

		if (IsRunning())
		{			
			m_CurrentTask = m_Plan->cend();
		}
	}

	bool Runner::IsRunning() const
	{
		return m_Plan && m_CurrentTask != m_Plan->cend();
	}

	void Runner::Update(World& world)
	{
		if (IsRunning())
		{
			if (!m_CurrentOperation)
			{
				Task const& currentTask = **m_CurrentTask;
				if (currentTask.CanExecute(world))
				{
					m_CurrentOperation = currentTask.GetOperation();
					m_OperationExecutor->Execute(*m_CurrentOperation, currentTask.GetParameters().data());
				}
				else
				{
					Abort();
				}
			}

			if (m_CurrentOperation)
			{
				Operation::Result const result = m_OperationExecutor->Check(*m_CurrentOperation);
				switch (result)
				{
				case Operation::Result::running: break;
				case Operation::Result::success: AdvanceToNextTask(world); break;
				case Operation::Result::failure: Abort(); break;
				}
			}
		}
	}

	void Runner::AdvanceToNextTask(World& world)
	{
		if (Task const* currentTask = *m_CurrentTask)
		{
			currentTask->ApplyEffects(world);
		}

		if (m_CurrentOperation)
		{
			m_OperationExecutor->Stop(*m_CurrentOperation);
		}

		m_CurrentOperation = nullptr;
		++m_CurrentTask;
	}
}
