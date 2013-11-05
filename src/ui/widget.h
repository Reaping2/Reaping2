#ifndef INCLUDED_UI_WIDGET_H
#define INCLUDED_UI_WIDGET_H

class Widget
{
protected:
	class WidgetIterator
	{
	private:
		Widget const* mThis;
		int32_t mLevel;
	public:
		WidgetIterator(Widget const* This=NULL):mThis(This),mLevel(0){}
		Widget const& operator*(){return *mThis;}
		Widget const* operator->(){return mThis;}
		bool operator==(WidgetIterator const& Other)const{return mThis==Other.mThis;}
		bool operator!=(WidgetIterator const& Other)const{return !operator==(Other);}
		WidgetIterator &operator++();
	};
	friend class WidgetIterator;
public:
	typedef WidgetIterator const_iterator;
	const_iterator begin()const;
	const_iterator end()const;
	Widget();
	virtual ~Widget();
	void AddChild(Widget* Child);
	glm::vec4 const& GetDimensions()const;
	void SetDimensions(const glm::vec4& Dim);
	Widget* GetHit(const glm::vec2& Pos);
	bool IsVisible()const;
	void SetVisible(bool Visible);
//<<<<kikurni
protected:
	bool mFlagged;
public:
	bool IsFlagged()const{return mFlagged;}
	void ToggleFlag(){mFlagged=!mFlagged;}
//>>>>

protected:
	void Include(glm::vec4 const& Dim);
	bool IsInside(const glm::vec2& Pos)const;
	int32_t mZOrder;
	Widget* mParent;
	Widget* mNext;
	Widget* mPrev;
	Widget* mFirstChild;
	Widget* mLastChild;
	glm::vec4 mDimensions;
	bool mDimSet;
	bool mVisible;
};

#endif//INCLUDED_UI_WIDGET_H
