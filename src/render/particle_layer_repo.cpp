#include "particle_layer_repo.h"
#include "platform/jsonreader.h"
#include <boost/assign/list_of.hpp>
#include "platform/filesystem_utils.h"


namespace render {
ParticleLayerRepo::ParticleLayerRepo()
    : Repository<ParticleLayer>( mDefaultParticleLayer )
    , mDefaultParticleLayer( Json::Value() )
{
    fs_utils::for_each( "misc/particlelayers", ".json", [&]( Json::Value const& desc )
    {
        int32_t Idx = desc["id"].asInt();
        mElements.insert( Idx, new ParticleLayer( desc ) );
    } );
}

ParticleLayer::ParticleLayer( Json::Value const& json )
{
    sfactor = GL_SRC_ALPHA;
    dfactor = GL_ONE;
    typedef std::map<std::string, GLenum> glmap_t;
#define ELEM( x ) \
    ( std::string( #x ), x )

    static glmap_t const glmap = boost::assign::map_list_of
                                 ELEM( GL_ONE )
                                 ELEM( GL_ZERO )
                                 ELEM( GL_SRC_COLOR )
                                 ELEM( GL_ONE_MINUS_SRC_COLOR )
                                 ELEM( GL_DST_COLOR )
                                 ELEM( GL_ONE_MINUS_DST_COLOR )
                                 ELEM( GL_SRC_ALPHA )
                                 ELEM( GL_ONE_MINUS_SRC_ALPHA )
                                 ELEM( GL_DST_ALPHA )
                                 ELEM( GL_ONE_MINUS_DST_ALPHA )
                                 ELEM( GL_CONSTANT_COLOR )
                                 ELEM( GL_ONE_MINUS_CONSTANT_COLOR )
                                 ELEM( GL_CONSTANT_ALPHA )
                                 ELEM( GL_ONE_MINUS_CONSTANT_ALPHA );
#undef ELEM
    std::string str;
#define MAP( type, name ) \
    if( Json::GetStr( json[ #name ], str ) ) \
    { \
        type##map_t::const_iterator i = type##map.find( str ); \
        if( i != type##map.end() ) \
        { \
            name = i->second; \
        } \
    }
    MAP( gl, sfactor );
    MAP( gl, dfactor );
#undef MAP
}
}

