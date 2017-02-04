#ifndef INCLUDED_MAP_MAP_REPO_H
#define INCLUDED_MAP_MAP_REPO_H

#include "platform/repository.h"
#include "platform/singleton.h"
#include <json/json.h>

namespace map {

// Repository to store metadata about maps, the content of description.json
class MapRepo : public platform::Repository<Json::Value>, public platform::Singleton<MapRepo>
{
    friend class platform::Singleton<MapRepo>;
    static Json::Value const mDefault;
    MapRepo();
    void Init();
public:
    static std::string const mMapDir;
};

} // namespace map

#endif//INCLUDED_MAP_MAP_REPO_H

//command:  "./classgenerator" -g "repository" -c "map_repo" -n "map" -t "Json::Value"
