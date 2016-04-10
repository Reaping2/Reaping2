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

std::string Settings::GetStr(std::string const& key, std::string const& Default/*=""*/) const
{
    std::string r=Default;
    Json::GetStr(Resolve(key), r);
    return r;
}

uint32_t Settings::GetUInt(std::string const& key, uint32_t Default /*= 0*/) const
{
    Json::GetUInt(Resolve(key), Default);
    return Default;
}

int32_t Settings::GetInt(std::string const& key, int32_t Default /*= 0*/) const
{
    Json::GetInt(Resolve(key), Default);
    return Default;
}

double Settings::GetDouble(std::string const& key, double Default /*= 0.0*/) const
{
    Json::GetDouble(Resolve(key), Default);
    return Default;
}

float Settings::GetFloat(std::string const& key, float Default /*= 0.0f*/) const
{
    Json::GetFloat(Resolve(key), Default);
    return Default;
}

int32_t Settings::GetColor(std::string const& key, int32_t Default /*= 0.0f*/) const
{
    Json::GetColor(Resolve(key), Default);
    return Default;
}

glm::vec4 Settings::GetColor(std::string const& key, glm::vec4 Default /*= glm::vec4(0.0)*/) const
{
    Json::GetColor(Resolve(key), Default);
    return Default;
}

// TODO: multiple settings hierarchy file handling should be here
Json::Value Settings::Resolve(std::string const& key) const
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

