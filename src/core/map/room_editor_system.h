#ifndef INCLUDED_MAP_ROOM_EDITOR_SYSTEM_H
#define INCLUDED_MAP_ROOM_EDITOR_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "boost/ptr_container/ptr_vector.hpp"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "platform/register.h"
#include "main/window.h"
#include "render/camera.h"
#include "render/renderer.h"
#include "platform/frequency_timer.h"
#include "editor_layer.h"
#include "level_generator/room_desc.h"
#include "editor_back_event.h"
#include "../../platform/program_phases.h"

namespace map {

class RoomEditorSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE( RoomEditorSystem )
    RoomEditorSystem();
    ~RoomEditorSystem();
    static Opt<RoomEditorSystem> Get();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );

    void SitchToModeSelect();

private:
    Scene& mScene;
    ModelValue mEditorModel;
    ModelValue mLevelModel;
    ModelValue mStartModel;
    ModelValue mLoadModel;
    ModelValue mModeModel;
    ModelValue mSaveModel;
    double mX;
    double mY;
    double const& GetX() const;
    double const& GetY() const;
    boost::ptr_vector<ModelValue> mPlayerModels;
    std::string mRoomName;
    std::vector<std::string> mLevelNames;
    std::vector<std::string> LevelNames();

    Opt<engine::KeyboardSystem> mKeyboard;
    void Start();
    void Load( std::string const& room );
    void Save();
    void ModeSelect( std::string const& mode );

    std::string mEditorMode;
    uint32_t mCurrentMovement;
    Opt<engine::RendererSystem> mRenderer;
    Opt<engine::WindowSystem> mWindow;
    AutoReg mOnScreenMouseMove;
    void OnScreenMouseMove( ::ScreenMouseMoveEvent const& Evt );
    AutoReg mKeyId;
    void OnKeyEvent( const KeyEvent& Event );

    FrequencyTimer mTimer;
    bool mAutoSaveOn;
    RoomDesc mRoomDesc;
    int32_t mRoomId;
    AutoReg mOnEditorBack;
    void OnEditorBack( map::EditorBackEvent const& Evt );
    AutoReg mOnPhaseChanged;
    void OnPhaseChanged( PhaseChangedEvent const& Evt );
};

} // namespace map

#endif//INCLUDED_MAP_ROOM_EDITOR_SYSTEM_H



//command:  "classgenerator.exe" -g "system" -c "editor_system" -n "map"
