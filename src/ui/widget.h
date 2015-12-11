#ifndef INCLUDED_UI_WIDGET_H
#define INCLUDED_UI_WIDGET_H

#include "platform/i_platform.h"

class Widget
{
protected:
    class WidgetIterator
    {
    private:
        Widget const* mThis;
        int32_t mLevel;
    public:
        WidgetIterator( Widget const* This = NULL ): mThis( This ), mLevel( 0 ) {}
        Widget const& operator*()
        {
            return *mThis;
        }
        Widget const* operator->()
        {
            return mThis;
        }
        bool operator==( WidgetIterator const& Other )const
        {
            return mThis == Other.mThis;
        }
        bool operator!=( WidgetIterator const& Other )const
        {
            return !operator==( Other );
        }
        WidgetIterator& operator++();
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
        Prop();
        Prop( Prop const& Other );
        explicit Prop( int32_t IntVal );
        explicit Prop( double DoubleVal );
        explicit Prop( const std::string& StrVal );
        operator std::string()const;
        operator int32_t()const;
        operator double()const;
        Prop& operator=( std::string const& Str );
        Prop& operator=( char const* Buf );
        Prop& operator=( int32_t I );
        Prop& operator=( double D );
        Type_t GetType()const
        {
            return Type;
        }
        bool IsModelValue()const;
        ~Prop();
    private:
        Type_t Type;
        operator char const* ()const;
        void Init( std::string const& Str );
        void Cleanup();
        ModelValue const& ResolveModel()const;
    };
    enum PropertyType
    {
        PT_Visible,
        PT_Highlight,
        PT_Enabled,
        PT_Text,
        PT_FontSize,
        PT_Color,
        PT_HighlightColor,
        PT_Progress,
        PT_MaxProgress,
        PT_ActorVisual, // name of the actor visual
        PT_Animation,   // name of the animation of the actor visual, defaults to body_idle
        PT_State,       // current state of the animation, defaults to 0
    };
    typedef WidgetIterator const_iterator;  // rename to hierarchy iterator?
    const_iterator begin()const;
    const_iterator end()const;
    Widget( int32_t Id );
    virtual ~Widget();
    void AddChild( Widget* Child );
    virtual bool AreDimensionsSet()const;
    virtual glm::vec4 const& GetDimensions()const;
    void SetRelativeDimensions( glm::vec4 const& Dim );
    Widget* GetHit( const glm::vec2& Pos );
    Widget* GetNext()const; // vagy child_iterator?
    Prop const& operator()( PropertyType Property )const;
    Prop& operator()( PropertyType Property );
    int32_t GetId()const;
    virtual void Init( Json::Value& Descriptor );
    virtual bool Trigger()
    {
        return false;
    }
    virtual void OnMouseEnter() {}
    virtual void OnMouseLeave() {}
protected:
    virtual void UpdateDimensions();
    virtual void UpdateSelfDimensions();
    virtual void UpdateChildrenDimensions();
    bool IsInside( const glm::vec2& Pos )const;
    void ParseIntProp( PropertyType Pt, Json::Value& Val, int32_t Default );
    void ParseDoubleProp( PropertyType Pt, Json::Value& Val, double Default );
    void ParseStrProp( PropertyType Pt, Json::Value& Val, std::string const& Default );
    static int32_t ParseColor( Json::Value& Color, int32_t Default );
    int32_t mZOrder;
    Widget* mParent;
    Widget* mNext;
    Widget* mPrev;
    Widget* mFirstChild;
    Widget* mLastChild;
    glm::vec4 mRelativeDimensions;
    glm::vec4 mDimensions;
    int32_t mTypeId;
    bool mDimSet;
    struct PropertyRepo_t : public Repository<Prop>
    {
        static Prop DefaultProperty;
        PropertyRepo_t();
        Prop& Mutable( PropertyType Property );
    };
    PropertyRepo_t mProperties;
};

#endif//INCLUDED_UI_WIDGET_H
