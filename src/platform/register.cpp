#include "i_platform.h"

Registration::~Registration()
{
	Unregister();
}

Registration::Registration( Registration& O ) :mData(NULL),mRegister(NULL)
{
	*this=O;
}

Registration::Registration( Registry* Parent, void* Data )
: mRegister(Parent)
, mData(Data)
{

}

Registration::Registration()
: mRegister(NULL)
, mData(NULL)
{

}

Registration& Registration::operator=( Registration& O )
{
	if(this!=&O)
	{
		std::swap(mData,O.mData);
		std::swap(mRegister,O.mRegister);
	}
	return *this;
}

void Registration::Unregister()
{
	if(mRegister)
		mRegister->Unregister(this);
	delete mData;
}


Registration Registry::Register(void *Data)
{
	Registration Reg(this,Data);
	mRegistrations.push_back(Data);
	return Reg;
}

void Registry::Unregister( Registration* Reg )
{
	mRegistrations.erase(std::find(mRegistrations.begin(),mRegistrations.end(),Reg->GetData()));
}
