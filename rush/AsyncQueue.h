#ifndef __INCLUDE_ASYNCQUEUE_H__
#define __INCLUDE_ASYNCQUEUE_H__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <iostream>
#include <deque>
#include <Windows.h>

using namespace std;

const int AQ_SUCCESS = 0;
const int AQ_ENQUEUE_BACK_ERROR = 1;
const int AQ_ENQUEUE_FRONT_ERROR = 2;
const int AQ_DEQUEUE_BACK_ERROR = 3;
const int AQ_DEQUEUE_FRONT_ERROR = 4;
const int AQ_EMPTY_QUEUE = 5;
const int AQ_NULL_PROC_FUNC_ERROR = 6;
const int AQ_NULL_PROC_THREAD_ERROR = 7;

typedef class QueueNodeBase
{
protected:
	int type;
public:
	QueueNodeBase(void);
	QueueNodeBase(int);
	inline int getType(void)const
	{
		return type;
	}

	virtual void* getValue(void)const = 0;
	//Queue Node MUST override operator =
	//virtual const QueueNodeBase& operator = (const QueueNodeBase&) = 0;

	//Cope with clone carefully! do not return this directly!
	//when clone, the instance MUST return a NEW instance same as itself
	virtual QueueNodeBase* clone (void);
}IQueueNode,*pIQueueNode;

typedef class AsyncQueueBase
{
protected:
	bool debugFlag;
	HANDLE innerSema;
	HANDLE procSema;
	HANDLE procHandle;
	deque<pIQueueNode> innerDeque;
	
	//User defined queue proc func, will be call when queue is not empty; Message will be sent to this function to cope with;
	int (*pQueueProc)(pIQueueNode);

	//Queue proc thread func, will keep Dequeue_Front and call procFuncPointer;
	static void* __stdcall queueProcFunc(void*);


	explicit AsyncQueueBase(const AsyncQueueBase&);	//not allow to copy
public:
	explicit AsyncQueueBase(int(*)(pIQueueNode));
	
	virtual ~AsyncQueueBase(void);

	int Enqueue_Back(IQueueNode&);
	int Enqueue_Front(IQueueNode&);
	pIQueueNode Dequeue_Back(void);
	pIQueueNode Dequeue_Front(void);

	inline UINT Length(void)const;
	inline bool isEmpty(void)const;
	inline bool isInit(void)const;

	int updateProcFunc(int(*)(pIQueueNode));

	int Clear(void);
}CommandQueue;

#endif