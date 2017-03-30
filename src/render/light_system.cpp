#include "platform/i_platform.h"
#include "light_system.h"
#include "core/i_light_component.h"
#include "core/i_position_component.h"
#include "engine/system_factory.h"
#include "core/program_state.h"
#include "core/actor_factory.h"

namespace render {

LightSystem::LightSystem()
    : mScene( Scene::Get() )
{
}


void LightSystem::Init()
{
    mOnMapLoad = EventServer<core::MapLoadEvent>::Get().Subscribe( std::bind( &LightSystem::OnMapLoad, this, std::placeholders::_1 ) );
    mOnActorEvent = EventServer<ActorEvent>::Get().Subscribe( std::bind( &LightSystem::OnActorEvent, this, std::placeholders::_1 ) );

    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<ILightComponent>().IsValid()
            && actor.Get<IPositionComponent>().IsValid(); } );
}


void LightSystem::Update(double DeltaTime)
{
    static core::ProgramState& ps( core::ProgramState::Get() );
    auto player = mScene.GetActor( ps.mControlledActorGUID );
    if( !player.IsValid() )
    {
        return;
    }
    auto playerPosC = player->Get<IPositionComponent>();
    if( !playerPosC.IsValid() )
    {
        return;
    }
    std::vector<Actor*> activeLights;
    for( auto actor : mScene.GetActorsFromMap( GetType_static() ) )
    {
        auto posC=actor->Get<IPositionComponent>();
        auto lightC=actor->Get<ILightComponent>();
        glm::vec2 dist( posC->GetX() - playerPosC->GetX(),
               posC->GetY() - playerPosC->GetY() );
        auto distSq = glm::dot( dist, dist );
        auto r = lightC->GetRadius() + 2000.0;
        if( distSq <= r*r*2 )
        {
            activeLights.push_back( actor );
        }
    }

    if (!ps.mIsClient)
    {
        mActiveDummyObjects.resize( std::max( mActiveDummyObjects.size(), activeLights.size() ) );
        auto lightIt = activeLights.begin();
        for (auto*& dummy : mActiveDummyObjects)
        {
            if (activeLights.end() == lightIt)
            {
                // drop dummmy
                if (dummy != nullptr)
                {
                    mScene.RemoveActor( dummy->GetGUID() );
                }
                continue;
            }
            if (dummy == nullptr&& !ps.mIsClient)
            {
                static ActorFactory& af( ActorFactory::Get() );
                auto newDummy( af( AutoId( "light_active_dummy" ) ) );
                dummy = newDummy.get();
                mScene.AddActor( newDummy.release() );
            }
            auto light = *lightIt;
            // TODO: attachable_component would solve this positioning i think.
            dummy->Get<IPositionComponent>()->SetX( light->Get<IPositionComponent>()->GetX() );
            dummy->Get<IPositionComponent>()->SetY( light->Get<IPositionComponent>()->GetY() );
            ++lightIt;
        }
        mActiveDummyObjects.resize( activeLights.size() );
    }
    std::swap( activeLights, mActiveLights );
}

void LightSystem::OnMapLoad( core::MapLoadEvent const& Evt )
{
    PathVect_t Paths;
    Filesys& FSys = Filesys::Get();
    FSys.GetFileNames( Paths, Evt.mMapName );
    for( PathVect_t::const_iterator i = Paths.begin(), e = Paths.end(); i != e; ++i )
    {
        boost::filesystem::path const& Path = *i;
        if( Path.filename().string() != "description.json" )
        {
            continue;
        }
        AutoFile JsonFile = FSys.Open( *i );
        if( !JsonFile.get() )
        {
            continue;
        }
        JsonReader Reader( *JsonFile );
        if( !Reader.IsValid() )
        {
            continue;
        }
        Json::Value Root = Reader.GetRoot();
        if( !Json::GetDouble( Root["max_shadow"], mMaxShadow ) )
        {
            mMaxShadow = 0.6;
        }
        Json::Value vec = Root["shadow_vector"];
        mShadowVec = glm::vec2( 25, -25 );
        if( vec.isArray() && vec.size() >= 2 )
        {
            double x,y;
            if( Json::GetDouble( vec[0], x ) &&
                Json::GetDouble( vec[1], y ) )
            {
                mShadowVec = glm::vec2( x, y );
            }
        }
        mAmbientLight = glm::vec4( 1,1,1,0.6 );
        vec = Root["ambient_light"];
        if( vec.isArray() && vec.size() >= 2 )
        {
            double x,y,z,w;
            if( Json::GetDouble( vec[0], x ) &&
                Json::GetDouble( vec[1], y ) &&
                Json::GetDouble( vec[2], z ) &&
                Json::GetDouble( vec[3], w ) )
            {
                mAmbientLight = glm::vec4(x,y,z,w);
            }
        }
        break;
    }
    LL() << mMaxShadow << " (" << mShadowVec.x
        << " " << mShadowVec.y
        << ") ("
        << mAmbientLight.x << " "
        << mAmbientLight.y << " "
        << mAmbientLight.z << " "
        << mAmbientLight.w
        << ")";
}

std::vector<Actor*> const& LightSystem::GetActiveLights() const
{
    return mActiveLights;
}

glm::vec2 LightSystem::GetShadowVector() const
{
    return mShadowVec;
}

glm::vec4 LightSystem::GetAmbientLight() const
{
    return mAmbientLight;
}

double LightSystem::GetMaxShadow() const
{
    return mMaxShadow;
}

void LightSystem::OnActorEvent( ActorEvent const& Evt )
{
    if (Evt.mState == ActorEvent::Removed)
    {
        mActiveLights.erase( std::remove_if( mActiveLights.begin(), mActiveLights.end(), 
            [&Evt]( auto * actor ) { return actor->GetGUID() == Evt.mActor->GetGUID(); } ), mActiveLights.end() );
        mActiveDummyObjects.erase( std::remove_if( mActiveDummyObjects.begin(), mActiveDummyObjects.end(), 
            [&Evt]( auto * actor ) { return actor->GetGUID() == Evt.mActor->GetGUID(); } ), mActiveDummyObjects.end() );
    }
}

REGISTER_SYSTEM( LightSystem );

} // namespace render

