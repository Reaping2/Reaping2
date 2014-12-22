#ifndef INCLUDED_ENGINE_SYSTEM_H
#define INCLUDED_ENGINE_SYSTEM_H

namespace engine {

class System
{
public:
    virtual void Init()=0;
    virtual void Update( double DeltaTime )=0;
    virtual ~System();
};

class DefaultSystem : public System
{
public:
    virtual void Init();
    virtual void Update( double DeltaTime );
};

}

#endif//INCLUDED_ENGINE_SYSTEM_H
