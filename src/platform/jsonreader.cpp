#include "jsonreader.h"
#include "ifile.h"

#include <boost/assert.hpp>

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
            ( c & 0xff000000 >> 24 ) / 255.,
            ( c & 0x00ff0000 >> 16 ) / 255.,
            ( c & 0x0000ff00 >> 8 ) / 255.,
            ( c & 0x000000ff ) / 255.
            );
    return true;
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
    if( !Reader.parse( Contents, mRoot, false ) )
    {
        BOOST_ASSERT( false );
    }
    mValid = true;
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

