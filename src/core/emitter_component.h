#ifndef INCLUDED_CORE_EMITTER_COMPONENT_H
#define INCLUDED_CORE_EMITTER_COMPONENT_H

#include "i_emitter_component.h"
#include "core/property_loader.h"

class EmitterComponent : public IEmitterComponent
{
public:
    EmitterComponent();
    virtual void SetEmitType(int32_t emitType);
    virtual int32_t GetEmitType()const;
    virtual bool IsEmitting()const;
    virtual void SetFrequency(double frequency);
    virtual double GetFrequency()const;
    virtual void Update( double dt );
protected:
    friend class ComponentFactory;
    int32_t mEmitType;
    double mFrequency;
private:
    double mCooldown;
};

class EmitterComponentLoader : public ComponentLoader<EmitterComponent>
{
    virtual void BindValues();
protected:
    EmitterComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_EMITTER_COMPONENT_H

//command:  "../../build/tools/classgenerator/classgenerator" -g "component" -c "emitter_component" -m "int32_t-emitType bool-isemitting double-frequency"
