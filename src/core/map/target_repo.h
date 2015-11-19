#ifndef INCLUDED_MAP_TARGET_REPO_H
#define INCLUDED_MAP_TARGET_REPO_H

#include "platform/repository.h"
#include "platform/singleton.h"
#include "i_target.h"

namespace map {
    
class DefaultTarget: public ITarget
{
public:
    DefaultTarget();
    virtual void Update( double DeltaTime );
    virtual void PutTarget(glm::vec2 position);
};


class TargetRepo : public platform::Repository<ITarget>, public platform::Singleton<TargetRepo>
{
    friend class platform::Singleton<TargetRepo>;
    static DefaultTarget const mDefault;
    TargetRepo();
};

} // namespace map

#endif//INCLUDED_MAP_TARGET_REPO_H

//command:  "classgenerator.exe" -g "repository" -c "target_repo" -n "map" -t "i_target"
