#include "engine/system.h"

namespace engine {

    System::~System()
    {

    }

    // when enabled is changed on this system
    void System::SetEnabled(bool enabled)
    {
        mEnabled=enabled;
    }


    void DefaultSystem::Init()
    {

    }

    void DefaultSystem::Update(double DeltaTime)
    {

    }

} // namespace engine
