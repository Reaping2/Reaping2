#ifndef INCLUDED_PLATFORM_JSONREADER_H
#define INCLUDED_PLATFORM_JSONREADER_H

#include <json/json.h>
#include <string>
#include <stdint.h>

// TODO: remove this, uncomment the one inside platform
namespace Json {
bool GetStr( Json::Value const& V, std::string& O );
bool GetUInt( Json::Value const& V, uint32_t& O );
bool GetInt( Json::Value const& V, int32_t& O );
bool GetDouble( Json::Value const& V, double& O );
} // namespace Json

namespace platform {
class File;
/*
namespace Json {
using namespace ::Json;
bool GetStr( Json::Value const& V, std::string& O );
bool GetUInt( Json::Value const& V, uint32_t& O );
bool GetInt( Json::Value const& V, int32_t& O );
bool GetDouble( Json::Value const& V, double& O );
} // namespace Json
*/
class JsonReader
{
    Json::Value mRoot;
    bool mValid;
public:
    JsonReader( File& F );
    bool IsValid()const;
    Json::Value& GetRoot();
};

} // namespace platform

#endif//INCLUDED_PLATFORM_JSONREADER_H
