#ifndef INCLUDED_UI_WIDGET_H
#define INCLUDED_UI_WIDGET_H

class Widget : public AutoId
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
	struct Prop
	{
		union
		{
			double ToDouble;
			int32_t ToInt;
			const char* ToStr;
		} Value;
		enum Type_t
		{
			T_Null,
			T_Double,
			T_Int,
			T_Str,
		};
		const Type_t Type;
		Prop();
		explicit Prop(int32_t IntVal);
		explicit Prop(double DoubleVal);
		explicit Prop(const std::string& StrVal);
		~Prop();
	};
	enum PropertyType {
		PT_Visible,
		PT_Flagged,
		PT_Enabled,
		PT_Text,
		PT_Color,
	};
	typedef WidgetIterator const_iterator;	// rename to hierarchy iterator?
	const_iterator begin()const;
	const_iterator end()const;
	Widget(glm::vec4 const& RelativeDimensions=glm::vec4(),std::string const& Name="widget");
	virtual ~Widget();
	void AddChild(Widget* Child);
	virtual bool AreDimensionsSet()const;
	virtual glm::vec4 const& GetDimensions()const;
	void SetRelativeDimensions(glm::vec4 const& Dim);
	Widget* GetHit(const glm::vec2& Pos);
	Widget* GetNext()const;	// vagy child_iterator?
	Prop const& GetProp(PropertyType Property)const;
	void SetProp(PropertyType Property, std::string const& StringVal);
	void SetProp(PropertyType Property, int32_t IntVal);
	void SetProp(PropertyType Property, double DoubleVal);
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
	struct PropertyRepo_t : public Repository<Prop>
	{
		static Prop DefaultProperty;
		PropertyRepo_t();
		void Set(PropertyType Property, Prop* Prp);
	};
	PropertyRepo_t mProperties;
};

#endif//INCLUDED_UI_WIDGET_H
