#ifndef INCLUDED_CORE_MAP_I_TARGET_H
#define INCLUDED_CORE_MAP_I_TARGET_H

#include "platform\i_platform.h"

namespace map {

class ITarget
{
public:
    ITarget( int32_t Id );
    virtual void Update( double DeltaTime )=0;
    virtual int32_t GetCursorId() const;
    virtual int32_t GetActorId() const;
protected:
    int32_t mId;
};


} // namespace map
#endif//INCLUDED_CORE_MAP_I_TARGET_H