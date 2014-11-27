#include "health_component.h"
#include "i_position_component.h"
#include "platform/auto_id.h"
#include "platform/event.h"
#include "core/damage_taken_event.h"
#include "core/i_collision_component.h"
#include "renderable_component.h"
#include "i_core.h"
#include "core/opt.h"

using platform::AutoId;
using platform::EventServer;

HealthComponent::HealthComponent()
    : mHP( 1 )
    , mAlive( true )
    , mTimeOfDeath( 0 )
    , mNeedDelete( false )
{
}

void HealthComponent::Update( double Seconds )
{
    if ( IsAlive() )
    {
        return;
    }
    BOOST_ASSERT( mActor );
    mAlive = false;
    mActor->Get<ICollisionComponent>()->SetCollisionClass( CollisionClass::No_Collision );
    mActor->AddAction( AutoId( "death" ) );
    if ( mTimeOfDeath <= 0.0 )
    {
        Scene::Get().ModifyActor(mActor,RenderableComponentModifier(IRenderableComponent::Corpses,(int)glfwGetTime()));
        mTimeOfDeath=glfwGetTime();
    }
}

int32_t const& HealthComponent::GetHP() const
{
    return mHP;
}

void HealthComponent::SetHP( int32_t Hp )
{
    if (IsAlive())
    {
        if (mHP>Hp)
        {
            Opt<IPositionComponent> positionC = mActor->Get<IPositionComponent>();
            if(positionC.IsValid())
            {
                EventServer<core::DamageTakenEvent>::Get().SendEvent( core::DamageTakenEvent( positionC->GetX(), positionC->GetY() ) );
            }
        }
        mHP = Hp;
        mAlive = mHP > 0;
    }
}

bool HealthComponent::IsAlive() const
{
    return mAlive;
}

double HealthComponent::GetTimeOfDeath() const
{
    return mTimeOfDeath;
}

bool HealthComponent::NeedDelete() const
{
    return mNeedDelete;
}

void HealthComponentLoader::LoadValues(Json::Value& setters)
{
    int32_t hp;
    if( Json::GetInt( setters["hp"], hp))
    {
        Bind<int32_t>(&HealthComponent::SetHP, hp);
    }
}

HealthComponentLoader::HealthComponentLoader()
{
}
