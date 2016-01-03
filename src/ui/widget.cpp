#include "i_ui.h"
#include <boost/algorithm/string.hpp>
#include <string.h>

Widget::WidgetIterator& Widget::WidgetIterator::operator++()
{
    if( mThis->mFirstChild )
    {
        mThis = mThis->mFirstChild;
        ++mLevel;
    }
    else if( mThis->mNext )
    {
        mThis = mThis->mNext;
    }
    else
    {
        Widget const* Next = mThis;
        while( Next && !Next->mNext && mLevel )
        {
            Next = Next->mParent;
            --mLevel;
        }
        mThis = Next->mNext;
    }
    return *this;
}

void Widget::AddChild( Widget* Child )
{
    assert( !Child->mPrev && !Child->mParent && !Child->mNext );

    Widget* WhatWillBeNext = mFirstChild;
    while( WhatWillBeNext && WhatWillBeNext->mZOrder < Child->mZOrder )
    {
        WhatWillBeNext = WhatWillBeNext->mNext;
    }

    if( WhatWillBeNext )
    {
        Child->mPrev = WhatWillBeNext->mPrev;
        if( WhatWillBeNext->mPrev )
        {
            WhatWillBeNext->mPrev->mNext = Child;
        }
        Child->mNext = WhatWillBeNext;
        WhatWillBeNext->mPrev = Child;
        if( WhatWillBeNext == mFirstChild )
        {
            mFirstChild = Child;
        }
    }
    else
    {
        if( !mFirstChild )
        {
            mFirstChild = Child;
        }
        mLastChild = Child;
    }
    Child->mParent = this;
    Child->UpdateDimensions();
    Child->UpdateChildrenDimensions();
}

Widget::Widget( int32_t Id )
    : mTypeId( Id )
    , mZOrder( 0 )
    , mParent( NULL )
    , mNext( NULL )
    , mPrev( NULL )
    , mFirstChild( NULL )
    , mLastChild( NULL )
    , mDimSet( false )
    , mRelativeDimensions( 0, 0, 1, 1 )
    , mProperties( this )
{
    operator()( PT_Highlight ) = 0;
}

Widget const* Widget::Parent() const
{
    return mParent;
}

Widget::~Widget()
{
    Widget* What = mFirstChild;
    while( What )
    {
        Widget* Next = What->mNext;
        delete What;
        What = Next;
    }
    if( mNext )
    {
        mNext->mPrev = mPrev;
    }
    if( mPrev )
    {
        mPrev->mNext = mNext;
    }
    if( mParent )
    {
        if( mParent->mFirstChild == this )
        {
            mParent->mFirstChild = mNext;
        }
        if( mParent->mLastChild == this )
        {
            mParent->mLastChild = mPrev;
        }
    }
}

glm::vec4 const& Widget::GetDimensions() const
{
    return mDimensions;
}

Widget::const_iterator Widget::begin() const
{
    return WidgetIterator( this );
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
    if( !mParent || !mParent->mDimSet )
    {
        return;
    }
    mDimSet = true;
    glm::vec4 const& ParentDim = mParent->GetDimensions();
    double const X = ParentDim.x;
    double const Y = ParentDim.y;
    double const Width = ParentDim.z;
    double const Height = ParentDim.w;
    mDimensions = glm::vec4(  X + mRelativeDimensions.x * Width,
                              Y + mRelativeDimensions.y * Height,
                              mRelativeDimensions.z * Width,
                              mRelativeDimensions.w * Height );
}

void Widget::UpdateChildrenDimensions()
{
    if( !mParent || !mParent->mDimSet )
    {
        return;
    }
    for( Widget* i = mFirstChild; i; i = i->mNext )
    {
        i->UpdateDimensions();
    }
}

Widget* Widget::GetHit( const glm::vec2& Pos )
{
    if( !IsInside( Pos ) )
    {
        return NULL;
    }
    for( Widget* i = mLastChild; i; i = i->mPrev )
    {
        Widget* Wdg = i->GetHit( Pos );
        if( Wdg )
        {
            return Wdg;
        }
    }
    return mProperties( PT_Enabled ).Value.ToInt ? this : NULL;
}

bool Widget::IsInside( const glm::vec2& Pos ) const
{
    return mDimSet &&
           Pos.x >= mDimensions.x &&
           Pos.x <= mDimensions.x + mDimensions.z &&
           Pos.y >= mDimensions.y &&
           Pos.y <= mDimensions.y + mDimensions.w;
}

