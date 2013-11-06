#include "i_platform.h"

Event::~Event()
{

}

Event::Event()
:mHandled(false)
{

}

bool Event::IsHandled() const
{
	return mHandled;
}

void Event::SetHandled() const
{
	mHandled=true;
}

