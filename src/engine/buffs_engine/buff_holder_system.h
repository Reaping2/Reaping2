#ifndef INCLUDED_ENGINE_BUFF_HOLDER_SYSTEM_H
#define INCLUDED_ENGINE_BUFF_HOLDER_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "engine/sub_system_holder.h"

namespace engine {

class BuffHolderSystem : public System, public SubSystemHolder
{
public:
    DEFINE_SYSTEM_BASE(BuffHolderSystem)
    BuffHolderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_BUFF_HOLDER_SYSTEM_H


