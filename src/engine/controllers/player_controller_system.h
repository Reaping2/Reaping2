#ifndef INCLUDED_PLAYER_CONTROLLER_SYSTEM_H
#define INCLUDED_PLAYER_CONTROLLER_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "input/i_input.h"

namespace engine {

class PlayerControllerSystem : public System
{
public:
    PlayerControllerSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
};

} // namespace engine

#endif//INCLUDED_PLAYER_CONTROLLER_SYSTEM_H
