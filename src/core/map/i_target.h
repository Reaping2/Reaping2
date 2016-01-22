#ifndef INCLUDED_CORE_MAP_I_TARGET_H
#define INCLUDED_CORE_MAP_I_TARGET_H

#include "platform/i_platform.h"
#include "../actor.h"
#include "../border_type.h"
#include "../i_border_component.h"

namespace map {

class ITarget
{
public:
    ITarget( int32_t Id );
    virtual ~ITarget();
    virtual void Update( double DeltaTime )=0;
    virtual int32_t GetCursorId() const;
    virtual std::auto_ptr<Actor> GetCursor();
    virtual int32_t GetActorId() const;
    virtual void PutTarget(glm::vec2 position)=0;
    virtual void PutTarget(glm::vec2 position, IBorderComponent::Borders_t& borders, IBorderComponent::Borders_t& outerBorders);
protected:
    int32_t mId;
};


} // namespace map
#endif//INCLUDED_CORE_MAP_I_TARGET_H