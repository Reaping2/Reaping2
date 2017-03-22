#include "jsonreader.h"
#include "ifile.h"

#include <boost/assert.hpp>
#include "boost/algorithm/string/predicate.hpp"

// namespace platform {

namespace Json {
bool GetStr( Json::Value const& V, std::string& O )
{
    if( !V.isString() )
    {
        return false;
    }
    O = V.asString();
    return !O.empty();
}
bool GetUInt( Json::Value const& V, uint32_t& O )
{
    if( !V.isNumeric() )
    {
        return false;
    }
    O = V.asUInt();
    return true;
}
bool GetInt( Json::Value const& V, int32_t& O )
{
    if( !V.isNumeric() )
    {
        return false;
    }
    O = V.asInt();
    return true;
}
bool GetDouble( Json::Value const& V, double& O )
{
    if( !V.isNumeric() )
    {
        return false;
    }
    O = V.asDouble();
    return true;
}
bool GetFloat( Json::Value const& V, float& O )
{
    if( !V.isNumeric() )
    {
        return false;
    }
    O = V.asDouble();
    return true;
}
bool GetColor( Json::Value const& Color, int32_t& O )
{
    if( Json::GetInt( Color, O ) )
    {
        return true;
    }
    std::string s;
    if( Json::GetStr( Color, s ) && ( O = strtoul( s.c_str(), NULL, 16 ) ) )
    {
        size_t const h = s.find( 'x' );
        if( ( s.size() <= 6 && h == std::string::npos )
            || ( s.size() <= 8 && h == 1 ) )
        {
            // make it rgba
            O = ( O << 8 ) | 0xff;
        }
        return true;
    }
    return false;
}
bool GetColor( Json::Value const& Color, glm::vec4& O )
{
    int32_t c;
    if( !GetColor( Color, c ) )
    {
        return false;
    }
    O = glm::vec4(
            ( ( c & 0xff000000 ) >> 24 ) / 255.,
            ( ( c & 0x00ff0000 ) >> 16 ) / 255.,
            ( ( c & 0x0000ff00 ) >> 8 ) / 255.,
            ( ( c & 0x000000ff ) ) / 255.
        );
    return true;
}

bool GetBool( Json::Value const &V, bool& O)
{
    int32_t i = 0;
    if (Json::GetInt( V, i ))
    {
        O = i != 0;
        return true;
    }
    std::string s = "";
    if (Json::GetStr( V, s ))
    {
        O = !(boost::iequals( s, "false" )
            || boost::iequals( s, "f" )
            || boost::iequals( s, "0" )
            || boost::iequals( s, "n" )
            || boost::iequals( s, "no" ));
        return true;
    }
    return false;
}

bool Get( Value const& V, std::string& O )
{
    return GetStr( V, O );
}

bool Get( Value const& V, uint32_t& O )
{
    return GetUInt( V, O );
}

bool Get( Value const& V, int32_t& O )
{
    return GetInt( V, O );
}

bool Get( Value const& V, double& O )
{
    return GetDouble( V, O );
}

bool Get( Value const& V, float& O )
{
    return GetFloat( V, O );
}

bool Get( Value const& V, bool& O )
{
    return GetBool( V, O );
}

bool Get( Value const& V, glm::vec4& O )
{
    return GetColor( V, O );
}

} // namespace Json

namespace platform {

JsonReader::JsonReader( File& F )
    : mValid( false )
{
    Json::Reader Reader;
    std::string Contents;
    if( !F.ReadAll( Contents ) )
    {
        return;
    }
    try
    {
        Json::Value target;
        if( !Reader.parse( Contents, target, false ) )
        {
            BOOST_ASSERT( false );
        }
        using namespace std;
        swap( target, mRoot );
        mValid = true;
    }
    catch( std::runtime_error const& )
    {
    }
}

bool JsonReader::IsValid() const
{
    return mValid;
}

Json::Value& JsonReader::GetRoot()
{
    return mRoot;
}

} // namespace platform

