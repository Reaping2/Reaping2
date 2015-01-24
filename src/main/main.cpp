#include "window.h"
#include "render/i_render.h"
#include "platform/i_platform.h"
#include "input/i_input.h"
#include "audio/i_audio.h"
#include "core/scene.h"

#include <boost/timer.hpp>
#include "engine/engine.h"
#include "engine/collision_system.h"
#include "engine/controllers/controller_system.h"
#include "core/opt.h"
#include "engine/items/inventory_system.h"
#include "core/item.h"
#include "engine/items/weapon_item_sub_system.h"
#include "platform/event.h"
using engine::Engine;
namespace {
class Timer_t
{
    boost::timer mMeasurer;
    double mPrevMeasurement;
public:
    void Log( std::string const& Str = std::string() )
    {
        double Now = mMeasurer.elapsed();
        L1( "Timer - %s: %f %f\n", Str.c_str(), Now, Now - mPrevMeasurement );
        mPrevMeasurement = Now;
    }
    Timer_t(): mMeasurer(), mPrevMeasurement( mMeasurer.elapsed() )
    {
        Log( "timer init" );
    }
} PerfTimer;

class FrameCounter
{
    uint32_t mFrames;
    double mStart;
    double mPrev;
public:
    FrameCounter() : mFrames( 0 ), mStart( glfwGetTime() ), mPrev( mStart ) {}
    void Inc()
    {
        ++mFrames;
        double const Now = glfwGetTime();
        double const Diff = Now - mPrev;
        if( Diff >= 2.0 )
        {
            L1( "FPS: %f\n", mFrames / Diff );
            mPrev = Now;
            mFrames = 0;
        }
    }
};
}

static bool IsMainRunning;

void OnPhaseChangedEvent( PhaseChangedEvent const& Evt )
{
    if( Evt.CurrentPhase == ProgramPhase::InitiateShutdown )
    {
        IsMainRunning=false;
    }
}

int main()
{
    IsMainRunning=true;
    EventServer<PhaseChangedEvent>& PhaseChangeEventServer( EventServer<PhaseChangedEvent>::Get() );
    AutoReg PhaseChangeId(PhaseChangeEventServer.Subscribe( &OnPhaseChangedEvent ));

    Engine& Eng = Engine::Get();
    Eng.AddSystem(AutoId("window_system"));

    if( !Eng.GetSystem<engine::WindowSystem>()->Create( 640, 480, "Reaping2" ) )
    {
        PhaseChangeEventServer.SendEvent( PhaseChangedEvent( ProgramPhase::InitiateShutdown ) );
    }


    RootModel::Get();
    PhaseChangeEventServer.SendEvent( PhaseChangedEvent( ProgramPhase::Startup ) );
    PerfTimer.Log( "wnd" );
    TimerServer& Timers( TimerServer::Get() );
    Filesys::Get().Mount( std::auto_ptr<Package>( new Package( AutoFile( new OsFile( "data.pkg" ) ) ) ) );
    AudioEffectPlayer::Get();
    AudioPlayer::Get();
    //AudioPlayer::Get().Play( "sounds/Zap_Beat.ogg", AudioFile::Music );
    PerfTimer.Log( "input" );
    Renderer& Rend = Renderer::Get();
    DamageDecals::Get();
    PerfTimer.Log( "renderer" );
    Scene& Scen = Scene::Get();
    PerfTimer.Log( "scene" );

    Eng.AddSystem(AutoId("keyboard_system"));
    Eng.AddSystem(AutoId("mouse_system"));
    Eng.AddSystem(AutoId("collision_system"));
    Opt<engine::CollisionSystem> collisionSS(Eng.GetSystem<engine::CollisionSystem>());
    collisionSS->AddSubSystem(AutoId("pickup_collision_component"),AutoId("pickup_collision_sub_system"));
    collisionSS->AddSubSystem(AutoId("wall_collision_component"),AutoId("wall_collision_sub_system"));
    collisionSS->AddSubSystem(AutoId("shot_collision_component"),AutoId("shot_collision_sub_system"));

    Eng.AddSystem(AutoId("controller_system"));
    Opt<engine::ControllerSystem> controllserSystem(Eng.GetSystem<engine::ControllerSystem>());
    controllserSystem->AddSubSystem(AutoId("player_controller_component"), AutoId("player_controller_sub_system"));
    controllserSystem->AddSubSystem(AutoId("random_controller_component"), AutoId("random_controller_sub_system"));
    controllserSystem->AddSubSystem(AutoId("target_player_controller_component"), AutoId("target_player_controller_sub_system"));

    Eng.AddSystem(AutoId("inventory_system"));
    Opt<engine::InventorySystem> inventorySystem(Eng.GetSystem<engine::InventorySystem>());
    inventorySystem->AddSubSystem(Item::Normal,AutoId("normal_item_sub_system"));
    inventorySystem->AddSubSystem(Item::Weapon,AutoId("weapon_item_sub_system"));
    Opt<engine::WeaponItemSubSystem> weaponitemSS=engine::WeaponItemSubSystem::Get();
    weaponitemSS->AddSubSystem(AutoId("plasma_gun"),AutoId("plasma_gun_weapon_sub_system"));
    weaponitemSS->AddSubSystem(AutoId("pistol"),AutoId("pistol_weapon_sub_system"));

    Eng.AddSystem(AutoId("fade_out_system"));
    Eng.AddSystem(AutoId("drop_on_death_system"));
    Eng.AddSystem(AutoId("health_system"));
    Eng.AddSystem(AutoId("remove_on_death_system"));
    Eng.AddSystem(AutoId("move_system"));

    Eng.Init();
    Eng.SetEnabled<engine::CollisionSystem>(true); //just for testing

    static const double MaxFrameRate = 100.;
    static const double MinFrameTime = 1. / MaxFrameRate;
    double Prevtime, Curtime;
    Prevtime = Curtime = glfwGetTime();
    FrameCounter Counter;
    PhaseChangeEventServer.SendEvent( PhaseChangedEvent( ProgramPhase::Running ) );
    EventServer<CycleEvent>& CycleEventServer( EventServer<CycleEvent>::Get() );
    while( IsMainRunning )
    {
        Curtime = glfwGetTime();
        double Dt = Curtime - Prevtime;
        if( Dt < MinFrameTime )
        {
            const double SleepTime = MinFrameTime - Dt;
            boost::this_thread::sleep( boost::posix_time::milliseconds( boost::int64_t( SleepTime * 1000. ) ) );
            Dt = MinFrameTime;
            Curtime = glfwGetTime();
        }
        Timers.Update( Dt );
        Eng.Update( Dt );
        if (!IsMainRunning)
        {
            break; //TODO: temporary need to finish all system extractions
        }
        Scen.Update( Dt );
        CycleEventServer.SendEvent( CycleEvent( Curtime ) );
        if( !Rend.Render() )
        {
            break;
        }
        Prevtime = Curtime;
        Counter.Inc();
    }
    PhaseChangeEventServer.SendEvent( PhaseChangedEvent( ProgramPhase::CloseSignal ) );
    PhaseChangeEventServer.SendEvent( PhaseChangedEvent( ProgramPhase::Shutdown ) );
    return 0;
}