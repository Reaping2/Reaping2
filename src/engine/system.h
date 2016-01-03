#ifndef INCLUDED_ENGINE_SYSTEM_H
#define INCLUDED_ENGINE_SYSTEM_H

#include "platform/auto_id.h"

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
    static bool mInitedSystem; \
    static bool initSystem() \
    { \
        mInitedSystem = true; \
        return mInitedSystem; \
    } \

#define INSTANTIATE_SYSTEM( SystemType ) \
namespace { \
namespace ns##SystemType { \
volatile bool inited = SystemType::initSystem(); \
} \
} \

#define REGISTER_SYSTEM( SystemType ) \
bool SystemType::mInitedSystem = true; \
namespace { \
struct RegisterSystem { \
RegisterSystem() \
{ \
    ::engine::SystemFactory::Get().RegisterSystem<SystemType>( #SystemType ); \
} \
} registerSystem; \
}

namespace engine {

class System
{
public:
    virtual int32_t GetType() const=0;
    virtual void Init()=0;
    virtual void Update( double DeltaTime )=0;
    virtual void SetEnabled ( bool enabled );
    virtual ~System();
protected:
    bool mEnabled;
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
