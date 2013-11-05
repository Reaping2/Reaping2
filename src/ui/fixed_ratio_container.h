#ifndef INCLUDED_UI_FIXED_RATIO_CONTAINER_H
#define INCLUDED_UI_FIXED_RATIO_CONTAINER_H

#include "main/window.h"

class FixedRatioContainer : public Widget
{
public:
	FixedRatioContainer(double XtoYRatio);
protected:
	// todo: content alignment: top/bottom/center/left/right/center
	double mTargetRatio;
	double mWindowRatio;
	glm::vec4 mContainedDimensions;
	Registration mWindowResizeId;
	void OnWindowResizeEvent(const WindowResizeEvent& Event);
	void Resize(int Width, int Height);
	virtual void UpdateSelfDimensions();
	virtual glm::vec4 const& GetDimensions()const;
};

#endif//INCLUDED_UI_FIXED_RATIO_CONTAINER_H
