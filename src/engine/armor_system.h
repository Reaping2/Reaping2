#ifndef INCLUDED_ENGINE_ARMOR_SYSTEM_H
#define INCLUDED_ENGINE_ARMOR_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {

class ArmorSystem : public System
{
public:
    DEFINE_SYSTEM_BASE( ArmorSystem )
    ArmorSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_ARMOR_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "armor_system" -t "armor"
