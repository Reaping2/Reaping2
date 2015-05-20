#ifndef INCLUDED_CORE_TARGET_PLAYER_CONTROLLER_H
#define INCLUDED_CORE_TARGET_PLAYER_CONTROLLER_H

#include "core/controller_component.h"
#include "core/actor.h"
#include "core/property_loader.h"

class TargetPlayerControllerComponent : public ControllerComponent
{
public:
    double GetAttackCounter() const;
    void SetAttackCounter(double counter);
    double GetAttackFrequency() const;
    void SetAttackFrequency(double frequency);
    double GetHeadingModifierCounter() const;
    void SetHeadingModifierCounter(double counter);
    double GetHeadingModifier() const;
    void SetHeadingModifier(double headingModifier);
    double GetHeadingModifierFrequency() const;
    void SetHeadingModifierFrequency(double frequency);

    bool IsNeedInit() const;
    void SetNeedInit(bool needInit);
    TargetPlayerControllerComponent();
private:
    double mAttackCounter;
    double mAttackFrequency;
    double mHeadingModifierCounter;
    double mHeadingModifierFrequency;
    double mHeadingModifier;
    bool mNeedInit;
};

class TargetPlayerControllerComponentLoader: public ComponentLoader<TargetPlayerControllerComponent>
{
    virtual void BindValues();
protected:
    TargetPlayerControllerComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_TARGET_PLAYER_CONTROLLER_H
