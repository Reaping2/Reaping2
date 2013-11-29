#include "i_render.h"

Camera::Camera()
: mScene(Scene::Get())
, mAllowedDistance(0.5,0.5)
{
}

void Camera::UpdateMatrices()
{
	mView=glm::translate(-mCenter.x,-mCenter.y,0.f);
	mInverseView=glm::inverse(mView);
}

void Camera::Update()
{
	double px=RootModel::Get()["player"]["x"];
	double py=RootModel::Get()["player"]["y"];
	if(mCenter.x<px-mAllowedDistance.x)
		mCenter.x=(float)px-mAllowedDistance.x;
	else if(mCenter.x>px+mAllowedDistance.x)
		mCenter.x=(float)px+mAllowedDistance.x;
	if(mCenter.y<py-mAllowedDistance.y)
		mCenter.y=(float)py-mAllowedDistance.y;
	else if(mCenter.y>py+mAllowedDistance.y)
		mCenter.y=(float)py+mAllowedDistance.y;
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

