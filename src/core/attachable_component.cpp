#include "core/attachable_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace ctf {

AttachableComponent::AttachableComponent()
    : mAttachedGUID( -1 )
    , mPositionX( 0.0 )
    , mPositionY( 0.0 )
    , mInheritOrientation( true )
    , mRemoveOnAttachedDeath( false )
{
}

void AttachableComponent::SetAttachedGUID( int32_t attachedGUID )
{
    mAttachedGUID = attachedGUID;
}

int32_t AttachableComponent::GetAttachedGUID()const
{
    return mAttachedGUID;
}

void AttachableComponent::SetPositionX( double positionX )
{
    mPositionX = positionX;
}

double AttachableComponent::GetPositionX()const
{
    return mPositionX;
}

void AttachableComponent::SetPositionY( double positionY )
{
    mPositionY = positionY;
}

double AttachableComponent::GetPositionY()const
{
    return mPositionY;
}

void AttachableComponent::SetInheritOrientation( bool inheritOrientation )
{
    mInheritOrientation = inheritOrientation;
}

bool AttachableComponent::IsInheritOrientation()const
{
    return mInheritOrientation;
}

void AttachableComponent::SetRemoveOnAttachedDeath( bool removeOnAttachedDeath )
{
    mRemoveOnAttachedDeath = removeOnAttachedDeath;
}

bool AttachableComponent::IsRemoveOnAttachedDeath()const
{
    return mRemoveOnAttachedDeath;
}

void AttachableComponentLoader::BindValues()
{
    Bind( "position_x", func_double( &AttachableComponent::SetPositionX ) );
    Bind( "position_y", func_double( &AttachableComponent::SetPositionY ) );
    Bind( "inherit_orientation", func_bool( &AttachableComponent::SetInheritOrientation ) );
    Bind( "remove_on_attached_death", func_bool( &AttachableComponent::SetRemoveOnAttachedDeath ) );
}

AttachableComponentLoader::AttachableComponentLoader()
{
}

} // namespace ctf
REAPING2_CLASS_EXPORT_IMPLEMENT( ctf__AttachableComponent, ctf::AttachableComponent );
