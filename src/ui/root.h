#ifndef INCLUDED_UI_ROOT_H
#define INCLUDED_UI_ROOT_H

class Root : public Widget
{
public:
	Root();
protected:
	virtual void UpdateSelfDimensions(){}

	void InitTestUi();
};

#endif//INCLUDED_UI_ROOT_H
