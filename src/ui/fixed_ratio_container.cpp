#include "i_ui.h"

FixedRatioContainer::FixedRatioContainer(double XtoYRatio)
: mTargetRatio(XtoYRatio)
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
	if(Mult>=1.)
		mContainedDimensions=glm::vec4(mDimensions.x/Mult,mDimensions.y,mDimensions.z/Mult,mDimensions.w);
	else
		mContainedDimensions=glm::vec4(mDimensions.x,mDimensions.y*Mult,mDimensions.z,mDimensions.w*Mult);
}
