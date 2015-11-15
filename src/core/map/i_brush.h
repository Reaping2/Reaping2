#ifndef INCLUDED_CORE_MAP_I_BRUSH_H
#define INCLUDED_CORE_MAP_I_BRUSH_H
#include "platform/i_platform.h"

namespace map {

class IBrush
{
public:
    IBrush( int32_t Id );
    virtual void Update( double DeltaTime )=0;
protected:
    int32_t mId;
};

} // namespace map
#endif//INCLUDED_CORE_MAP_I_BRUSH_H