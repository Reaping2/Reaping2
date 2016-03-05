#pragma once
#ifndef INCLUDED_INIT_H
#define INCLUDED_INIT_H

#include "singleton.h"
#include <functional>
#include <string>
#include <map>

namespace platform
{
typedef std::function<void()> InitFunc_t;

class Init : public Singleton<Init>
{
    friend class Singleton<Init>;
    Init();
    // name -> function map, so the order is alphabetical
    typedef std::map<std::string, InitFunc_t> FuncMap_t;
    FuncMap_t mFuncs;
public:
    void Register( std::string const& name, InitFunc_t const& f );
    void Execute() const;
};

}

#define REGISTER_INIT_PRIO( Prio, InitName, InitFunc ) \
namespace { \
struct RegisterInit { \
RegisterInit() \
{ \
    platform::Init::Get().Register( #Prio __FILE__ #InitName, InitFunc ); \
} \
} registerSystem; \
}

#define REGISTER_INIT( InitName, InitFunc ) \
    REGISTER_INIT_PRIO( normal, InitName, InitFunc )


#endif // INCLUDED_INIT_H

