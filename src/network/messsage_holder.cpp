#include "messsage_holder.h"
#include <mutex>

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

void MessageList::Add( std::auto_ptr<Message> message )
{
    mMessages.push_back( message );
}

void MessageList::TransferFrom( Messages_t& messages )
{
    mMessages.transfer( mMessages.end(), messages );
}

void MessageList::Publish()
{
    mPublishedMessages.transfer( mPublishedMessages.end(), mMessages );
}

void MessageList::TransferPublishedMessagesTo( Messages_t& messages )
{
    messages.transfer( messages.end(), mPublishedMessages );
}


bool MessageList::HasPublishedMessages() const
{
    return !mPublishedMessages.empty();
}

std::mutex& MessageList::GetMutex()
{
    return mMutex;
}

std::condition_variable& MessageList::GetCV()
{
    return mCV;
}

} // namespace network
