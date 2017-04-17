#ifndef INCLUDED_CORE_PLAYER_CONTROLLER_H
#define INCLUDED_CORE_PLAYER_CONTROLLER_H

#include "input/i_input.h"
#include "core/controller_component.h"
#include "core/actor.h"
#include "core/property_loader.h"
#include "trigger.h"
#include "platform/export.h"

class PlayerControllerComponent : public ControllerComponent
{
public:
    PlayerControllerComponent();
    double mOrientation;
    double mHeading;
    bool mShoot;
    bool mShootAlt;
    Trigger mUseNormalItem;
    Trigger mActivate;
    Trigger mSwitchWeapon;
    Trigger mSwitchNormalItem;
    bool mActive; //controlled by current client
    int32_t mControllerId; //controller client id
    Trigger mUseReload;
    bool mMoving;
    double mActivateRange; // this might be buffable later
    int32_t mHighLightedGUID;
    void SetActivateRange( double activateRange );
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void PlayerControllerComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<ControllerComponent>( *this );
    ar& mOrientation;
    ar& mHeading;
    ar& mShoot;
    ar& mShootAlt;
    ar& mUseNormalItem;
    ar& mActivate;
    ar& mSwitchWeapon;
    ar& mSwitchNormalItem;
    ar& mActive;
    ar& mControllerId;
    ar& mUseReload;
    ar& mMoving;
    ar& mActivateRange;
    ar& mHighLightedGUID;
}

class PlayerControllerComponentLoader: public ComponentLoader<PlayerControllerComponent>
{
public:
    DEFINE_COMPONENT_LOADER_BASE( PlayerControllerComponentLoader )
private:
    virtual void BindValues();
protected:
    PlayerControllerComponentLoader();
    friend class ComponentLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( PlayerControllerComponent, PlayerControllerComponent, "player_controller_component" );
#endif//INCLUDED_CORE_PLAYER_CONTROLLER_H
