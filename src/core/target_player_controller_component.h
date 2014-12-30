#ifndef INCLUDED_CORE_TARGET_PLAYER_CONTROLLER_H
#define INCLUDED_CORE_TARGET_PLAYER_CONTROLLER_H

#include "core/controller_component.h"
#include "core/actor.h"
#include "core/property_loader.h"

class TargetPlayerControllerComponent : public ControllerComponent
{
public:
    double GetCounter() const;
    void SetCounter(double counter);
    double GetHeadingModifier() const;
    void SetHeadingModifier(double headingModifier);

    void SetPlayer( Actor* Player );
    Actor* GetPlayer();

    TargetPlayerControllerComponent();
private:
    Actor* mPlayer;
    double mCounter;
    double mHeadingModifier;
};

class TargetPlayerControllerComponentLoader: public ComponentLoader<TargetPlayerControllerComponent>
{
    virtual void BindValues();
protected:
    TargetPlayerControllerComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_TARGET_PLAYER_CONTROLLER_H
