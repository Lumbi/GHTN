#include "Runner.h"

#include "Planner.h"
#include "Task.h"
#include "World.h"

namespace GHTN
{
	Runner::Runner(OperationExecutorInterface* operationExecutor)
		: m_OperationExecutor(operationExecutor)
	{
		// TODO: Assert if m_OperationExecutor is null
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
			m_OperationExecutor->Abort(*m_CurrentOperation);
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

	void Runner::Update(World const& world)
	{
		if (IsRunning())
		{
			if (!m_CurrentOperation)
			{
				Task const& currentTask = **m_CurrentTask;
				if (currentTask.CanExecute(world))
				{
					m_CurrentOperation = currentTask.GetOperation();
				}
				else
				{
					Abort();
				}
			}

			if (m_CurrentOperation)
			{
				Operation::Result const result = m_OperationExecutor->Execute(*m_CurrentOperation);
				switch (result)
				{
				case Operation::Result::running: break;
				case Operation::Result::success: AdvanceToNextTask(); break;
				case Operation::Result::failure: Abort(); break;
				}
			}
		}
	}

	void Runner::AdvanceToNextTask()
	{
		m_CurrentOperation = nullptr;
		++m_CurrentTask;
	}
}
