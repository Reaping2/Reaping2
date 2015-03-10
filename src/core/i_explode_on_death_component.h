#ifndef INCLUDED_CORE_I_EXPLODE_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_I_EXPLODE_ON_DEATH_COMPONENT_H
#include "component.h"

class IExplodeOnDeathComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IExplodeOnDeathComponent)
    virtual void SetExplosionProjectile( int32_t explosionProjectile )=0;
    virtual int32_t GetExplosionProjectile() const=0;    
protected:
	friend class ComponentFactory;
};

#endif//INCLUDED_CORE_I_EXPLODE_ON_DEATH_COMPONENT_H