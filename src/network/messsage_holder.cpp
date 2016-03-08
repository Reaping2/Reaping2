#include "messsage_holder.h"
namespace network {



MessageHolder::MessageHolder()
{
}

MessageList& MessageHolder::GetOutgoingMessages()
{
    return mOutgoingMessages;
}

MessageList& MessageHolder::GetIncomingMessages()
{
    return mIncomingMessages;
}

void MessageHolder::AddIncomingMessage( std::auto_ptr<Message> message )
{
    mIncomingMessages.mMessages.push_back( message );
}

void MessageHolder::ClearOutgoingMessages()
{
    mOutgoingMessages.mMessages.clear();
}

void MessageHolder::ClearIncomingMessages()
{
    mIncomingMessages.mMessages.clear();
}

} // namespace network