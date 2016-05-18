#ifndef INCLUDED_CORE_MAP_PICKUP_TARGET_H
#define INCLUDED_CORE_MAP_PICKUP_TARGET_H

#include "platform/i_platform.h"
#include "i_target.h"
#include "../scene.h"

namespace map {

class PickupTarget: public ITarget
{
public:
    PickupTarget( int32_t Id, int32_t typeId, int32_t contentId );
    virtual void Update( double DeltaTime );
    virtual int32_t GetCursorId() const;
    virtual int32_t GetActorId() const;
    virtual void PutTarget( glm::vec2 position );
    virtual std::auto_ptr<Actor> GetCursor();
protected:
    int32_t mCursorId;
    int32_t mActorId;
    int32_t mTypeId;
    int32_t mContentId;
private:
    Scene& mScene;
};


} // namespace map
#endif//INCLUDED_CORE_MAP_PICKUP_TARGET_H
