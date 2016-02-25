#ifndef INCLUDED_NETWORK_SET_TEAM_H
#define INCLUDED_NETWORK_SET_TEAM_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "core/ctf_program_state.h"
#include "engine/soldier_created_event.h"
#include "engine/flag_created_event.h"
#include <boost/serialization/export.hpp>

namespace network {

class SetTeamMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(SetTeamMessage)
    int32_t mActorGUID;
    Team::Type mTeam;
    SetTeamMessage()
        : mActorGUID(0)
        , mTeam(Team::Blue)
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void SetTeamMessage::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Message>(*this);
    ar & mActorGUID;
    ar & mTeam;
}

class SetTeamMessageHandlerSubSystem : public PendingMessageHandlerSubSystem<SetTeamMessage>
{
public:
    DEFINE_SUB_SYSTEM_BASE(SetTeamMessageHandlerSubSystem)
    SetTeamMessageHandlerSubSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
    virtual bool ProcessPending(Message const& message);
};

class SetTeamMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnSoldierCreated;
    void OnSoldierCreated(engine::SoldierCreatedEvent const& Evt);
    AutoReg mOnFlagCreated;
    void OnFlagCreated(engine::FlagCreatedEvent const& Evt);
public:
    DEFINE_SYSTEM_BASE(SetTeamMessageSenderSystem)
    SetTeamMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
    static std::auto_ptr<SetTeamMessage> GenerateSetTeamMessage(Actor &actor);
};
} // namespace network


BOOST_CLASS_EXPORT_KEY2(network::SetTeamMessage,"set_team");
#endif//INCLUDED_NETWORK_SET_TEAM_H

//command:  "classgenerator.exe" -g "message" -c "set_team" -m "Team::Type-team" -e "engine-soldierCreated" -p "pending" -t "team"
