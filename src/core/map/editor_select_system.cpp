#include "platform/i_platform.h"
#include "editor_select_system.h"
#include "ui/ui.h"
#include "editor_grid_system.h"
#include "editor_target_system.h"
#include "editor_brush_system.h"
#include "../../engine/engine.h"
#include "../../input/keyboard.h"
#include "editor_hud_state.h"
#include "../actor.h"
#include "../actor_factory.h"
#include "../i_position_component.h"
#include "../i_cell_component.h"
#include "../i_collision_component.h"
#include "input/mouse.h"
#include "group_map_element.h"
#include "map_system.h"
#include "../../platform/id_storage.h"

namespace map {

EditorSelectSystem::EditorSelectSystem()
    : mScene( Scene::Get() )
    , mEditorSelectModel( "editor_select", &RootModel::Get() )
    , mMouseModeModel( StringFunc( this, &EditorSelectSystem::SetMouseMode ), "mode", &mEditorSelectModel )
{
}


void EditorSelectSystem::Init()
{
    mOnEditorModeChanged = EventServer<map::EditorModeChangedEvent>::Get().Subscribe( boost::bind( &EditorSelectSystem::OnEditorModeChanged, this, _1 ) );
    mOnEditorBack = EventServer<map::EditorBackEvent>::Get().Subscribe( boost::bind( &EditorSelectSystem::OnEditorBack, this, _1 ) );
    mMouseMoveId = EventServer<WorldMouseMoveEvent>::Get().Subscribe( boost::bind( &EditorSelectSystem::OnMouseMoveEvent, this, _1 ) );
    mOnGroupSelected = EventServer<map::GroupSelectedEvent>::Get().Subscribe( boost::bind( &EditorSelectSystem::OnGroupSelected, this, _1 ) );
}


void EditorSelectSystem::Update(double DeltaTime)
{
    if (!mEnabled)
    {
        return;
    }
    static Opt<engine::KeyboardSystem> keyboard = ::engine::Engine::Get().GetSystem<engine::KeyboardSystem>();
    static Opt<engine::MouseSystem> mouse = ::engine::Engine::Get().GetSystem<MouseSystem>();
    if (keyboard->GetKey( GLFW_KEY_SPACE ).State == KeyState::Typed)
    {
        if (EditorHudState::Get().IsHudShown())
        {
            EnableSubsystems( true );
            EventServer<EditorBackEvent>::Get().SendEvent( EditorBackEvent( true ) );
        }
        else
        {
            EnableSubsystems( false );
            Ui::Get().Load( "editor_select_hud" );
            EditorHudState::Get().SetHudShown( true );
        }
    }
    if (!EditorHudState::Get().IsHudShown())
    {
        if (keyboard->GetKey( GLFW_KEY_F ).State == KeyState::Typed
            || keyboard->GetKey( GLFW_KEY_KP_5 ).State == KeyState::Typed)
        {
        }

        if (!mMouseLeftPressed && mouse->IsButtonPressed( MouseSystem::Button_Left ))
        {
            if (keyboard->GetKey( GLFW_KEY_LEFT_CONTROL ).State != KeyState::Down
                && keyboard->GetKey( GLFW_KEY_RIGHT_CONTROL ).State != KeyState::Down)
            {
                mSelectStarted = true;
                mSelectStartPos = mMousePos;
                if (keyboard->GetKey( GLFW_KEY_LEFT_SHIFT ).State != KeyState::Down
                    && keyboard->GetKey( GLFW_KEY_RIGHT_SHIFT ).State != KeyState::Down)
                {
                    SetActorColors( mSelectedActors, nullptr );
                    mSelectedActors.clear();
                }
            }
        }

        if (mMouseLeftPressed && !mouse->IsButtonPressed( MouseSystem::Button_Left ))
        {
            if (keyboard->GetKey( GLFW_KEY_LEFT_CONTROL ).State == KeyState::Down
                || keyboard->GetKey( GLFW_KEY_RIGHT_CONTROL ).State == KeyState::Down)
            {
                mSelectStarted = true;
                mSelectStartPos = mMousePos;
                mSelectEndPos = mMousePos;
                UpdateSelectedActors();
                mSelectStarted = false;                
                mSelectedActors.insert( mCurrentSelectedActors.begin(), mCurrentSelectedActors.end() );
            }
            else if (keyboard->GetKey( GLFW_KEY_LEFT_SHIFT ).State == KeyState::Down
                || keyboard->GetKey( GLFW_KEY_RIGHT_SHIFT ).State == KeyState::Down)
            {
                mSelectedActors.insert( mCurrentSelectedActors.begin(), mCurrentSelectedActors.end() );
            }
            else
            {
                SetActorColors( mSelectedActors, nullptr );
                mSelectedActors = mCurrentSelectedActors;
            }
            mSelectStarted = false;
            mMouseLeftPressed = false;
        }
        else if (mouse->IsButtonPressed( MouseSystem::Button_Left ))
        {
            mSelectEndPos = mMousePos;
            mMouseLeftPressed = true;
        }

        if (mMouseRightPressed && !mouse->IsButtonPressed( MouseSystem::Button_Right ))
        {
            if (keyboard->GetKey( GLFW_KEY_LEFT_CONTROL ).State == KeyState::Down
                || keyboard->GetKey( GLFW_KEY_RIGHT_CONTROL ).State == KeyState::Down)
            {
                mSelectStarted = true;
                mSelectStartPos = mMousePos;
                mSelectEndPos = mMousePos;
                UpdateSelectedActors();
                mSelectStarted = false;
                for (auto& actorGUID : mCurrentSelectedActors)
                {
                    mSelectedActors.erase( actorGUID.first );
                }
            }
            mMouseRightPressed = false;
        }
        else if (mouse->IsButtonPressed( MouseSystem::Button_Right ))
        {
            mMouseRightPressed = true;
        }
        SetActorColors( mCurrentSelectedActors, nullptr );
        UpdateSelectedActors();
        glm::vec4 selectColor = glm::vec4( 0, 0, 0.5, 1 );
        SetActorColors( mCurrentSelectedActors, &selectColor );
        SetActorColors( mSelectedActors, &selectColor );
    }
}

void EditorSelectSystem::OnEditorModeChanged(map::EditorModeChangedEvent const& Evt)
{
    if (Evt.mMode == "select")
    {
        EnableSubsystems( false );
        ::engine::Engine::Get().SetEnabled<EditorSelectSystem>( true );
        Ui::Get().Load( "editor_select_hud" );
        EditorHudState::Get().SetHudShown( true );
    }
    else
    {
        EnableSubsystems( false );
        ::engine::Engine::Get().SetEnabled<EditorSelectSystem>( false );
    }
}

void EditorSelectSystem::OnEditorBack( map::EditorBackEvent const& Evt )
{
    if (mEnabled)
    {
        if (!Evt.mBackToBaseHud)
        {
            EnableSubsystems( false );
            Ui::Get().Load( "editor_select_hud" );
            EditorHudState::Get().SetHudShown( true );
        }
    }
}

void EditorSelectSystem::EnableSubsystems( bool enable )
{
}

void EditorSelectSystem::RemoveCells()
{
    for (auto cellGUID : mCellGUIDs)
    {
        mScene.RemoveActor( cellGUID );
    }
}

void EditorSelectSystem::AddCells()
{
    static ActorFactory& actorFactory = ActorFactory::Get();
    static int32_t cellId = AutoId( "cell" );
    RemoveCells();
    mCellGUIDs.clear();
    std::auto_ptr<Actor> cellActor( actorFactory( cellId ) );
    Opt<IPositionComponent> positionC( cellActor->Get<IPositionComponent>() );
    if (positionC.IsValid())
    {
    }
    auto CollisionC( cellActor->Get<ICollisionComponent>() );
    if (CollisionC.IsValid())
    {
        CollisionC->SetRadius( 100 );
    }

    mScene.AddActor( cellActor.release() );
}

Opt<EditorSelectSystem> EditorSelectSystem::Get()
{
    return engine::Engine::Get().GetSystem<EditorSelectSystem>();
}

void EditorSelectSystem::OnMouseMoveEvent( const WorldMouseMoveEvent& Event )
{
    mMousePos = Event.Pos;
}

void EditorSelectSystem::SetMouseMode( std::string mode )
{
    EventServer<EditorBackEvent>::Get().SendEvent( EditorBackEvent( true ) );
}

void EditorSelectSystem::UpdateSelectedActors()
{
    mCurrentSelectedActors.clear();
    if (!mSelectStarted)
    {
        return;
    }
    for (auto actor : mScene.GetActors())
    {
        auto positionC( actor->Get<IPositionComponent>() );
        if (positionC.IsValid())
        {
            int32_t x = positionC->GetX();
            int32_t y = positionC->GetY();
            auto collisionC( actor->Get<ICollisionComponent>() );
            int32_t radius = 0;
            if (collisionC.IsValid())
            {
                radius = collisionC->GetRadius();
            }
            if (std::min( mSelectStartPos.x, mSelectEndPos.x ) <= x+radius
                && x-radius <= std::max( mSelectStartPos.x, mSelectEndPos.x )
                && std::min( mSelectStartPos.y, mSelectEndPos.y ) <= y+radius
                && y-radius <= std::max( mSelectStartPos.y, mSelectEndPos.y ))
            {
                auto renderableC( actor->Get<IRenderableComponent>() );
                auto color = renderableC.IsValid() ? renderableC->GetColor() : glm::vec4( 1.0 );
                auto found = mSelectedActors.find( actor->GetGUID() );
                if (found != mSelectedActors.end())
                {
                    // this actor is already selected so the color is the selected color. taking color from the selected and saved colors
                    color = found->second;
                }
                mCurrentSelectedActors.emplace( actor->GetGUID(), color );
            }
        }
    }
}

void EditorSelectSystem::SetActorColors( std::map<int32_t, glm::vec4> const& actorGUIDs, Opt<glm::vec4> col )
{
    for (auto& actorGUID : actorGUIDs)
    {
        auto actor = mScene.GetActor( actorGUID.first );
        auto renderableC( actor->Get<IRenderableComponent>() );
        if (renderableC.IsValid())
        {
            renderableC->SetColor( col.Get()?*col:actorGUID.second );
        }
    }

}

void EditorSelectSystem::OnGroupSelected( map::GroupSelectedEvent const& Evt )
{
    if (!mEnabled)
    {
        return;
    }
    int32_t groupId = AutoId(Evt.mGroupName);
    GroupMapElement::Targets_t targets;
    ActorColors_t newSelectedActors;
    SetUIDUniqueForSelectedActors();
    for (auto&& actorColor : mSelectedActors)
    {
        int32_t actorGUID = actorColor.first;
        for (auto mapElement : MapSystem::Get()->GetMapElementList())
        {
            if (mapElement->GetSpawnedActorGUID() == actorGUID)
            {
                targets.insert( mapElement->GetUID() );
            }
        }
    }
    for (Opt<GroupMapElement> groupMapElement : MapElementListFilter<MapSystem::UID>( MapSystem::Get()->GetMapElementList(), groupId ))
    {
        auto newTargets = targets;
        newTargets.insert(groupMapElement->GetTargets().begin(), groupMapElement->GetTargets().end() );
        groupMapElement->SetTargets( newTargets );
    }
}

void EditorSelectSystem::SetUIDUniqueForSelectedActors()
{
    // basically a newly created actor on map has the same uid (ensures to always spawn to the map instantly)
    // to avoid this, have to check if an actor has a uid like this
    // if so it has to be changed for some really unique id to put the specific actor to a group
    static int32_t spawnID = EditorTargetSystem::Get()->GetNextUID();
    std::vector<Opt<MapElement>> mapElementsToModify;
    for (auto&& actorColor : mSelectedActors)
    {
        int32_t actorGUID = actorColor.first;
        for (auto mapElement : MapSystem::Get()->GetMapElementList())
        {
            if (mapElement->GetSpawnedActorGUID() == actorGUID
                && mapElement->GetUID() == spawnID)
            {
                mapElementsToModify.push_back( mapElement );
            }
        }
    }
    int32_t nextIndex = GetNextUniqueSpawnIndex();
    for (auto mapElement : mapElementsToModify)
    {
        for (auto i = MapSystem::Get()->GetMapElementList().begin(), e = MapSystem::Get()->GetMapElementList().end(); i != e;++i)
        {
            if (i->Get() == mapElement.Get())
            {
                MapSystem::Get()->GetMapElementList().modify(
                i, MapElementUIDModifier( AutoId( "a" + std::to_string( nextIndex++ ) ) ) );
                break;
            }
        }
    }
}

int32_t EditorSelectSystem::GetNextUniqueSpawnIndex()
{
    int max = 0;
    static IdStorage& idStorage = IdStorage::Get();
    for (auto mapElement : MapSystem::Get()->GetMapElementList())
    {
        try
        {
            std::string mapElementUName;
            if (idStorage.GetName( mapElement->GetUID(), mapElementUName ))
            {
                if (boost::starts_with( mapElementUName, "a" ))
                {
                    max = std::max( std::stoi( mapElementUName.substr( 1 ) ), max );
                }
            }
        }
        catch (...)
        {
        }
    }
    return ++max;
}

} // namespace map

