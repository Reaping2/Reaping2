#ifndef INCLUDED_CORE_ATTRACTABLE_COMPONENT_H
#define INCLUDED_CORE_ATTRACTABLE_COMPONENT_H

#include "i_attractable_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class AttractableComponent : public IAttractableComponent
{
public:
    AttractableComponent();
    virtual void SetAttractType( int32_t type );
    virtual int32_t GetAttractType() const;
    virtual void SetDeceleration( int32_t deceleration );
    virtual int32_t GetDeceleration() const;
    virtual void SetTurnToTargetAct( scriptedcontroller::TurnToTargetAct turnToTargetAct );
    virtual scriptedcontroller::TurnToTargetAct& GetTurnToTargetAct();
protected:
    friend class ComponentFactory;
    int32_t mType;
    int32_t mDeceleration;
    scriptedcontroller::TurnToTargetAct mTurnToTargetAct;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void AttractableComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<IAttractableComponent>(*this);
    ar& mType;
    ar& mDeceleration;
    ar& mTurnToTargetAct;
}

class AttractableComponentLoader : public ComponentLoader<AttractableComponent>
{
    virtual void BindValues();
protected:
    AttractableComponentLoader();
    friend class ComponentLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( AttractableComponent, AttractableComponent, "attractable_component" );

#endif//INCLUDED_CORE_ATTRACTABLE_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "attractable_component" -m "int32_t-type int32_t-deceleration IAct-turnToTargetAct"
