#ifndef INCLUDED_CORE_PLAYER_CONTROLLER_H
#define INCLUDED_CORE_PLAYER_CONTROLLER_H

#include "input/i_input.h"
#include "core/controller_component.h"
#include "core/actor.h"
#include "core/property_loader.h"

class PlayerControllerComponent : public ControllerComponent
{
public:
    PlayerControllerComponent();
    double mOrientation;
    double mHeading;
    bool mShoot;
    bool mShootAlt;
    bool mUseNormalItem;
    uint32_t mCurrentMovement;
    bool mActive; //controlled by current client
    int32_t mControllerId; //controller client id
    bool mReviveTyped;
    bool mReloadTyped;
};

class PlayerControllerComponentLoader: public ComponentLoader<PlayerControllerComponent>
{
    virtual void BindValues();
protected:
    PlayerControllerComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_PLAYER_CONTROLLER_H
