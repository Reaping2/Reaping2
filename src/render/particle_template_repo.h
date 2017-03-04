#pragma once
#ifndef INCLUDED_PARTICLE_TEMPLATE_REPO_H
#define INCLUDED_PARTICLE_TEMPLATE_REPO_H

#include "platform/i_platform.h"
#include "sprite.h"
#include <json/json.h>

namespace render {
struct ParticleTemplate
{
    ParticleTemplate( Json::Value const& json );
    Sprite const* Spr;
    glm::vec4 Color;
    glm::vec4 ColorVariance;
    float PosVariance;
    float AbsSpeed;
    float AbsSpeedVariance;
    float MinSpeed;
    float MaxSpeed;
    float AbsAcceleration;
    float AbsAccelerationVariance;
    float RotationSpeed;
    float MinRotationSpeed;
    float MaxRotationSpeed;
    float RotationSpeedVariance;
    enum RotationDirection
    {
        Rot_P,
        Rot_N,
        Rot_Any,
    };
    RotationDirection RotDir;
    RotationDirection RotAccelerationDir;
    enum SpeedDirection
    {
        Towards,
        Away,
        Any,
    };
    SpeedDirection SpeedDir;
    SpeedDirection AccelerationDir;
    float RotationAcceleration;
    float RotationAccelerationVariance;
    float Lifetime;
    float LifetimeVariance;
    float Radius;
    float RadiusVariance;
    float MinRadius;
    float MaxRadius;
    float ScaleSpeed;
    int32_t Num;
    int32_t NumVariance;
    enum HeadingType
    {
        H_Actor,
        H_Any,
    };
    HeadingType Heading;
    int32_t ParticleLayer;
    bool Interpolate;
};


class ParticleTemplateRepo : public platform::Repository<ParticleTemplate>, public platform::Singleton<ParticleTemplateRepo>
{
    ParticleTemplate mDefaultParticleTemplate;
    friend class Singleton<ParticleTemplateRepo>;
    ParticleTemplateRepo();
};

}

#endif // INCLUDED_PARTICLE_TEMPLATE_REPO_H

