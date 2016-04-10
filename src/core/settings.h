#ifndef INCLUDED_CORE_SETTINGS_H
#define INCLUDED_CORE_SETTINGS_H
#include "platform/i_platform.h"

class Settings : public platform::Singleton<Settings>
{
public:
    std::string GetStr(std::string const& key, std::string const& Default="") const;
    uint32_t GetUInt(std::string const& key, uint32_t Default = 0) const;
    int32_t GetInt(std::string const& key, int32_t Default = 0) const;
    double GetDouble(std::string const& key, double Default = 0.0) const;
    float GetFloat(std::string const& key, float Default = 0.0f) const;
    int32_t GetColor(std::string const& key, int32_t Default = 0.0f) const;
    glm::vec4 GetColor(std::string const& key, glm::vec4 Default = glm::vec4(0.0)) const;
    Json::Value Resolve(std::string const& key) const;
private:
    friend class platform::Singleton<Settings>;
    Settings();
    void Init();
    Json::Value mRoot;
};
#endif//INCLUDED_CORE_SETTINGS_H
