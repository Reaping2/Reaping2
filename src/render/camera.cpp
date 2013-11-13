#include "i_render.h"

Camera::Camera()
: mScene(Scene::Get())
, mAllowedDistance(0.5,0.5)
{
	mPlayerMoveId=EventServer<PlayerMoveEvent>::Get().Subscribe(boost::bind(&Camera::OnPlayerMoveEvent,this,_1));
}

void Camera::UpdateMatrices()
{
	mView=glm::translate(-mCenter.x,-mCenter.y,0.f);
	mInverseView=glm::inverse(mView);
}

void Camera::OnPlayerMoveEvent( const PlayerMoveEvent& Event )
{
	if(mCenter.x<Event.X-mAllowedDistance.x)
		mCenter.x=Event.X-mAllowedDistance.x;
	else if(mCenter.x>Event.X+mAllowedDistance.x)
		mCenter.x=Event.X+mAllowedDistance.x;
	if(mCenter.y<Event.Y-mAllowedDistance.y)
		mCenter.y=Event.Y-mAllowedDistance.y;
	else if(mCenter.y>Event.Y+mAllowedDistance.y)
		mCenter.y=Event.Y+mAllowedDistance.y;
	UpdateMatrices();
}

glm::mat4 const& Camera::GetView()const
{
	return mView;
}

glm::mat4 const& Camera::GetInverseView() const
{
	return mInverseView;
}

