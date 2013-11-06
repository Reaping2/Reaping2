#include "i_ui.h"

FixedRatioContainer::FixedRatioContainer(double XtoYRatio,HorizontalAlignment::Type HoriAlignment/*=HorizontalAlignment::Left*/, VerticalAlignment::Type VertAlignment/*=VerticalAlignment::Bottom*/)
: mTargetRatio(XtoYRatio)
, mHorizontalAlignment(HoriAlignment)
, mVerticalAlignment(VertAlignment)
{
	assert(XtoYRatio>0.00001);
	mWindowResizeId=EventServer<WindowResizeEvent>::Get().Subscribe(boost::bind(&FixedRatioContainer::OnWindowResizeEvent,this,_1));
	int w,h;
	Window::Get().GetWindowSize(w,h);
	Resize(w,h);
}

void FixedRatioContainer::Resize(int Width, int Height)
{
	mWindowRatio=(double)Width/Height;
	UpdateDimensions();
}

void FixedRatioContainer::OnWindowResizeEvent(const WindowResizeEvent& Event)
{
	Resize(Event.Width,Event.Height);
}

glm::vec4 const& FixedRatioContainer::GetDimensions() const
{
	return mContainedDimensions;
}

void FixedRatioContainer::UpdateSelfDimensions()
{
	Widget::UpdateSelfDimensions();
	if(!mDimSet)return;
	const double Mult=mWindowRatio/mTargetRatio;
	glm::vec4 const& ParentDim=mParent->GetDimensions();
	if(Mult>=1.)
		mContainedDimensions=glm::vec4(mDimensions.x,mDimensions.y,mDimensions.z/Mult,mDimensions.w);
	else
		mContainedDimensions=glm::vec4(mDimensions.x,mDimensions.y,mDimensions.z,mDimensions.w*Mult);
	const float DW=mDimensions.z-mContainedDimensions.z;
	const float DH=mDimensions.w-mContainedDimensions.w;
	switch(mHorizontalAlignment)
	{
	case HorizontalAlignment::Left:break;
	case HorizontalAlignment::Right:mContainedDimensions.x+=DW;break;
	case HorizontalAlignment::Center:mContainedDimensions.x+=DW/2.f;break;
	default:break;
	}
	switch(mVerticalAlignment)
	{
	case VerticalAlignment::Bottom:break;
	case VerticalAlignment::Top:mContainedDimensions.y+=DH;break;
	case VerticalAlignment::Center:mContainedDimensions.y+=DH/2.f;break;
	default:break;
	}
}
