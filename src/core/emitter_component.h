#ifndef INCLUDED_CORE_EMITTER_COMPONENT_H
#define INCLUDED_CORE_EMITTER_COMPONENT_H

#include "i_emitter_component.h"
#include "core/property_loader.h"

class EmitterComponent : public IEmitterComponent
{
public:
    EmitterComponent();
    virtual std::vector<int32_t> GetEmitTypes() const;
    virtual void Update( double dt );
private:
    friend class ComponentFactory;
    friend class EmitterComponentLoader;
    struct EmitDesc {
        int32_t mEmitType;
        double mDelay;
        double mDelayVariance;
        double mCooldown;
        double mProbability;
        EmitDesc();
    };
    typedef std::vector<EmitDesc> EmitDescs;
    EmitDescs mEmitDescs;
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
