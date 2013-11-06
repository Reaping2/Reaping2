#include "i_ui.h"

Widget::WidgetIterator & Widget::WidgetIterator::operator++()
{
	if(mThis->mFirstChild)
	{
		mThis=mThis->mFirstChild;
		++mLevel;
	}
	else if(mThis->mNext)
	{
		mThis=mThis->mNext;
	}
	else
	{
		Widget const* Next=mThis;
		while(Next&&!Next->mNext&&mLevel)
		{
			Next=Next->mParent;
			--mLevel;
		}
		mThis=Next->mNext;
	}
	return *this;
}

void Widget::AddChild(Widget* Child)
{
	assert(!Child->mPrev&&!Child->mParent&&!Child->mNext);

	Widget* WhatWillBeNext=mFirstChild;
	while(WhatWillBeNext&&WhatWillBeNext->mZOrder<Child->mZOrder)WhatWillBeNext=WhatWillBeNext->mNext;

	if(WhatWillBeNext)
	{
		Child->mPrev=WhatWillBeNext->mPrev;
		if(WhatWillBeNext->mPrev)
			WhatWillBeNext->mPrev->mNext=Child;
		Child->mNext=WhatWillBeNext;
		WhatWillBeNext->mPrev=Child;
		if(WhatWillBeNext==mFirstChild)
			mFirstChild=Child;
	}
	else
	{
		if(!mFirstChild)
			mFirstChild=Child;
		mLastChild=Child;
	}
	Child->mParent=this;
	Child->UpdateDimensions();
	Child->UpdateChildrenDimensions();
}

Widget::Widget(glm::vec4 const& RelativeDimensions)
: mZOrder(0)
, mParent(NULL)
, mNext(NULL)
, mPrev(NULL)
, mFirstChild(NULL)
, mLastChild(NULL)
, mFlagged(false)
, mDimSet(false)
, mVisible(false)
, mRelativeDimensions(RelativeDimensions)
{

}

Widget::~Widget()
{
	Widget* What=mFirstChild;
	while(What)
	{
		Widget* Next=What->mNext;
		delete What;
		What=Next;
	}
	if(mNext)
	{
		mNext->mPrev=mPrev;
	}
	if(mPrev)
	{
		mPrev->mNext=mNext;
	}
	if(mParent)
	{
		if(mParent->mFirstChild==this)
			mParent->mFirstChild=mNext;
		if(mParent->mLastChild==this)
			mParent->mLastChild=mPrev;
	}
}

glm::vec4 const& Widget::GetDimensions() const
{
	return mDimensions;
}

Widget::const_iterator Widget::begin() const
{
	return WidgetIterator(this);
}

Widget::const_iterator Widget::end() const
{
	return WidgetIterator();
}

void Widget::UpdateDimensions()
{
	UpdateSelfDimensions();
	UpdateChildrenDimensions();
}

void Widget::UpdateSelfDimensions()
{
	if(!mParent||!mParent->mDimSet) return;
	mDimSet=true;
	glm::vec4 const& ParentDim=mParent->GetDimensions();
	double const X=ParentDim.x;
	double const Y=ParentDim.y;
	double const Width=ParentDim.z;
	double const Height=ParentDim.w;
	mDimensions=glm::vec4(	X+mRelativeDimensions.x*Width,
							Y+mRelativeDimensions.y*Height,
							mRelativeDimensions.z*Width,
							mRelativeDimensions.w*Height);
}

void Widget::UpdateChildrenDimensions()
{
	if(!mParent||!mParent->mDimSet) return;
	for(Widget* i=mFirstChild;i;i=i->mNext)
		i->UpdateDimensions();
}

Widget* Widget::GetHit( const glm::vec2& Pos )
{
	if(!IsInside(Pos))return NULL;
	for(Widget* i=mLastChild;i;i=i->mPrev)
	{
		Widget* Wdg=i->GetHit(Pos);
		if(Wdg) return Wdg;
	}
	return mVisible?this:NULL;
}

bool Widget::IsInside( const glm::vec2& Pos ) const
{
	return mDimSet&&
			Pos.x>=mDimensions.x&&
			Pos.x<=mDimensions.x+mDimensions.z&&
			Pos.y>=mDimensions.y&&
			Pos.y<=mDimensions.y+mDimensions.w;
}

bool Widget::IsVisible() const
{
	return mVisible;
}

void Widget::SetVisible(bool Visible)
{
	mVisible=Visible;
}

void Widget::SetRelativeDimensions( glm::vec4 const& Dim )
{
	mRelativeDimensions=Dim;
	UpdateDimensions();
}

bool Widget::AreDimensionsSet() const
{
	return mDimSet;
}

Widget* Widget::GetNext() const
{
	return mNext;
}

