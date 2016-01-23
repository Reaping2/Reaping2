#include "i_target.h"
#include "../actor_factory.h"

namespace map {

ITarget::ITarget(int32_t Id)
    : mId(Id)
{

}

int32_t ITarget::GetCursorId() const
{
    return mId;
}

int32_t ITarget::GetActorId() const
{
    return mId;
}

std::auto_ptr<Actor> ITarget::GetCursor()
{
    return ActorFactory::Get()(GetCursorId());
}

void ITarget::PutTarget(glm::vec2 position, IBorderComponent::Borders_t& borders, IBorderComponent::Borders_t& outerBorders)
{
    return PutTarget(position);
}

ITarget::~ITarget()
{

}


} // namespace map
