#ifndef INCLUDED_MAP_EDITOR_BRUSH_SYSTEM_H
#define INCLUDED_MAP_EDITOR_BRUSH_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "boost/ptr_container/ptr_vector.hpp"
#include "brush_repo.h"

namespace map {

class EditorBrushSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE( EditorBrushSystem )
    EditorBrushSystem();
    ~EditorBrushSystem();
    static Opt<EditorBrushSystem> Get();
    IBrush& GetBrush();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    BrushRepo& mBrushRepo;
    int32_t mBrushId;
    boost::ptr_vector<ModelValue> mEditorModels;
    void BrushChanged( std::string const& brush );
};

} // namespace map

#endif//INCLUDED_MAP_EDITOR_BRUSH_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "editor_brush_system" -n "map"
