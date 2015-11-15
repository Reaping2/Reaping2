#ifndef INCLUDED_MAP_EDITOR_GRID_SYSTEM_H
#define INCLUDED_MAP_EDITOR_GRID_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "boost/ptr_container/ptr_vector.hpp"
#include "grid_repo.h"
#include "input/mouse.h"

namespace map {

class EditorGridSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE(EditorGridSystem)
    EditorGridSystem();
    ~EditorGridSystem();
    static Opt<EditorGridSystem> Get();
    IGrid& GetGrid();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    GridRepo& mGridRepo;
    int32_t mGridId;
    boost::ptr_vector<ModelValue> mEditorModels;
    void GridChanged(std::string const& grid);
    AutoReg mOnWorldMouseMove;
    void OnWorldMouseMoveEvent( const WorldMouseMoveEvent& Event );
};

} // namespace map

#endif//INCLUDED_MAP_EDITOR_GRID_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "editor_grid_system" -n "map"
