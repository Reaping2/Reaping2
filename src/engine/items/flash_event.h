#ifndef INCLUDED_ENGINE_FLASH_EVENT_H
#define INCLUDED_ENGINE_FLASH_EVENT_H

#include "platform/event.h"

namespace engine {

struct FlashEvent : public platform::Event
{
    int32_t mActorGUID;
    double mOriginalX;
    double mOriginalY;
    double mX;
    double mY;
    FlashEvent(int32_t actorGUID, double originalX, double originalY, double X, double Y)
        :mActorGUID(actorGUID),mOriginalX(originalX),mOriginalY(originalY),mX(X),mY(Y){}
};

} // namespace engine

#endif//INCLUDED_ENGINE_FLASH_EVENT_H
