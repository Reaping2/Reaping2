#include "platform/i_platform.h"
#include "editor_system.h"
#include "engine/engine.h"
#include "engine/controllers/controller_system.h"
#include "ui/ui.h"
#include "engine/controllers/player_controller_sub_system.h"

namespace map {

EditorSystem::EditorSystem()
    : mScene( Scene::Get() )
    , mEditorModel( "editor", &RootModel::Get() )
    , mStartModel( VoidFunc( this, &EditorSystem::Start ), "start", &mEditorModel )
    , mLoadModel( StringFunc( this, &EditorSystem::Load ), "load", &mEditorModel )
    , mX(0)
    , mY(0)
    , mCurrentMovement(0)
{
}


void EditorSystem::Init()
{
    mKeyboard=::engine::Engine::Get().GetSystem<engine::KeyboardSystem>();
    mOnScreenMouseMove=EventServer<::ScreenMouseMoveEvent>::Get().Subscribe( boost::bind( &EditorSystem::OnScreenMouseMove, this, _1 ) );
    mWindow=engine::Engine::Get().GetSystem<engine::WindowSystem>();
    mRenderer=engine::Engine::Get().GetSystem<engine::RendererSystem>();

}



void EditorSystem::Start()
{
    ::engine::Engine::Get().SetEnabled< ::engine::ControllerSystem>(false);
}

void EditorSystem::Load(std::string const& level)
{
    mX=0;
    mY=0;
    ModelValue& PlayerModel = const_cast<ModelValue&>(RootModel::Get()["player"]);
    mPlayerModels.clear();
    mPlayerModels.push_back( new ModelValue( GetX(), "x", &PlayerModel ) );
    mPlayerModels.push_back( new ModelValue( GetY(), "y", &PlayerModel ) );

    mScene.Load(level);
    Ui::Get().Load("editor_hud");
}

double const& EditorSystem::GetX() const
{
    return mX;
}

double const& EditorSystem::GetY() const
{
    return mY;
}

EditorSystem::~EditorSystem()
{
     mPlayerModels.clear();
}



void EditorSystem::Update(double DeltaTime)
{
    glm::vec2 cameraCenter=mRenderer->GetCamera().GetCenter();
    mX=cameraCenter.x;
    mY=cameraCenter.y;
    uint32_t currentKeyMovement = 0;
    if( mKeyboard->GetKey(GLFW_KEY_W).State==KeyState::Down)
    {
        currentKeyMovement |= MF_Up;
    }
    if( mKeyboard->GetKey(GLFW_KEY_A).State==KeyState::Down )
    {
        currentKeyMovement |= MF_Left;
    }
    if( mKeyboard->GetKey(GLFW_KEY_S).State==KeyState::Down )
    {
        currentKeyMovement |= MF_Down;
    }
    if( mKeyboard->GetKey(GLFW_KEY_D).State==KeyState::Down )
    {
        currentKeyMovement |= MF_Right;
    }
    currentKeyMovement |= mCurrentMovement;
    mX += 15*(( ( currentKeyMovement & MF_Left ) ? -1 : 0 ) + ( ( currentKeyMovement & MF_Right ) ? 1 : 0 ));
    mY += 15*(( ( currentKeyMovement & MF_Up ) ? 1 : 0 ) + ( ( currentKeyMovement & MF_Down ) ? -1 : 0 ));

}
void EditorSystem::OnScreenMouseMove(::ScreenMouseMoveEvent const& Evt)
{
    int w, h;
    mWindow->GetWindowSize( w, h );
    mCurrentMovement = 0;
    if( Evt.Pos.y<100)
    {
        mCurrentMovement |= MF_Up;
    }
    if( Evt.Pos.x<100 )
    {
        mCurrentMovement |= MF_Left;
    }
    if(Evt.Pos.y>h-150 )
    {
        mCurrentMovement |= MF_Down;
    }
    if( Evt.Pos.x>w-100 )
    {
        mCurrentMovement |= MF_Right;
    }
}

} // namespace map

