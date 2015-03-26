#ifndef INCLUDED_CORE_I_NOTIFY_PARENT_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_I_NOTIFY_PARENT_ON_DEATH_COMPONENT_H

#include "component.h"

class INotifyParentOnDeathComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(INotifyParentOnDeathComponent)
    virtual void SetParentGUID(int32_t parentId)=0;
    virtual int32_t GetParentGUID()const=0;
    virtual void SetKillerGUID(int32_t killerId)=0;
    virtual int32_t GetKillerGUID()const=0;
};

#endif//INCLUDED_CORE_I_NOTIFY_PARENT_ON_DEATH_COMPONENT_H