void Widget::SetRelativeDimensions( glm::vec4 const& Dim )
{
    mRelativeDimensions = Dim;
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

Widget::Prop const& Widget::operator()( PropertyType Property ) const
{
    return mProperties( Property );
}

Widget::Prop& Widget::operator()( PropertyType Property )
{
    return mProperties.Mutable( Property );
}

int32_t Widget::ParseColor( Json::Value& Color, int32_t Default )
{
    int32_t i;
    if( Json::GetColor( Color, i ) )
    {
        return i;
    }
    return Default;
}

void Widget::Init( Json::Value& Descriptor )
{
    double d;
    mRelativeDimensions.x = Json::GetDouble( Descriptor["x"], d ) ? d : 0;
    mRelativeDimensions.y = Json::GetDouble( Descriptor["y"], d ) ? d : 0;
    mRelativeDimensions.z = Json::GetDouble( Descriptor["w"], d ) ? d : 0;
    mRelativeDimensions.w = Json::GetDouble( Descriptor["h"], d ) ? d : 0;
    static const int32_t DefaultColor = 0x0000fa77;
    int32_t i = ParseColor( Descriptor["color"], DefaultColor );
    operator()( PT_Color ) = i;
    operator()( PT_HighlightColor ) = ParseColor( Descriptor["highlight_color"], i );
    ParseIntProp( PT_Enabled, Descriptor["enabled"], 0 );
    ParseIntProp( PT_Visible, Descriptor["visible"], 0 );
    Json::Value& Children = Descriptor["children"];
    if( !Children.isArray() || !Children.size() )
    {
        return;
    }
    WidgetFactory& Fact( WidgetFactory::Get() );
    for( Json::Value::iterator i = Children.begin(), e = Children.end(); i != e; ++i )
    {
        std::string Str;
        if( !Json::GetStr( ( *i )["type"], Str ) )
        {
            continue;
        }
        std::auto_ptr<Widget> wdg = Fact( Str, *i );
        AddChild( wdg.release() );
    }
}

int32_t Widget::GetId() const
{
    return mTypeId;
}

void Widget::ParseIntProp( PropertyType Pt, Json::Value& Val, int32_t Default )
{
    ParseIntProp( operator()( Pt ), Val, Default );
}

void Widget::ParseIntProp( Prop& Pt, Json::Value& Val, int32_t Default )
{
    if( Val.isString() )
    {
        Pt = Val.asString();
        assert( Pt.IsResolvable() );
    }
    else
    {
        int32_t i;
        Pt = Json::GetInt( Val, i ) ? i : Default;
    }
}

void Widget::ParseDoubleProp( PropertyType Pt, Json::Value& Val, double Default )
{
    ParseDoubleProp( operator()( Pt ), Val, Default );
}

void Widget::ParseDoubleProp( Prop& Pt, Json::Value& Val, double Default )
{
    if( Val.isString() )
    {
        Pt = Val.asString();
        assert( Pt.IsResolvable() );
    }
    else
    {
        double d;
        Pt = Json::GetDouble( Val, d ) ? d : Default;
    }
}

void Widget::ParseStrProp( PropertyType Pt, Json::Value& Val, std::string const& Default )
{
    ParseStrProp( operator()( Pt ), Val, Default );
}

void Widget::ParseStrProp( Prop& Pt, Json::Value& Val, std::string const& Default )
{
    Pt = Val.isString() ? Val.asString() : Default;
}

Widget::Prop::Prop( Widget* owner )
    : Type( T_Null )
    , mOwner( owner )
{
    Value.ToInt = 0;
}

Widget* Widget::Prop::GetOwner() const
{
    return mOwner;
}

Widget::Prop::Prop( Prop const& Other )
    : Type( Other.Type )
    , mOwner( Other.mOwner )
{
    if( Type == T_Str )
    {
        Init( std::string( Other.Value.ToStr ) );
    }
}

Widget::Prop::~Prop()
{
    Cleanup();
}

Widget::Prop::operator int32_t() const
{
    if( IsResolvable() )
    {
        if( IsAutoId() )
        {
            return (int32_t)AutoId( Value.ToStr + 2 );
        }
        if( IsVectorModelValue() )
        {
            std::vector<int32_t> const& v = ResolveModel().operator std::vector<int32_t>();
            int32_t idx = ResolveIndex();
            return v.size() > idx ? v.at( idx ) : 0;
        }
        if( IsModelIndex() )
        {
            return ResolveIndex();
        }
        return( int32_t )ResolveModel();
    }
    assert( Type == T_Int || Type == T_Double );
    return ( Type == T_Int ) ? Value.ToInt : ( Type == T_Double ? int32_t( Value.ToDouble ) : 0 );
}

Widget::Prop::operator double() const
{
    if( IsResolvable() )
    {
        return( double )ResolveModel();
    }
    assert( Type == T_Int || Type == T_Double );
    return ( Type == T_Double ) ? Value.ToDouble : ( Type == T_Int ? double( Value.ToInt ) : 0.0 );
}

Widget::Prop& Widget::Prop::operator=( std::string const& Str )
{
    Cleanup();
    Type = T_Str;
    Init( Str );
    return *this;
}

Widget::Prop& Widget::Prop::operator=( char const* Str )
{
    Cleanup();
    Type = T_Str;
    Init( std::string( Str ) );
    return *this;
}

Widget::Prop& Widget::Prop::operator=( int32_t I )
{
    Cleanup();
    Type = T_Int;
    Value.ToInt = I;
    return *this;
}

Widget::Prop& Widget::Prop::operator=( double D )
{
    Cleanup();
    Type = T_Double;
    Value.ToDouble = D;
    return *this;
}

void Widget::Prop::Init( std::string const& StrVal )
{
    Type = T_Str;
    const size_t Size = StrVal.size();
    char* Buf = new char[Size + 1];
    memset( Buf, 0, Size + 1 );
    memcpy( Buf, StrVal.c_str(), Size );
    Value.ToStr = Buf;
}

void Widget::Prop::Cleanup()
{
    if( Type == T_Str )
    {
        delete[] Value.ToStr;
    }
    Type = T_Null;
}

Widget::Prop::operator std::string() const
{
    if( IsResolvable() )
    {
        if( IsVectorModelValue() )
        {
            std::vector<std::string> const& v = ResolveModel().operator std::vector<std::string>();
            int32_t idx = ResolveIndex();
            return v.size() > idx ? v.at( idx ) : std::string();
        }
        return( std::string )ResolveModel();
    }
    assert( Type == T_Str );
    return ( Type == T_Str ) ? Value.ToStr : NULL;
}

bool Widget::Prop::IsResolvable() const
{
    return Type == T_Str && Value.ToStr && ( *Value.ToStr == '%' || *Value.ToStr == '#' );
}

bool Widget::Prop::IsAutoId() const
{
    return IsResolvable() && Value.ToStr[1] == '%';
}

bool Widget::Prop::IsModelValue() const
{
    return Type == T_Str && Value.ToStr && (( *Value.ToStr == '%' && Value.ToStr[1] != '%' ) || *Value.ToStr == '#');
}

bool Widget::Prop::IsModelIndex() const
{
    return Type == T_Str && Value.ToStr && *Value.ToStr == '#';
}

bool Widget::Prop::IsVectorModelValue() const
{
    return IsModelValue() && strrchr( Value.ToStr + 1, '#' ) != NULL ;
}

ModelValue const& Widget::Prop::ResolveModel() const
{
    assert( IsResolvable() );
    std::string name = std::string( ( *Value.ToStr == '#' ) ?  "ui." : "" ) + std::string( Value.ToStr + 1);
    typedef std::vector<std::string> Fields_t;
    Fields_t fields;
    boost::split( fields, name, boost::is_any_of( "#%" ) );
    return RootModel::Get()[ fields[0] ];
}

int32_t Widget::Prop::ResolveIndex() const
{
    assert( IsVectorModelValue() || IsModelIndex() );
    typedef std::vector<std::string> Fields_t;
    Fields_t fields;
    boost::split( fields, Value.ToStr, boost::is_any_of( "#" ) );
    std::string last = fields.back();
    int32_t startindex = RootModel::Get()[ "ui." + last ].operator int32_t();
    for( Widget const* w = mOwner; NULL != w; w = w->Parent() )
    {
        Prop const& p = w->operator()( PT_StartId );
        if( p.GetType() != T_Str )
        {
            continue;
        }
        if( p.Value.ToStr + 1 !=  last )
        {
            continue;
        }
        // found a widget with the same startid
        // check for index prop
        Prop const& p2 = w->operator()( PT_Index );
        if( p2.GetType() != T_Int )
        {
            continue;
        }
        return startindex + p2.operator int32_t();
    }
    return startindex;
}

Widget::PropertyRepo_t::PropertyRepo_t( Widget* owner )
    : mOwner( owner )
    , mDefaultProperty( owner )
    , RepoBase( mDefaultProperty )
{
}

Widget::Prop& Widget::PropertyRepo_t::Mutable( PropertyType Property )
{
    int32_t Id( Property );
    ElementMap_t::iterator i = mElements.find( Id );
    if( mElements.end() == i )
    {
        Prop* p = new Prop( mOwner );
        mElements.insert( Id, p );
        return *p;
    }
    return *i->second;
}

