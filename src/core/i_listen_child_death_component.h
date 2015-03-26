#ifndef INCLUDED_CORE_I_LISTEN_CHILD_DEATH_COMPONENT_H
#define INCLUDED_CORE_I_LISTEN_CHILD_DEATH_COMPONENT_H

#include "component.h"

class IListenChildDeathComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IListenChildDeathComponent)
    virtual void SetKillerOfChildGUID(int32_t killerOfChildId)=0;
    virtual int32_t GetKillerOfChildGUID()const=0;
};

#endif//INCLUDED_CORE_I_LISTEN_CHILD_DEATH_COMPONENT_H
