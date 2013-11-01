#ifndef INCLUDED_PLATFORM_JSONREADER_H
#define INCLUDED_PLATFORM_JSONREADER_H

class JsonReader
{
	Json::Value mRoot;
	bool mValid;
public:
	JsonReader(File& F);
	bool IsValid()const;
	Json::Value& GetRoot();
};

namespace Json
{	// helpers
	bool GetStr(Json::Value& V, std::string& O);
	bool GetUInt(Json::Value& V, uint32_t& O);
}

#endif//INCLUDED_PLATFORM_JSONREADER_H
