#include "AsyncQueue.h"

QueueNodeBase::QueueNodeBase(void)
{
}

QueueNodeBase::QueueNodeBase(int t,void* ptr)
{
	header = ptr;
	type = t;
}

QueueNodeBase* QueueNodeBase::clone(void)
{
	return nullptr;
}

AsyncQueueBase::AsyncQueueBase(int(__clrcall *pF)(pIQueueNode))
{
	innerSema = CreateSemaphore(nullptr,(LONG)(1),(LONG)1,nullptr);
	procSema = CreateSemaphore(nullptr,(LONG)(0),(LONG)(INFINITE),nullptr);

	pQueueProc = nullptr;
	pClrQueueProc = pF;

	queueType = false;

	if(pF!=nullptr)
	{
		procHandle = CreateThread(nullptr,(DWORD)(NULL),(LPTHREAD_START_ROUTINE)(AsyncQueueBase::queueProcFunc),(void*)(this),(DWORD)(NULL),nullptr);
	}
	else
	{
		procHandle = nullptr;
	}
	if(NULL==procHandle)
	{
		procHandle = nullptr;

	}
}

AsyncQueueBase::AsyncQueueBase(int(*pF)(pIQueueNode))
{
	innerSema = CreateSemaphore(nullptr,(LONG)(1),(LONG)1,nullptr);
	procSema = CreateSemaphore(nullptr,(LONG)(0),(LONG)(INFINITE),nullptr);

	pQueueProc = pF;
	pClrQueueProc = nullptr;

	queueType = true;

	if(pF!=nullptr)
	{
		procHandle = CreateThread(nullptr,(DWORD)(NULL),(LPTHREAD_START_ROUTINE)(AsyncQueueBase::queueProcFunc),(void*)(this),(DWORD)(NULL),nullptr);
	}
	else
	{
		procHandle = nullptr;
	}
	if(NULL==procHandle)
	{
		procHandle = nullptr;

	}
}

void* AsyncQueueBase::queueProcFunc(void* ptr)
{
	AsyncQueueBase* instance = (AsyncQueueBase*)(ptr);

	while(true)
	{
		WaitForSingleObject(instance->procSema,(DWORD)INFINITE);
		
		pIQueueNode tmp = instance->Dequeue_Front();

		if(tmp != nullptr)
		{
			if(instance->queueType && nullptr!=(instance->pQueueProc))
			{
				int ret = (*(instance->pQueueProc))(tmp);
				delete tmp;//delete the data after use;
			}
			else if( !(instance->queueType) && nullptr!=(instance->pClrQueueProc))
			{
				int ret = (*(instance->pClrQueueProc))(tmp);
				delete tmp;
			}
		}
	}
	ExitThread((DWORD)NULL);
}
	
AsyncQueueBase::~AsyncQueueBase(void)
{
	TerminateThread(procHandle,(DWORD)NULL);
	Clear();
}

int AsyncQueueBase::Enqueue_Back(IQueueNode& node)
{
	try
	{
		WaitForSingleObject(innerSema,(DWORD)INFINITE);

		innerDeque.push_back(node.clone());

		ReleaseSemaphore(procSema,(LONG)1,NULL);
		ReleaseSemaphore(innerSema,(LONG)1,NULL);
	}
	catch(...)
	{
		ReleaseSemaphore(innerSema,(LONG)1,NULL);
		return AQ_ENQUEUE_BACK_ERROR;
	}
	return AQ_SUCCESS;
}

int AsyncQueueBase::Enqueue_Front(IQueueNode& node)
{
	try
	{
		WaitForSingleObject(innerSema,(DWORD)INFINITE);

		innerDeque.push_front(node.clone());

		ReleaseSemaphore(procSema,(LONG)1,NULL);
		ReleaseSemaphore(innerSema,(LONG)1,NULL);
	}
	catch(...)
	{
		ReleaseSemaphore(innerSema,(LONG)1,NULL);
		return AQ_ENQUEUE_FRONT_ERROR;
	}
	return AQ_SUCCESS;
}

pIQueueNode AsyncQueueBase::Dequeue_Back(void)
{
	if(isEmpty())
	{
		return nullptr;
	}
	try
	{
		WaitForSingleObject(innerSema,(DWORD)INFINITE);

		pIQueueNode tmp = innerDeque.back();	
		innerDeque.pop_back();

		ReleaseSemaphore(innerSema,(LONG)1,NULL);

		return tmp;
	}
	catch(...)
	{
		ReleaseSemaphore(innerSema,(DWORD)1,NULL);
		return nullptr;
	}
}

pIQueueNode AsyncQueueBase::Dequeue_Front(void)
{
	if(isEmpty())
	{
		return nullptr;
	}
	try
	{
		WaitForSingleObject(innerSema,(DWORD)INFINITE);

		pIQueueNode tmp = innerDeque.front();	
		innerDeque.pop_front();

		ReleaseSemaphore(innerSema,(LONG)1,NULL);

		return tmp;
	}
	catch(...)
	{
		ReleaseSemaphore(innerSema,(LONG)1,NULL);
		return nullptr;
	}
}

UINT AsyncQueueBase::Length(void)const
{
	return innerDeque.size();
}

bool AsyncQueueBase::isEmpty(void)const
{
	return (innerDeque.size()==0);
}

bool AsyncQueueBase::isInit(void)const
{
	return (pQueueProc == nullptr)||(procHandle == nullptr);
}

int AsyncQueueBase::updateProcFunc(int(*pF)(pIQueueNode))
{
	if(nullptr==procHandle)
	{
		procHandle = CreateThread(nullptr,(DWORD)(NULL),(LPTHREAD_START_ROUTINE)(AsyncQueueBase::queueProcFunc),(void*)(this),(DWORD)(NULL),nullptr);
	}

	if(NULL==procHandle || nullptr == procHandle)
	{
		return AQ_NULL_PROC_THREAD_ERROR;
	}

	if(NULL==pF || nullptr == pF)
	{
		return AQ_NULL_PROC_FUNC_ERROR;
	}

	//pause queue proc thread to update new proc function
	SuspendThread(procHandle);
	queueType = true;
	pQueueProc = pF;
	ResumeThread(procHandle);

	return AQ_SUCCESS;
}

int AsyncQueueBase::updateProcFunc(int(__clrcall *pF)(pIQueueNode))
{
	if(nullptr==procHandle)
	{
		procHandle = CreateThread(nullptr,(DWORD)(NULL),(LPTHREAD_START_ROUTINE)(AsyncQueueBase::queueProcFunc),(void*)(this),(DWORD)(NULL),nullptr);
	}

	if(NULL==procHandle || nullptr == procHandle)
	{
		return AQ_NULL_PROC_THREAD_ERROR;
	}

	if(NULL==pF || nullptr == pF)
	{
		return AQ_NULL_PROC_FUNC_ERROR;
	}

	//pause queue proc thread to update new proc function
	SuspendThread(procHandle);
	queueType = false;
	pClrQueueProc = pF;
	ResumeThread(procHandle);

	return AQ_SUCCESS;
}

int AsyncQueueBase::Clear(void)
{
	try
	{
		WaitForSingleObject(innerSema,(LONG)INFINITE);

		while(innerDeque.size())
		{
			pIQueueNode tmp = innerDeque.front();
			innerDeque.pop_front();
			delete tmp;
		}

		ReleaseSemaphore(innerSema,(LONG)1,NULL);
	}
	catch(...)
	{
		ReleaseSemaphore(innerSema,(LONG)1,NULL);
	}
	return AQ_SUCCESS;;
}