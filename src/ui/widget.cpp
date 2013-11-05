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
	if(Child->mDimSet)
		Include(Child->mDimensions);
}

Widget::Widget()
: mZOrder(0)
, mParent(NULL)
, mNext(NULL)
, mPrev(NULL)
, mFirstChild(NULL)
, mLastChild(NULL)
, mFlagged(false)
, mDimSet(false)
, mVisible(false)
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

void Widget::SetDimensions( const glm::vec4& Dim )
{
	mDimSet=true;
	mDimensions=Dim;
	if(mParent)
		mParent->Include(Dim);
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

void Widget::Include( glm::vec4 const& Dim )
{
	if(!mDimSet)
		SetDimensions(Dim);

	if(mDimensions.x>Dim.x)
		mDimensions.x=Dim.x;
	if(mDimensions.y>Dim.y)
		mDimensions.y=Dim.y;
	if(mDimensions.z<Dim.z)
		mDimensions.z=Dim.z;
	if(mDimensions.w<Dim.w)
		mDimensions.w=Dim.w;

	if(mParent)
		mParent->Include(mDimensions);
}

bool Widget::IsInside( const glm::vec2& Pos ) const
{
	return mDimSet&&
			Pos.x>=mDimensions.x&&
			Pos.x<=mDimensions.z&&
			Pos.y>=mDimensions.y&&
			Pos.y<=mDimensions.w;
}

bool Widget::IsVisible() const
{
	return mVisible;
}

void Widget::SetVisible(bool Visible)
{
	mVisible=Visible;
}

