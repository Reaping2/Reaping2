#ifndef INCLUDED_CORE_I_REMOVE_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_I_REMOVE_ON_DEATH_COMPONENT_H
#include "component.h"

class IRemoveOnDeathComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IRemoveOnDeathComponent)
    virtual void Update( double Seconds )=0;  
    virtual bool NeedDelete()=0;
protected:
	friend class ComponentFactory;
};

#endif//INCLUDED_CORE_I_REMOVE_ON_DEATH_COMPONENT_H