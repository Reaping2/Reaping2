#ifndef INCLUDED_MAP_EDITOR_SYSTEM_H
#define INCLUDED_MAP_EDITOR_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "boost/ptr_container/ptr_vector.hpp"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "platform/register.h"
#include "main/window.h"
#include "render/camera.h"
#include "render/renderer.h"

namespace map {

class EditorSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE(EditorSystem)
    EditorSystem();
    ~EditorSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    ModelValue mEditorModel;
    ModelValue mStartModel;
    ModelValue mLoadModel;
    ModelValue mSaveModel;
    double mX;
    double mY;
    double const& GetX() const;
    double const& GetY() const;
    boost::ptr_vector<ModelValue> mPlayerModels;
    std::string mLevelName;

    Opt<engine::KeyboardSystem> mKeyboard;
    void Start();
    void Load(std::string const& level);
    void Save();

    uint32_t mCurrentMovement;
    Opt<engine::RendererSystem> mRenderer;
    Opt<engine::WindowSystem> mWindow;
    AutoReg mOnScreenMouseMove;
    void OnScreenMouseMove(::ScreenMouseMoveEvent const& Evt);
    AutoReg mKeyId;
    bool mHudState;
    bool mSpaceTyped;
    void OnKeyEvent( const KeyEvent& Event );
};

} // namespace map

#endif//INCLUDED_MAP_EDITOR_SYSTEM_H



//command:  "classgenerator.exe" -g "system" -c "editor_system" -n "map"
