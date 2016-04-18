#include "mapping.h"
#include "platform/settings.h"

namespace input {

Mapping::Mapping()
{
}

Json::Value const& Mapping::getMapping( std::string controlSchemeName )
{
    if( mMappings.isMember( controlSchemeName ) )
    {
        return mMappings[ controlSchemeName ];
    }
    Json::Value& val = mMappings[ controlSchemeName ];
    static Settings& settings( Settings::Get() );
    Json::Value controls = settings.Resolve( "controlmappings." + controlSchemeName );
    while( !controls && !controlSchemeName.empty() )
    {   // control scheme can be more fine-grained, e.g. controller.XBox 360 Wireless.linux
        // if the fine-grained one is not found, go up a step
        size_t part = controlSchemeName.find_last_of( '.' );
        if( std::string::npos == part )
        {
            break;
        }
        controlSchemeName = controlSchemeName.substr( 0, part );
        controls = settings.Resolve( "controlmappings." + controlSchemeName );
    }
    val = controls;
    return val;
}

}

