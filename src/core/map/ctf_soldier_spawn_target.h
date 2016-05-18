#ifndef INCLUDED_CORE_MAP_CTF_SOLDIER_SPAWN_TARGET_H
#define INCLUDED_CORE_MAP_CTF_SOLDIER_SPAWN_TARGET_H

#include "platform/i_platform.h"
#include "i_target.h"
#include "../scene.h"
#include "../ctf_program_state.h"

namespace map {

class CtfSoldierSpawnTarget: public ITarget
{
public:
    CtfSoldierSpawnTarget( int32_t Id, int32_t curosrId, Team::Type team );
    virtual void Update( double DeltaTime );
    virtual int32_t GetCursorId() const;
    virtual int32_t GetActorId() const;
    virtual void PutTarget( glm::vec2 position );
    virtual std::auto_ptr<Actor> GetCursor();
protected:
    int32_t mCursorId;
    int32_t mActorId;
    Team::Type mTeam;
private:
    Scene& mScene;
};


} // namespace map
#endif//INCLUDED_CORE_MAP_CTF_SOLDIER_SPAWN_TARGET_H
