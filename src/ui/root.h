#ifndef INCLUDED_UI_ROOT_H
#define INCLUDED_UI_ROOT_H

class Root : public Widget
{
public:
	Root();
protected:
	virtual void UpdateSelfDimensions(){}

	// fun
	void OnTimer();
	void InitTestUi();
	Widget* mTexWdg;
	Registration mTimerId;
};

#endif//INCLUDED_UI_ROOT_H
