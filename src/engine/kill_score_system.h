#ifndef INCLUDED_ENGINE_KILL_SCORE_SYSTEM_H
#define INCLUDED_ENGINE_KILL_SCORE_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "kill_score_event.h"

namespace engine {

class KillScoreSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(KillScoreSystem)
    KillScoreSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    AutoReg mOnKillScore;
    core::ProgramState& mProgramState;
    ModelValue mScoreModel;
    boost::ptr_vector<ModelValue> mScoreModels;
    void OnKillScore(engine::KillScoreEvent const& Evt);
    int32_t mKill;
    int32_t mDeath;
    int32_t mScore;
    int32_t const& GetKill();
    int32_t const& GetDeath();
    int32_t const& GetScore();
};

} // namespace engine

#endif//INCLUDED_ENGINE_KILL_SCORE_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "kill_score_system" -e "engine-killScore"
