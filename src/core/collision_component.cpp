#include "collision_component.h"
#include "i_position_component.h"
#include "platform/auto_id.h"
#include "platform/event.h"
#include "core/scene.h"
#include "platform/i_platform.h"

using platform::AutoId;
using platform::EventServer;


CollisionComponent::CollisionComponent()
    : mCollisionClassType( CollisionClass::No_Collision )
    , mRadius( 0.0 )
{

}

void CollisionComponent::Collide(Actor& Other)
{
}

void CollisionComponent::ClipScene()
{
    glm::vec4 AllowedDimensions = Scene::Get().GetDimensions();
    float Radius = ( float )GetRadius();
    AllowedDimensions.x += Radius;
    AllowedDimensions.y += Radius;
    AllowedDimensions.z -= Radius;
    AllowedDimensions.w -= Radius;
    Opt<IPositionComponent> positionC = mActor->Get<IPositionComponent>();
    if( positionC->GetX() < AllowedDimensions.x )
    {
        positionC->SetX( AllowedDimensions.x );
    }
    else if( positionC->GetX() > AllowedDimensions.z )
    {
        positionC->SetX( AllowedDimensions.z );
    }
    if( positionC->GetY() < AllowedDimensions.y )
    {
        positionC->SetY( AllowedDimensions.y );
    }
    else if( positionC->GetY() > AllowedDimensions.w )
    {
        positionC->SetY( AllowedDimensions.w );
    }
}

CollisionClass::Type CollisionComponent::GetCollisionClass() const
{
    return mCollisionClassType;
}

void CollisionComponent::SetCollisionClass(CollisionClass::Type CollisionClassType)
{
    mCollisionClassType=CollisionClassType;
}

double CollisionComponent::GetRadius() const
{
    return mRadius;
}

void CollisionComponent::SetRadius(double Radius)
{
    mRadius=Radius;
}

void CollisionComponentLoader::BindValues()
{
    Bind("radius",func_double(&CollisionComponent::SetRadius));
    std::string istr;
    if( Json::GetStr( (*mSetters)["class"], istr))
    {
        Bind<CollisionClass::Type>(&CollisionComponent::SetCollisionClass,mCollisionClass(AutoId(istr)));
    }

}

CollisionComponentLoader::CollisionComponentLoader()
    : mCollisionClass(CollisionClass::Get())
{
}
