#ifndef INCLUDED_CORE_I_DROP_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_I_DROP_ON_DEATH_COMPONENT_H
#include "component.h"

class IDropOnDeathComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IDropOnDeathComponent)
    virtual void Update( double Seconds )=0;    
protected:
	friend class ComponentFactory;
};

#endif//INCLUDED_CORE_I_DROP_ON_DEATH_COMPONENT_H