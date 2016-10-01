#ifndef INCLUDED_CORE_GUARD_CONTROLLER_COMPONENT_H
#define INCLUDED_CORE_GUARD_CONTROLLER_COMPONENT_H

#include "controller_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class GuardControllerComponent : public ControllerComponent
{
public:
    enum Direction
    {
        Left = 0,
        Right,
        None
    };
    GuardControllerComponent();
    virtual void SetNextMoveTimer( double nextMoveTimer );
    virtual double GetNextMoveTimer() const;
    virtual void SetMoveDirection( Direction moveDirection );
    virtual Direction GetMoveDirection() const;
    virtual void SetAggroAltDist( int32_t aggroAltDist );
    virtual int32_t GetAggroAltDist() const;
    virtual void SetCloseDist( int32_t closeDist );
    virtual int32_t GetCloseDist() const;
    virtual void SetWalkAwayDist( int32_t walkAwayDist );
    virtual int32_t GetWalkAwayDist() const;
    virtual void SetPeaceDist( int32_t peaceDist );
    virtual int32_t GetPeaceDist() const;
    virtual void SetNextMoveTimerMax( double nextMoveTimerMax );
    virtual double GetNextMoveTimerMax() const;
    virtual void SetNextMoveTimerVariance( double nextMoveTimerVariance );
    virtual double GetNextMoveTimerVariance() const;
    virtual void SetAggroDist( int32_t aggroDist );
    virtual int32_t GetAggroDist() const;
protected:
    friend class ComponentFactory;
    double mNextMoveTimer;
    Direction mMoveDirection;
    int32_t mAggroAltDist;
    int32_t mCloseDist;
    int32_t mWalkAwayDist;
    int32_t mPeaceDist;
    double mNextMoveTimerMax;
    double mNextMoveTimerVariance;
    int32_t mAggroDist;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void GuardControllerComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<ControllerComponent>(*this);
    ar& mNextMoveTimer;
    ar& mMoveDirection;
    ar& mAggroAltDist;
    ar& mCloseDist;
    ar& mWalkAwayDist;
    ar& mPeaceDist;
    ar& mNextMoveTimerMax;
    ar& mNextMoveTimerVariance;
    ar& mAggroDist;
}

class GuardControllerComponentLoader : public ComponentLoader<GuardControllerComponent>
{
public:
    DEFINE_COMPONENT_LOADER_BASE( GuardControllerComponentLoader )
    virtual void BindValues();
protected:
    GuardControllerComponentLoader();
    friend class ComponentLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( GuardControllerComponent, GuardControllerComponent, "guard_controller_component" );

#endif//INCLUDED_CORE_GUARD_CONTROLLER_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -p "controller_component" -c "guard_controller_component"
