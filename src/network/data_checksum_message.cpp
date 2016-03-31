#include "platform/i_platform.h"
#include "network/data_checksum_message.h"

namespace network {

DataChecksumMessageSenderSystem::DataChecksumMessageSenderSystem()
    : MessageSenderSystem()
{
}


void DataChecksumMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
}


void DataChecksumMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

DataChecksumMessageHandlerSubSystem::DataChecksumMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void DataChecksumMessageHandlerSubSystem::Init()
{
}

void DataChecksumMessageHandlerSubSystem::Update(double DeltaTime)
{
    MessageHandlerSubSystem::Update(DeltaTime);
}

void DataChecksumMessageHandlerSubSystem::Execute(Message const& message)
{
    DataChecksumMessage const& msg=static_cast<DataChecksumMessage const&>(message);
    if ( msg.mClientId == mProgramState.mClientId )
    {
        std::cerr << "source " << msg.mDatasource << " chksum " << msg.mChecksum << std::endl;
    }
}

} // namespace network
REAPING2_CLASS_EXPORT_IMPLEMENT( network__DataChecksumMessage, network::DataChecksumMessage )
