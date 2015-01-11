#ifndef INCLUDED_ENGINE_SUB_SYSTEM_H
#define INCLUDED_ENGINE_SUB_SYSTEM_H

#include "core\actor.h"

#define DEFINE_SUB_SYSTEM_BASE( SystemType ) \
    static int32_t GetType_static() \
    { \
    static int32_t const typ = platform::AutoId( #SystemType ); \
    return typ; \
    } \
    virtual int32_t GetType() const \
    { \
    return SystemType::GetType_static(); \
    } \

namespace engine {

    class SubSystem
    {
    public:
        virtual int32_t GetType() const=0;
        virtual void Init()=0;
        virtual void Update( Actor& actor, double DeltaTime )=0;
        virtual ~SubSystem();
    };

    class DefaultSubSystem : public SubSystem
    {
    public:
        DEFINE_SUB_SYSTEM_BASE(DefaultSubSystem)
        virtual void Init();
        virtual void Update( Actor& actor, double DeltaTime );
    };
} // namespace engine

#endif//INCLUDED_ENGINE_SUB_SYSTEM_H