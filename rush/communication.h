#ifndef __INCLUDE_COMMUNICATION_H__
#define __INCLUDE_COMMUNICATION_H__

#pragma once

#include <iostream>
#include <fstream>
#include <winsock.h>
#include <Windows.h>
#include <cstdlib>
#include "AsyncQueue.h"

class AsyncMessage:public IQueueNode
{
public:
	AsyncMessage(wchar_t*,void*);
	~AsyncMessage();
	wchar_t* wptr;

	virtual void* getValue(void)const;
	virtual const AsyncMessage& operator = (const AsyncMessage&);
	virtual AsyncMessage* clone(void);
};

typedef class CommModelBase
{
protected:
	int mode;
	bool debugFlag;
	CommandQueue* queuePtr;
	CommandQueue* pformQueue;
public:
	explicit CommModelBase(int);
	virtual ~CommModelBase(void);

	static int __cdecl dealer(pIQueueNode);

	int SendAsyncMessage(System::String^ s,void* header);
}Communcation,*pCommunication;


#endif