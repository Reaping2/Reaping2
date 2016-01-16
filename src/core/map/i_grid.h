#ifndef INCLUDED_CORE_MAP_I_GRID_H
#define INCLUDED_CORE_MAP_I_GRID_H

#include "platform/i_platform.h"
#include "neighbors.h"
namespace map {

class IGrid
{
public:
    IGrid( int32_t Id );
    virtual void Update( double DeltaTime )=0;
    virtual glm::vec2 GetProcessedPosition();
    virtual void SetMousePosition(double x, double y);
    virtual glm::vec2 GetMousePosition();
    virtual Neighbors GetNeighbors(glm::vec2 position, int32_t actorID);
protected:
    glm::vec2 mMousePosition;
    glm::vec2 mProcessedPosition;
    int32_t mId;
};



} // namespace map
#endif//INCLUDED_CORE_MAP_I_GRID_H