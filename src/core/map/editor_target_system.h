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
    int32_t GetNextUID() const;
    void SetNextUID( int32_t uid );
    void PutTarget( glm::vec2 position );
    void PutTarget( glm::vec2 position, IBorderComponent::Borders_t& borders, IBorderComponent::Borders_t& outerBorders );
    void RemoveCursor();
    void AddCursor();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    TargetRepo& mTargetRepo;
    int32_t mTargetId;
    glm::vec2 mCursorPosition;
    Opt<Actor> mCursor;
    int32_t mNextUID;
    std::vector<int32_t> mGunVisualIds;
    std::vector<int32_t> mGunActorIds;
    std::vector<int32_t> mBuffVisualIds;
    std::vector<int32_t> mBuffActorIds;
    std::vector<int32_t> mItemVisualIds;
    std::vector<int32_t> mItemActorIds;
    std::vector<int32_t> mMapitemVisualIds;
    std::vector<int32_t> mMapitemActorIds;
    std::vector<int32_t> mSpawnpointVisualIds;
    std::vector<int32_t> mSpawnpointVisualBackground;
    std::vector<int32_t> mSpawnpointActorIds;
    std::map<std::string,std::vector<int32_t>> mTargetActorIdsMap;
    std::vector<int32_t> Guns();
    std::vector<int32_t> Buffs();
    std::vector<int32_t> Items();
    std::vector<int32_t> MapItems();
    std::vector<int32_t> Spawnpoints();
    std::vector<int32_t> SpawnpointBackground();
    boost::ptr_vector<ModelValue> mEditorModels;
    void TargetChanged( std::string const& targetType, int32_t targetIdx );
};

} // namespace map

#endif//INCLUDED_MAP_EDITOR_TARGET_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "editor_target_system" -n "map"
