#include "event.h"

namespace platform {

Event::~Event()
{

}

Event::Event()
    : mHandled( false )
{

}

bool Event::IsHandled() const
{
    return mHandled;
}

void Event::SetHandled() const
{
    mHandled = true;
}

} // namespace platform

