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
    , mEditorSelectStateModel( StringFunc( this, &EditorSelectSystem::SetEditorSelectState ), "state", &mEditorSelectModel )
    , mRemoveFromAllGroupsModel( VoidFunc( this, &EditorSelectSystem::OnRemoveFromAllGroups ), "remove_from_all_groups", &mEditorSelectModel )
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
            SetActorColors( mGroupPreSelectedActors, nullptr );
            mGroupPreSelectedActors.clear();
            mGroupPreSelectName.clear();
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
        SetActorColors( mCurrentSelectedActors, nullptr );
        SetActorColors( mSelectedActors, nullptr );
        SetActorColors( mGroupPreSelectedActors, nullptr );

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
                std::vector<int32_t> eraseActors;
                for (auto& actorGUID : mSelectedActors)
                {
                    if (mCurrentSelectedActors.find(actorGUID.first)!=mCurrentSelectedActors.end())
                    {
                        eraseActors.push_back( actorGUID.first );
                    }
                }
                for (auto actorGUID : eraseActors)
                {
                    mSelectedActors.erase( actorGUID );
                }
                if (eraseActors.empty())
                {
                    mSelectedActors.insert( mCurrentSelectedActors.begin(), mCurrentSelectedActors.end() );
                }
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
            mSelectedActors.clear();
            mCurrentSelectedActors.clear();
            mMouseRightPressed = false;
        }
        else if (mouse->IsButtonPressed( MouseSystem::Button_Right ))
        {
            mMouseRightPressed = true;
        }
        UpdateSelectedActors();
        SetActorColors( mCurrentSelectedActors, &mSelectColor );
        SetActorColors( mSelectedActors, &mSelectColor );
        SetActorColors( mGroupPreSelectedActors, &mPreSelectColor );

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
        SetActorColors( mCurrentSelectedActors, nullptr );
        SetActorColors( mSelectedActors, nullptr );
        SetActorColors( mGroupPreSelectedActors, nullptr );
        mCurrentSelectedActors.clear();
        mSelectedActors.clear();
        mGroupPreSelectedActors.clear();
        mGroupPreSelectName.clear();
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
    auto mapSystem = MapSystem::Get();
    if (mSelectState == AddToGroup)
    {
        if (mGroupPreSelectName == Evt.mGroupName
            || Evt.mNew )
        {
            int32_t groupId = AutoId( Evt.mGroupName );
            GroupMapElement::Targets_t targets;
            ActorColors_t newSelectedActors;
            SetUIDUniqueForSelectedActors();
            for (auto&& actorColor : mSelectedActors)
            {
                int32_t actorGUID = actorColor.first;
                auto mapElement(mapSystem->GetMapElement( actorGUID ));
                if (mapElement.IsValid())
                {
                    targets.insert( mapElement->GetIdentifier() );
                }
            }
            for (Opt<GroupMapElement> groupMapElement : MapElementListFilter<MapSystem::Identifier>( MapSystem::Get()->GetMapElementList(), groupId )) // all groups - most probably one
            {
                auto newTargets = targets;
                newTargets.insert( groupMapElement->GetTargets().begin(), groupMapElement->GetTargets().end() );
                groupMapElement->SetTargets( newTargets );
            }
            mSelectedActors.insert( mGroupPreSelectedActors.begin(), mGroupPreSelectedActors.end() );
            SetActorColors( mGroupPreSelectedActors, nullptr );
            SetActorColors( mSelectedActors, &mSelectColor );
            mGroupPreSelectedActors.clear();
            mGroupPreSelectName.clear();
            EventServer<EditorBackEvent>::Get().SendEvent( EditorBackEvent( true ) );
        }
        else // TODO: temporary, showing of group actors will be handled on mouse over
        {
            mGroupPreSelectName = Evt.mGroupName;
            SetActorColors( mGroupPreSelectedActors, nullptr );
            mGroupPreSelectedActors.clear();
            int32_t groupId = AutoId( Evt.mGroupName );
            for (Opt<GroupMapElement> groupMapElement : MapElementListFilter<MapSystem::Identifier>( MapSystem::Get()->GetMapElementList(), groupId )) // all groups - most probably one
            {
                for (auto target : groupMapElement->GetTargets())
                {
                    for (Opt<MapElement> mapElement : MapElementListFilter<MapSystem::Identifier>( MapSystem::Get()->GetMapElementList(), target )) // all targets - could be more
                    {
                        auto actor( mScene.GetActor( mapElement->GetSpawnedActorGUID() ) );
                        if (actor.IsValid())
                        {
                            auto renderableC( actor->Get<IRenderableComponent>() );
                            auto color = renderableC.IsValid() ? renderableC->GetColor() : glm::vec4( 1.0 );
                            mGroupPreSelectedActors.emplace( actor->GetGUID(), color );
                        }
                    }
                }
            }
            SetActorColors( mGroupPreSelectedActors, &mPreSelectColor );
        }
    }
    else if (mSelectState == RemoveFromGroup)
    {
        int32_t groupId = AutoId( Evt.mGroupName );
        std::vector<int32_t> grouplessMapElements;
        for (auto actorColor : mSelectedActors)
        {
            bool found = false;
            auto mapElement( mapSystem->GetMapElement( actorColor.first ) );
            if (!mapElement.IsValid())
            {
                continue; // this should not happen all actors should have a spawn actor map element at least
            }
            // check if there is another group containing this element if so the elements identifier remains the same. Otherwise it will be reset to spawn at start
            for (Opt<GroupMapElement> groupMapElement : MapElementListFilter<MapSystem::All>( MapSystem::Get()->GetMapElementList(), GroupMapElement::GetType_static() )) // all groups - most probably one
            {
                if (groupMapElement->GetIdentifier() == groupId)
                {
                    GroupMapElement::Targets_t targets=groupMapElement->GetTargets();
                    targets.erase( mapElement->GetIdentifier() );
                    groupMapElement->SetTargets( targets );
                }
                else if (!found)
                {
                    auto mapElement( mapSystem->GetMapElement( actorColor.first ) );
                    if (groupMapElement->GetTargets().find( mapElement->GetIdentifier() ) != groupMapElement->GetTargets().end())
                    {
                        found = true;
                    }
                }
            }
            if (!found)
            {
                grouplessMapElements.push_back( mapElement->GetUID() );
            }
        }

        static int32_t spawnID = EditorTargetSystem::Get()->GetNextUID();
        for (auto mapElementUID : grouplessMapElements)
        {
            auto it = mapSystem->GetMapElementList().find( mapElementUID );
            if (it != mapSystem->GetMapElementList().end())
            {
                MapSystem::Get()->GetMapElementList().modify(it, MapElementIdentifierModifier( spawnID ) );
            }
        }
        EventServer<EditorBackEvent>::Get().SendEvent( EditorBackEvent( true ) );
    }
}

