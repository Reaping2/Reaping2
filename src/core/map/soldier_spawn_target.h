#ifndef INCLUDED_CORE_MAP_SOLDIER_SPAWN_TARGET_H
#define INCLUDED_CORE_MAP_SOLDIER_SPAWN_TARGET_H

#include "platform/i_platform.h"
#include "i_target.h"
#include "../scene.h"

namespace map {

class SoldierSpawnTarget: public ITarget
{
public:
    SoldierSpawnTarget( int32_t Id );
    SoldierSpawnTarget( int32_t Id, int32_t curosrId );
    virtual void Update( double DeltaTime );
    virtual int32_t GetCursorId() const;
    virtual int32_t GetActorId() const;
    virtual void PutTarget( glm::vec2 position );
    virtual std::auto_ptr<Actor> GetCursor();
    virtual bool Load( const Json::Value& setters );
protected:
    int32_t mCursorId;
    int32_t mActorId;
private:
    Scene& mScene;
};


} // namespace map
#endif//INCLUDED_CORE_MAP_SOLDIER_SPAWN_TARGET_H
