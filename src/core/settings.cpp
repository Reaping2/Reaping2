#include "core/settings.h"

Settings::Settings()
{
    Init();
}

void Settings::Init()
{
    Filesys& Fsys(Filesys::Get());
    AutoFile F = Fsys.Open(boost::filesystem::path("settings.json"));
    if (!F.get() || !F->IsValid())
    {
        return;
    }
    JsonReader Reader(*F);
    if (!Reader.IsValid())
    {
        return;
    }
    mRoot = Reader.GetRoot();
}

std::string Settings::GetStr(std::string const& key, std::string const& default/*=""*/) const
{
    std::string r=default;
    Json::GetStr(Resolve(key), r);
    return r;
}

uint32_t Settings::GetUInt(std::string const& key, uint32_t default /*= 0*/) const
{
    Json::GetUInt(Resolve(key), default);
    return default;
}

int32_t Settings::GetInt(std::string const& key, int32_t default /*= 0*/) const
{
    Json::GetInt(Resolve(key), default);
    return default;
}

double Settings::GetDouble(std::string const& key, double default /*= 0.0*/) const
{
    Json::GetDouble(Resolve(key), default);
    return default;
}

float Settings::GetFloat(std::string const& key, float default /*= 0.0f*/) const
{
    Json::GetFloat(Resolve(key), default);
    return default;
}

int32_t Settings::GetColor(std::string const& key, int32_t default /*= 0.0f*/) const
{
    Json::GetColor(Resolve(key), default);
    return default;
}

glm::vec4 Settings::GetColor(std::string const& key, glm::vec4 default /*= glm::vec4(0.0)*/) const
{
    Json::GetColor(Resolve(key), default);
    return default;
}

// TODO: multiple settings hierarchy file handling should be here
Json::Value& Settings::Resolve(std::string const& key) const
{
    std::vector<std::string> keySplit;
    boost::split(keySplit, key, boost::is_any_of("."));
    Json::Value r = mRoot;
    for (auto const& i : keySplit)
    {
        r = r[i];
    }
    return r;
}

