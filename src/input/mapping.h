#pragma once
#ifndef INCLUDED_INPUT_MAPPING_H
#define INCLUDED_INPUT_MAPPING_H

#include "platform/i_platform.h"

namespace input {
class Mapping : public platform::Singleton<Mapping>
{
    friend class platform::Singleton<Mapping>;
    Mapping();
    Json::Value mMappings;
public:
    Json::Value const& getMapping( std::string controlSchemeName );
};
}

#endif // INCLUDED_MAPPING_H

