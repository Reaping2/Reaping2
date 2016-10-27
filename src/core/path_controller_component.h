#ifndef INCLUDED_CORE_PATH_CONTROLLER_COMPONENT_H
#define INCLUDED_CORE_PATH_CONTROLLER_COMPONENT_H

#include "controller_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class PathControllerComponent : public ControllerComponent
{
public:
    PathControllerComponent();
    virtual void SetNextAttackTimer( double nextAttackTimer );
    virtual double GetNextAttackTimer() const;
    virtual void SetNextAttackTimerMax( double nextAttackTimerMax );
    virtual double GetNextAttackTimerMax() const;
    virtual void SetDamage( int32_t damage );
    virtual int32_t GetDamage() const;
    virtual void SetAggroDist( int32_t aggroDist );
    virtual int32_t GetAggroDist() const;
    virtual void SetPeaceDist( int32_t peaceDist );
    virtual int32_t GetPeaceDist() const;
protected:
    friend class ComponentFactory;
    double mNextAttackTimer;
    double mNextAttackTimerMax;
    int32_t mDamage;
    int32_t mAggroDist;
    int32_t mPeaceDist;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void PathControllerComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<ControllerComponent>(*this);
    ar& mNextAttackTimer;
    ar& mNextAttackTimerMax;
    ar& mDamage;
    ar& mAggroDist;
    ar& mPeaceDist;
}

class PathControllerComponentLoader : public ComponentLoader<PathControllerComponent>
{
    virtual void BindValues();
protected:
    PathControllerComponentLoader();
    friend class ComponentLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( PathControllerComponent, PathControllerComponent, "path_controller_component" );

#endif//INCLUDED_CORE_PATH_CONTROLLER_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -p "controller_component" -c "path_controller_component" -m "double-nextAttackTimer double-nextAttackTimerMax int32_t-damage int32_t-aggroDist int32_t-peaceDist"
