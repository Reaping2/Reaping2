#pragma once
#ifndef INCLUDED_PARTICLE_ENGINE_H
#define INCLUDED_PARTICLE_ENGINE_H

#include "platform/i_platform.h"

namespace render {

class ParticleEngineImpl;
class ParticleEngine : public platform::Singleton<ParticleEngine>
{
    std::auto_ptr<ParticleEngineImpl> mImpl;
public:
    ParticleEngine();
    ~ParticleEngine();
    void Update( float dt );
    void Draw() const;
    void AddParticle( int32_t type, glm::vec2 const& pos );
};

} // namespace render

#endif // INCLUDED_PARTICLE_ENGINE_H

