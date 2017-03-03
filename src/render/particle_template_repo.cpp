#include "particle_template_repo.h"
#include "platform/jsonreader.h"
#include "renderable_repo.h"
#include <boost/assign/list_of.hpp>
#include "platform/filesystem_utils.h"

namespace render {

ParticleTemplateRepo::ParticleTemplateRepo()
    : Repository<ParticleTemplate>( mDefaultParticleTemplate )
    , mDefaultParticleTemplate( Json::Value() )
{
    fs_utils::for_each( "actors", ".particle", [&]( Json::Value const& desc )
    {
        int32_t Idx = AutoId( desc["name"].asString() );
        mElements.insert( Idx, new ParticleTemplate( desc ) );
    } );
}

ParticleTemplate::ParticleTemplate( Json::Value const& json )
    : Spr( NULL )
{
#define GET( type, name, defa ) \
    if( !Json::Get##type( json[ #name ], name ) ) { name = defa; }
    GET( Color, Color, glm::vec4() );
    GET( Color, ColorVariance, glm::vec4() );
    GET( Float, PosVariance, 0 );
    GET( Float, AbsSpeed, 0 );
    GET( Float, AbsSpeedVariance, 0 );
    GET( Float, MinSpeed, 0 );
    GET( Float, MaxSpeed, 0 );
    GET( Float, AbsAcceleration, 0 );
    GET( Float, AbsAccelerationVariance, 0 );
    GET( Float, RotationSpeed, 0 );
    GET( Float, MinRotationSpeed, 0 );
    GET( Float, MaxRotationSpeed, 0 );
    GET( Float, RotationSpeedVariance, 0 );
    GET( Float, RotationAcceleration, 0 );
    GET( Float, RotationAccelerationVariance, 0 );
    GET( Float, Lifetime, 0 );
    GET( Float, LifetimeVariance, 0 );
    GET( Float, Radius, 0 );
    GET( Float, RadiusVariance, 0 );
    GET( Float, MinRadius, Radius );
    GET( Float, MaxRadius, Radius );
    GET( Float, ScaleSpeed, 0 );
    GET( Int, Num, 1 );
    GET( Int, NumVariance, 0 );
    GET( Int, ParticleLayer, 0 );
    int32_t Interpolate_i=0;
    Json::GetInt( json["Interpolate"], Interpolate_i );
    Interpolate = Interpolate_i != 0;
#undef GET
    SpeedDir = AccelerationDir = Any;
    RotDir = RotAccelerationDir = Rot_Any;
    Heading = H_Any;

    typedef std::map<std::string, RotationDirection> rotmap_t;
    static rotmap_t const rotmap = boost::assign::map_list_of
                                   ( std::string( "+" ), Rot_P )
                                   ( std::string( "-" ), Rot_N )
                                   ( std::string( "=" ), Rot_Any );
    typedef std::map<std::string, SpeedDirection> spdmap_t;
    static spdmap_t const spdmap = boost::assign::map_list_of
                                   ( std::string( "Towards" ), Towards )
                                   ( std::string( "Away" ), Away )
                                   ( std::string( "Any" ), Any );
    typedef std::map<std::string, HeadingType> hmap_t;
    static hmap_t const hmap = boost::assign::map_list_of
                               ( std::string( "Actor" ), H_Actor )
                               ( std::string( "Any" ), H_Any );
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
    MAP( spd, SpeedDir );
    MAP( spd, AccelerationDir );
    MAP( rot, RotDir );
    MAP( rot, RotAccelerationDir );
    MAP( h, Heading );
#undef MAP
    if( Json::GetStr( json[ "Spr" ], str ) )
    {
        static RenderableRepo& rr( RenderableRepo::Get() );
        static int32_t actionId = AutoId( "default_action" );
        Sprite const& spr = rr( AutoId( str ) )( actionId );
        if( spr.IsValid() )
        {
            Spr = &spr;
        }
    }
}

}

