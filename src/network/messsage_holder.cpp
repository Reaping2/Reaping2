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

void MessageHolder::ClearOutgoingMessages()
{
    mOutgoingMessages.mMessages.clear();
}

void MessageHolder::ClearIncomingMessages()
{
    mIncomingMessages.mMessages.clear();
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
    //std::unique_lock<std::mutex> ulock( mMutex );
    mPublishedMessages.transfer( mPublishedMessages.end(), mMessages );
    //mCV.notify_all();
}

void MessageList::TransferPublishedMessagesTo( Messages_t& messages )
{
    messages.transfer( messages.end(), mPublishedMessages );
}

std::mutex& MessageList::GetMutex()
{
    return mMutex;
}

std::condition_variable& MessageList::GetCV()
{
    return mCV;
}

// void MessageList::Wait( std::chrono::milliseconds millisecs )
// {
//     std::unique_lock<std::mutex> ulock( mMutex );
//     mCV.wait_for( ulock, millisecs );
// }

} // namespace network
