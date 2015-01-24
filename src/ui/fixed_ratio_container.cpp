#include "i_ui.h"
#include "engine/engine.h"
#include "main/window.h"

FixedRatioContainer::FixedRatioContainer( int32_t Id )
    : Widget( Id )
    , mTargetRatio( 1.0 )
    , mHorizontalAlignment( HorizontalAlignment::Left )
    , mVerticalAlignment( VerticalAlignment::Bottom )
{
    mWindowResizeId = EventServer<WindowResizeEvent>::Get().Subscribe( boost::bind( &FixedRatioContainer::OnWindowResizeEvent, this, _1 ) );
    int w, h;
    engine::Engine::Get().GetSystem<engine::WindowSystem>()->GetWindowSize( w, h );
    Resize( w, h );
}

void FixedRatioContainer::Resize( int Width, int Height )
{
    mWindowRatio = ( double )Width / Height;
    UpdateDimensions();
}

void FixedRatioContainer::OnWindowResizeEvent( const WindowResizeEvent& Event )
{
    Resize( Event.Width, Event.Height );
}

glm::vec4 const& FixedRatioContainer::GetDimensions() const
{
    return mContainedDimensions;
}

void FixedRatioContainer::UpdateSelfDimensions()
{
    Widget::UpdateSelfDimensions();
    if( !mDimSet )
    {
        return;
    }
    const double Mult = mWindowRatio / mTargetRatio;
    glm::vec4 const& ParentDim = mParent->GetDimensions();
    if( Mult >= 1. )
    {
        mContainedDimensions = glm::vec4( mDimensions.x, mDimensions.y, mDimensions.z / Mult, mDimensions.w );
    }
    else
    {
        mContainedDimensions = glm::vec4( mDimensions.x, mDimensions.y, mDimensions.z, mDimensions.w * Mult );
    }
    const float DW = mDimensions.z - mContainedDimensions.z;
    const float DH = mDimensions.w - mContainedDimensions.w;
    switch( mHorizontalAlignment )
    {
    case HorizontalAlignment::Left:
        break;
    case HorizontalAlignment::Right:
        mContainedDimensions.x += DW;
        break;
    case HorizontalAlignment::Center:
        mContainedDimensions.x += DW / 2.f;
        break;
    default:
        break;
    }
    switch( mVerticalAlignment )
    {
    case VerticalAlignment::Bottom:
        break;
    case VerticalAlignment::Top:
        mContainedDimensions.y += DH;
        break;
    case VerticalAlignment::Center:
        mContainedDimensions.y += DH / 2.f;
        break;
    default:
        break;
    }
}

void FixedRatioContainer::Init( Json::Value& Descriptor )
{
    BaseClass::Init( Descriptor );
    double d;
    mTargetRatio = Json::GetDouble( Descriptor["ratio"], d ) ? d : 1.;
    assert( mTargetRatio > 0.00001 );
    std::string s;
    if( Json::GetStr( Descriptor["align_h"], s ) )
    {
        if( s == "right" )
        {
            mHorizontalAlignment = HorizontalAlignment::Right;
        }
        else if( s == "center" )
        {
            mHorizontalAlignment = HorizontalAlignment::Center;
        }
        else
        {
            mHorizontalAlignment = HorizontalAlignment::Left;
        }
    }
    else
    {
        mHorizontalAlignment = HorizontalAlignment::Left;
    }
    if( Json::GetStr( Descriptor["align_v"], s ) )
    {
        if( s == "top" )
        {
            mVerticalAlignment = VerticalAlignment::Top;
        }
        else if( s == "center" )
        {
            mVerticalAlignment = VerticalAlignment::Center;
        }
        else
        {
            mVerticalAlignment = VerticalAlignment::Bottom;
        }
    }
    else
    {
        mVerticalAlignment = VerticalAlignment::Bottom;
    }
}
