#include "platform/i_platform.h"
#include "editor_target_system.h"
#include "engine/engine.h"
#include "../i_position_component.h"

namespace map {

EditorTargetSystem::EditorTargetSystem()
    : mScene( Scene::Get() )
    , mTargetRepo(TargetRepo::Get())
    , mActorFactory(ActorFactory::Get())
    , mTargetId(-1)
    , mCursorPosition(0.0,0.0)
    , mCursor(NULL)
{
}


void EditorTargetSystem::Init()
{
    ModelValue& editorModel = const_cast<ModelValue&>(RootModel::Get()["editor"]);
    mEditorModels.push_back(new ModelValue( StringFunc(this,&EditorTargetSystem::TargetChanged),"target",&editorModel));
}

void EditorTargetSystem::Update(double DeltaTime)
{
    GetTarget().Update(DeltaTime);
    if (mCursor.IsValid())
    {
        Opt<IPositionComponent> positionC(mCursor->Get<IPositionComponent>());
        if (positionC.IsValid())
        {
            positionC->SetX(mCursorPosition.x);
            positionC->SetY(mCursorPosition.y);
        }
    }
}

void EditorTargetSystem::TargetChanged(std::string const& target)
{
    if (mCursor.IsValid())
    {
        mScene.RemoveActor(mCursor->GetGUID());
    }
    mTargetId=AutoId(target);
    std::auto_ptr<Actor> cursor(mActorFactory(GetTarget().GetCursorId()));
    Opt<IPositionComponent> positionC(cursor->Get<IPositionComponent>());
    if (positionC.IsValid())
    {
        positionC->SetX(mCursorPosition.x);
        positionC->SetY(mCursorPosition.y);
    }
    int32_t guid=cursor->GetGUID();
    mScene.AddActor(cursor.release());
    mCursor = mScene.GetActor(guid);
}

EditorTargetSystem::~EditorTargetSystem()
{
    mEditorModels.clear();
}

Opt<EditorTargetSystem> EditorTargetSystem::Get()
{
    return engine::Engine::Get().GetSystem<EditorTargetSystem>();
}

ITarget& EditorTargetSystem::GetTarget()
{
    return mTargetRepo(mTargetId);
}

glm::vec2 EditorTargetSystem::GetCursorPosition() const
{
    return mCursorPosition;
}

void EditorTargetSystem::SetCursorPosition(double x, double y)
{
    mCursorPosition.x=x;
    mCursorPosition.y=y;
}


} // namespace map

