#include "init.h"
#include <boost/assert.hpp>
#include <algorithm>

namespace platform {
Init::Init()
{
}
void Init::Register( std::string const& name, InitFunc_t const& f )
{
    InitFunc_t& fn = mFuncs[ name ];
    BOOST_ASSERT( !fn );
    fn = f;
}
void Init::Execute() const
{
    std::for_each( std::begin( mFuncs ), std::end( mFuncs ), []( FuncMap_t::value_type const& i)
            { if( i.second ) i.second(); } );
}
}
