#ifndef INCLUDED_MAP_BRUSH_REPO_H
#define INCLUDED_MAP_BRUSH_REPO_H

#include "platform/repository.h"
#include "platform/singleton.h"
#include "i_brush.h"

namespace map {

class DefaultBrush: public IBrush
{
public:
    DefaultBrush();
    virtual void Update( double DeltaTime );
};

class BrushRepo : public platform::Repository<IBrush>, public platform::Singleton<BrushRepo>
{
    friend class platform::Singleton<BrushRepo>;
    static DefaultBrush const mDefault;
    BrushRepo();
};

} // namespace map

#endif//INCLUDED_MAP_BRUSH_REPO_H

//command:  "classgenerator.exe" -g "repository" -c "brush_repo" -n "map" -t "i_brush"
