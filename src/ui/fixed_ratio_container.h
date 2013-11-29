#ifndef INCLUDED_UI_FIXED_RATIO_CONTAINER_H
#define INCLUDED_UI_FIXED_RATIO_CONTAINER_H

#include "main/window.h"

struct HorizontalAlignment
{
	enum Type
	{
		Left,
		Right,
		Center,
	};
};

struct VerticalAlignment
{
	enum Type
	{
		Bottom,
		Top,
		Center,
	};
};

class FixedRatioContainer : public Widget
{
	typedef Widget BaseClass;
public:
	FixedRatioContainer(int32_t Id);
	virtual void Init(Json::Value& Descriptor);
protected:
	HorizontalAlignment::Type mHorizontalAlignment;
	VerticalAlignment::Type mVerticalAlignment;
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
