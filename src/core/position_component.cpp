#include "platform/i_platform.h"
#include "core/position_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>
#include "component_loader_factory.h"
#include "i_position_component.h"
#include "actor.h"


PositionComponent::PositionComponent()
    : mX( 0.0 )
    , mY( 0.0 )
    , mOrientation( 0.0 )
{
}

double const& PositionComponent::GetX()const
{
    return mX;
}
double const& PositionComponent::GetY()const
{
    return mY;
}
double const& PositionComponent::GetOrientation()const
{
    return mOrientation;
}
void PositionComponent::SetX( double X )
{
    mX = std::floor( X * PRECISION ) / PRECISION;
}
void PositionComponent::SetY( double Y )
{
    mY = std::floor( Y * PRECISION ) / PRECISION;
}

void PositionComponent::AddX( double X )
{
    mX = mX + X;
}

void PositionComponent::AddY( double Y )
{
    mY = mY + Y;
}

void PositionComponent::SetOrientation( double Orientation )
{
    mOrientation = std::floor( Orientation * PRECISION ) / PRECISION;
}

void PositionComponent::Save( Json::Value& component )
{
    Component::Save( component );
    Json::Value SettersArr( Json::arrayValue );
    Json::Value Setters( Json::objectValue );
    Setters["x"] = Json::Value( GetX() );
    Setters["y"] = Json::Value( GetY() );
    SettersArr.append( Setters );
    component["set"] = SettersArr;
}

std::unique_ptr<PositionComponentLoader> PositionComponentLoader::FromActor( Actor const& actor )
{
    auto positionC(actor.Get<IPositionComponent>());
    return FromPosition( positionC->GetX(), positionC->GetY() );
}

std::unique_ptr<PositionComponentLoader> PositionComponentLoader::FromPosition( double x, double y )
{
    std::unique_ptr<PositionComponentLoader> positionCompLoader( new PositionComponentLoader() );
    positionCompLoader->Bind<double>( &PositionComponent::SetX, x );
    positionCompLoader->Bind<double>( &PositionComponent::SetY, y );
    return positionCompLoader;
}

void PositionComponentLoader::BindValues()
{
    Bind( "x", func_double( &PositionComponent::SetX ) );
    Bind( "y", func_double( &PositionComponent::SetY ) );
    SetterFuncList_t setterfunc = mSetterFuncList;
}

PositionComponentLoader::PositionComponentLoader()
{

}

REAPING2_CLASS_EXPORT_IMPLEMENT( PositionComponent, PositionComponent );
