#ifndef INCLUDED_PLATFORM_JSONREADER_TEMPLATES_H
#define INCLUDED_PLATFORM_JSONREADER_TEMPLATES_H

#include "jsonreader.h"

namespace Json {
bool Get( Value const& V, std::string& O );
bool Get( Value const& V, uint32_t& O );
bool Get( Value const& V, int32_t& O );
bool Get( Value const& V, double& O );
bool Get( Value const& V, float& O );
bool Get( Value const& V, bool& O );
bool Get( Value const& V, glm::vec4& O );
} // namespace Json

#endif//INCLUDED_PLATFORM_JSONREADER_TEMPLATES_H