void EditorSelectSystem::SetUIDUniqueForSelectedActors()
{
    // basically a newly created actor on map has the same uid (ensures to always spawn to the map instantly)
    // to avoid this, have to check if an actor has a uid like this
    // if so it has to be changed for some really unique id to put the specific actor to a group
    static int32_t spawnID = EditorTargetSystem::Get()->GetNextUID();
    std::vector<int32_t> mapElementsToModify;
    auto mapSystem(MapSystem::Get());
    for (auto&& actorColor : mSelectedActors)
    {
        int32_t actorGUID = actorColor.first;
        auto mapElement( mapSystem->GetMapElement( actorGUID ));
        if (mapElement.IsValid() && mapElement->GetIdentifier() == spawnID)
        {
            mapElementsToModify.push_back( mapElement->GetUID() );
        }
    }
    int32_t nextIndex = GetNextUniqueSpawnIndex();
    for (auto mapElementUID : mapElementsToModify)
    {
        auto it = mapSystem->GetMapElementList().find( mapElementUID );
        if (it != mapSystem->GetMapElementList().end())
        {
            mapSystem->GetMapElementList().modify(
                it, MapElementIdentifierModifier( AutoId( "a" + std::to_string( nextIndex++ ) ) ) );
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
            if (idStorage.GetName( mapElement->GetIdentifier(), mapElementUName ))
            {
                if (boost::starts_with( mapElementUName, "a" ))
                {
                    max = std::max( std::stoi( mapElementUName.substr( 1 ) ), max );
                }
            }
        } catch (...) {}
    }
    return ++max;
}

void EditorSelectSystem::SetEditorSelectState( std::string state )
{
    if (state == "add")
    {
        mSelectState = AddToGroup;
    }
    else if (state == "remove")
    {
        mSelectState = RemoveFromGroup;
    }
}

void EditorSelectSystem::OnRemoveFromAllGroups()
{
    auto mapSystem = MapSystem::Get();
    for (auto actorColor : mSelectedActors)
    {
        auto mapElement( mapSystem->GetMapElement( actorColor.first ) );
        if (!mapElement.IsValid())
        {
            continue; // this should not happen all actors should have a spawn actor map element at least
        }
        for (Opt<GroupMapElement> groupMapElement : MapElementListFilter<MapSystem::All>( MapSystem::Get()->GetMapElementList(), GroupMapElement::GetType_static() )) // all groups - most probably one
        {
            GroupMapElement::Targets_t targets = groupMapElement->GetTargets();
            targets.erase( mapElement->GetIdentifier() );
            groupMapElement->SetTargets( targets );
        }



        static int32_t spawnID = EditorTargetSystem::Get()->GetNextUID();
        auto it = mapSystem->GetMapElementList().find( mapElement->GetUID() );
        if (it != mapSystem->GetMapElementList().end())
        {
            MapSystem::Get()->GetMapElementList().modify( it, MapElementIdentifierModifier( spawnID ) );
        }
    }
    EventServer<EditorBackEvent>::Get().SendEvent( EditorBackEvent( true ) );
}

} // namespace map

