#ifndef INCLUDED_MAP_EDITOR_TARGET_SYSTEM_H
#define INCLUDED_MAP_EDITOR_TARGET_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "boost/ptr_container/ptr_vector.hpp"
#include "target_repo.h"
#include "../actor_factory.h"

namespace map {

class EditorTargetSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE( EditorTargetSystem )
    EditorTargetSystem();
    ~EditorTargetSystem();
    static Opt<EditorTargetSystem> Get();
    ITarget& GetTarget();
    glm::vec2 GetCursorPosition() const;
    void SetCursorPosition( double x, double y );
    Opt<Actor> GetCursor() const;
    double GetCursorRadius() const;
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    TargetRepo& mTargetRepo;
    int32_t mTargetId;
    glm::vec2 mCursorPosition;
    Opt<Actor> mCursor;
    std::vector<int32_t> mGuns;
    std::vector<int32_t> mBuffs;
    std::vector<int32_t> mItems;
    std::vector<int32_t> mMapitems;
    std::vector<int32_t> mSpawnpoints;
    // some items (spawn points) have different ids for the visuals and the actors
    std::map<int32_t, int32_t> mVisualToActor;
    std::vector<int32_t> Guns();
    std::vector<int32_t> Buffs();
    std::vector<int32_t> Items();
    std::vector<int32_t> MapItems();
    std::vector<int32_t> Spawnpoints();
    boost::ptr_vector<ModelValue> mEditorModels;
    void TargetChanged( int32_t target );
};

} // namespace map

#endif//INCLUDED_MAP_EDITOR_TARGET_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "editor_target_system" -n "map"
