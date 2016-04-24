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
    std::vector<std::string> mGuns;
    std::vector<std::string> mBuffs;
    std::vector<std::string> mItems;
    std::vector<std::string> Guns();
    std::vector<std::string> Buffs();
    std::vector<std::string> Items();
    boost::ptr_vector<ModelValue> mEditorModels;
    void TargetChanged( std::string const& target );
};

} // namespace map

#endif//INCLUDED_MAP_EDITOR_TARGET_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "editor_target_system" -n "map"
