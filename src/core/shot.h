#ifndef INCLUDED_CORE_SHOT_H
#define INCLUDED_CORE_SHOT_H
#include "core/actor.h"

class Shot : public Actor
{
protected:
    int32_t mParentGuid;
public:
    Shot( std::string const& Name );
};

#endif//INCLUDED_CORE_SHOT_H
