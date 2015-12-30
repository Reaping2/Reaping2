#ifndef INCLUDED_CORE_I_EMITTER_COMPONENT_H
#define INCLUDED_CORE_I_EMITTER_COMPONENT_H

#include "component.h"

class IEmitterComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IEmitterComponent)
    virtual void SetEmitType(int32_t emitType)=0;
    virtual int32_t GetEmitType()const=0;
    virtual bool IsEmitting()const=0;
    virtual void Update( double dt ) = 0;
};

#endif//INCLUDED_CORE_I_EMITTER_COMPONENT_H

//command:  "../../build/tools/classgenerator/classgenerator" -g "i_component" -c "i_emitter_component" -m "int32_t-emitType bool-isemitting"
