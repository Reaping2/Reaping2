#ifndef INCLUDED_ENGINE_SYSTEM_H
#define INCLUDED_ENGINE_SYSTEM_H

#include "platform\auto_id.h"

#define DEFINE_SYSTEM_BASE( SystemType ) \
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

class System
{
public:
    virtual int32_t GetType() const=0;
    virtual void Init()=0;
    virtual void Update( double DeltaTime )=0;
    virtual ~System();
};

class DefaultSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(DefaultSystem)
    virtual void Init();
    virtual void Update( double DeltaTime );
};

}

#endif//INCLUDED_ENGINE_SYSTEM_H
