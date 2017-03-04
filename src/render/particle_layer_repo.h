#pragma once
#ifndef INCLUDED_PARTICLE_LAYER_REPO_H
#define INCLUDED_PARTICLE_LAYER_REPO_H

#include "platform/i_platform.h"
#include <json/json.h>

namespace render {

struct ParticleLayer
{
    GLenum sfactor;
    GLenum dfactor;
    ParticleLayer( Json::Value const& json );
};

class ParticleLayerRepo : public platform::Repository<ParticleLayer>, public platform::Singleton<ParticleLayerRepo>
{
    ParticleLayer mDefaultParticleLayer;
    friend class Singleton<ParticleLayerRepo>;
    ParticleLayerRepo();
    static std::string const mDefaultParticleLayerName;
};

}

#endif // INCLUDED_PARTICLE_LAYER_REPO_H

