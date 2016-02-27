#ifndef INCLUDED_CORE_PLAYER_CONTROLLER_H
#define INCLUDED_CORE_PLAYER_CONTROLLER_H

#include "input/i_input.h"
#include "core/controller_component.h"
#include "core/actor.h"
#include "core/property_loader.h"
#include "consumable.h"

class PlayerControllerComponent : public ControllerComponent
{
public:
    PlayerControllerComponent();
    double mOrientation;
    double mHeading;
    bool mShoot;
    bool mShootAlt;
    Consumable mUseNormalItem;
    bool mActive; //controlled by current client
    int32_t mControllerId; //controller client id
    Consumable mReloadTyped;
    bool mMoving;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void PlayerControllerComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<ControllerComponent>(*this);
    ar & mOrientation;
    ar & mHeading;
    ar & mShoot;
    ar & mShootAlt;
    ar & mUseNormalItem;
    ar & mActive;
    ar & mControllerId;
    ar & mReloadTyped;
    ar & mMoving;
}

class PlayerControllerComponentLoader: public ComponentLoader<PlayerControllerComponent>
{
    virtual void BindValues();
protected:
    PlayerControllerComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_PLAYER_CONTROLLER_H
