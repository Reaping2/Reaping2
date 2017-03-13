#include "engine/items/lucky_rocket_weapon_subsystem.h"
#include "engine/item_properties_changed_event.h"
#include <boost/assign/list_of.hpp>

namespace engine {

LuckyRocketWeaponSubSystem::LuckyRocketWeaponSubSystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
    , mWeaponItemSubSystem( WeaponItemSubSystem::Get() )
    , mActorFactory( ActorFactory::Get() )
{
}

void LuckyRocketWeaponSubSystem::Init()
{
    SubSystemHolder::Init();
}

void LuckyRocketWeaponSubSystem::Update( Actor& actor, double DeltaTime )
{
    static std::vector<int32_t> const weapons = boost::assign::list_of
            ( ( int32_t )AutoId( "rocket_launcher" ) )
            ( ( int32_t )AutoId( "gauss_gun" ) )
            ( ( int32_t )AutoId( "gatling_gun" ) )
            ( ( int32_t )AutoId( "pistol" ) )
            ( ( int32_t )AutoId( "plasma_gun" ) )
            ( ( int32_t )AutoId( "ion_gun" ) )
            ( ( int32_t )AutoId( "shotgun" ) )
            ( ( int32_t )AutoId( "lucky_rocket" ) )
            ;
    static std::vector<int32_t> const altweapons = boost::assign::list_of
            ( ( int32_t )AutoId( "pistol" ) )
            ( ( int32_t )AutoId( "plasma_gun" ) )
            ( ( int32_t )AutoId( "ion_gun" ) )
            ( ( int32_t )AutoId( "shotgun" ) )
            ( ( int32_t )AutoId( "lucky_rocket" ) )
            ;

    Opt<Weapon> weapon = actor.Get<IInventoryComponent>()->GetSelectedItem( ItemType::Weapon );
    if( !weapon->IsShooting() && !weapon->IsShootingAlt() )
    {
        // not firing, nothing to do
        return;
    }
    if( weapon->GetCooldown() > 0 || weapon->GetReloadTime() > 0 )
    {
        return;
    }

    bool alt = RandomGenerator::global()() % 2;
    int32_t selectedWeapon = alt ? altweapons.at( RandomGenerator::global()() % altweapons.size() ) : weapons.at( rand() % weapons.size() );
    if( selectedWeapon != weapon->GetId() )
    {
        Opt<IInventoryComponent> inv = actor.Get<IInventoryComponent>();
        inv->AddItem( selectedWeapon );
        inv->SetSelectedItem( ItemType::Weapon, selectedWeapon );
        Opt<Weapon> rolledWeapon = actor.Get<IInventoryComponent>()->GetSelectedItem( ItemType::Weapon );
        rolledWeapon->GetScatter().mCurrent = weapon->GetScatter().mCurrent;
        if( alt )
        {
            rolledWeapon->SetShootAlt( true );
        }
        else
        {
            rolledWeapon->SetShoot( true );
        }
        mWeaponItemSubSystem->Update( actor, DeltaTime );
        weapon->GetScatter().mCurrent = rolledWeapon->GetScatter().mCurrent;
        weapon->SetCooldown( std::min( 1.0, rolledWeapon->GetCooldown() ) );
        int32_t bullets = RandomGenerator::global()() % int( 1 + weapon->GetBulletsMax() );
        if( bullets == 0 )
        {
            weapon->SetReloadTimeMax( rolledWeapon->GetReloadTimeMax() );
            weapon->SetReloadTime( rolledWeapon->GetReloadTimeMax() );
        }
        weapon->SetBullets( bullets == 0 ? 0 : ( bullets * 10 + RandomGenerator::global()() % 10 ) );

        inv->DropItem( selectedWeapon );
        inv->SetSelectedItem( ItemType::Weapon, weapon->GetId() );
    }
    else
    {
        int32_t id = alt ? weapon->GetShotAltId() : weapon->GetShotId();
        std::auto_ptr<Actor> rocket = mActorFactory( id );
        WeaponItemSubSystem::Projectiles_t projectiles;
        projectiles.push_back( rocket.release() );
        mWeaponItemSubSystem->AddProjectiles( actor, projectiles, weapon->GetScatter(), alt );
    }
    EventServer<ItemPropertiesChangedEvent>::Get().SendEvent( ItemPropertiesChangedEvent( *weapon ) );
}

} // namespace engine
