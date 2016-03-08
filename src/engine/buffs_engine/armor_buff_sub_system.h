#ifndef INCLUDED_ENGINE_ARMOR_BUFF_SUB_SYSTEM_H
#define INCLUDED_ENGINE_ARMOR_BUFF_SUB_SYSTEM_H

#include "core/scene.h"
#include "engine/sub_system.h"
#include "core/program_state.h"

namespace engine {

class ArmorBuffSubSystem : public SubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE( ArmorBuffSubSystem )
    ArmorBuffSubSystem();
protected:
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
private:
    Scene& mScene;
    core::ProgramState& mProgramState;
};

} // namespace engine

#endif//INCLUDED_ENGINE_ARMOR_BUFF_SUB_SYSTEM_H

//command:  "classgenerator.exe" -g "buff_sub_system" -c "armor_buff_sub_system" -t "armor_buff"
