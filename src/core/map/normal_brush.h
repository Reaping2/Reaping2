#ifndef INCLUDED_CORE_MAP_NORMAL_BRUSH_H
#define INCLUDED_CORE_MAP_NORMAL_BRUSH_H
#include "platform/i_platform.h"
#include "i_brush.h"

namespace map {

class NormalBrush: public IBrush
{
public:
    NormalBrush( int32_t Id );
    virtual void Update( double DeltaTime );
protected:
    bool mMouseLeftPressed;
};

} // namespace map
#endif//INCLUDED_CORE_MAP_NORMAL_BRUSH_H