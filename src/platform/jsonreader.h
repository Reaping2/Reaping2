#ifndef INCLUDED_PLATFORM_JSONREADER_H
#define INCLUDED_PLATFORM_JSONREADER_H

class JsonReader
{
    Json::Value mRoot;
    bool mValid;
public:
    JsonReader( File& F );
    bool IsValid()const;
    Json::Value& GetRoot();
};

namespace Json {  // helpers
bool GetStr( Json::Value const& V, std::string& O );
bool GetUInt( Json::Value const& V, uint32_t& O );
bool GetInt( Json::Value const& V, int32_t& O );
bool GetDouble( Json::Value const& V, double& O );
}

#endif//INCLUDED_PLATFORM_JSONREADER_H
