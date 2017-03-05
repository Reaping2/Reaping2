#include "jsonreader_templates.h"

namespace Json {

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
