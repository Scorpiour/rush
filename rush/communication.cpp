#include "communication.h"

CommModelBase::CommModelBase(int startupMode)
{
	SOCKET s;
	debugFlag = false;
	mode = startupMode;

	int (__cdecl *pF)(pIQueueNode);

	pF = &(CommModelBase::dealer);

	queuePtr = new CommandQueue(pF);
	pformQueue = nullptr;
}

CommModelBase::~CommModelBase(void)
{
	if(nullptr!=queuePtr)
	{
		delete queuePtr;
	}
}

int CommModelBase::dealer(pIQueueNode ptr)
{
	CommandQueue* qPtr = (CommandQueue*)(ptr->getHeader());
	
	qPtr->Enqueue_Back( *(AsyncMessage*)(ptr) );

	int i=0;
	return 0;
}

int CommModelBase::SendAsyncMessage(System::String^ s,void* header)
{
	//Convert Managed String to Unmanaged wchar_t[]
	int len = s->Length + 1;
	wchar_t* str = new wchar_t[len];

	memset(str,0,len*sizeof(wchar_t));

	for(int i=0;i<s->Length;i++)
	{
		str[i] = s[i];
	}

	AsyncMessage msg(str,header);

	queuePtr->Enqueue_Back(msg);

	delete[] str;
	return 0;
}

AsyncMessage::AsyncMessage(wchar_t* ptr,void* header):IQueueNode(0,header)
{
	size_t len = wcslen(ptr)+1;
	wptr = new wchar_t[len];
	memset(wptr,0,len*sizeof(wchar_t));
	
	wcscpy_s(wptr,len,ptr);
}

AsyncMessage::~AsyncMessage()
{
	if(wptr!=nullptr)
	{
		delete[] wptr;
	}
}

AsyncMessage* AsyncMessage::clone(void)
{
	AsyncMessage* tmp = new AsyncMessage(wptr,header);
	return tmp;
}

void* AsyncMessage::getValue(void)const
{
	return (void*)(wptr);
}

const AsyncMessage& AsyncMessage::operator = (const AsyncMessage& R)
{
	if(&R!=this)
	{
		if(wptr!=nullptr)
		{
			delete[] wptr;
		}
		size_t len = wcslen(R.wptr);
		wptr = new wchar_t[len];
		memset(wptr,0,len*sizeof(wchar_t));

		wcscpy_s(wptr,len,R.wptr);
	}
	return *this;
}