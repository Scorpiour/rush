#include "communication.h"

CommModelBase::CommModelBase(int startupMode)
{
	SOCKET s;
	debugFlag = false;
	mode = startupMode;

	queuePtr = new CommandQueue(CommModelBase::dealer);
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
	switch((int)(ptr->getValue()))
	{
	case 5:
		int i=1;
		break;
	}
	return 0;
}

int CommModelBase::SendAsyncMessage(System::String^ s)
{
	//Convert Managed String to Unmanaged wchar_t[]
	int len = s->Length + 1;
	wchar_t* str = new wchar_t[len];

	memset(str,0,len*sizeof(wchar_t));

	for(int i=0;i<s->Length;i++)
	{
		str[i] = s[i];
	}

	AsyncMessage msg(str);

	queuePtr->Enqueue_Back(msg);

	delete[] str;
	return 0;
}

AsyncMessage::AsyncMessage(wchar_t* ptr):IQueueNode(0)
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
	AsyncMessage* tmp = new AsyncMessage(wptr);
	return tmp;
}

void* AsyncMessage::getValue(void)const
{
	return (void*)(type + 5);
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