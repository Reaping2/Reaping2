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
	typedef WidgetIterator const_iterator;	// rename to hierarchy iterator?
	const_iterator begin()const;
	const_iterator end()const;
	Widget(glm::vec4 const& RelativeDimensions=glm::vec4());
	virtual ~Widget();
	void AddChild(Widget* Child);
	virtual bool AreDimensionsSet()const;
	virtual glm::vec4 const& GetDimensions()const;
	void SetRelativeDimensions(glm::vec4 const& Dim);
	Widget* GetHit(const glm::vec2& Pos);
	bool IsVisible()const;
	void SetVisible(bool Visible);
	Widget* GetNext()const;	// vagy child_iterator?

//<<<<kikurni
protected:
	bool mFlagged;
public:
	bool IsFlagged()const{return mFlagged;}
	void ToggleFlag(){mFlagged=!mFlagged;}
//>>>>

protected:
	virtual void UpdateDimensions();
	virtual void UpdateSelfDimensions();
	virtual void UpdateChildrenDimensions();
	bool IsInside(const glm::vec2& Pos)const;
	int32_t mZOrder;
	Widget* mParent;
	Widget* mNext;
	Widget* mPrev;
	Widget* mFirstChild;
	Widget* mLastChild;
	glm::vec4 mRelativeDimensions;
	glm::vec4 mDimensions;
	bool mDimSet;
	bool mVisible;
};

#endif//INCLUDED_UI_WIDGET_H
