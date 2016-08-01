#include "i_ui.h"
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>

Button::ActionDesc::ActionDesc( Widget* w )
    : mArg( w )
{
}

void Button::Init( Json::Value& Descriptor )
{
    BaseClass::Init( Descriptor );
    static std::map<std::string, ActionType> const actiontypes = boost::assign::map_list_of
        ( "action", AT_Trigger )
        ( "enter", AT_MouseEnter )
        ( "leave", AT_MouseLeave );
    for( auto i = actiontypes.begin(), e = actiontypes.end(); i != e; ++i )
    {
        InitAction( i->second, Descriptor[ i->first ] );
    }
}

void Button::InitAction( ActionType type, Json::Value& TriggerAction )
{
    if( !TriggerAction.isArray() )
    {
        return;
    }
    size_t const NumActions = TriggerAction.size();
    if( !NumActions )
    {
        return;
    }
    auto& actions = mActions[ type ];
    actions.reserve( NumActions );
    for( Json::Value::iterator i = TriggerAction.begin(), e = TriggerAction.end(); i != e; ++i )
    {
        AddAction( actions, *i );
    }
}

void Button::AddAction( Actions_t& target, Json::Value& TriggerAction )
{
    if( !TriggerAction.isArray() )
    {
        return;
    }
    size_t const NumActions = TriggerAction.size();
    if( !NumActions )
    {
        return;
    }
    ActionDesc Desc( this );
    if( !Json::GetStr( TriggerAction[0], Desc.mAction ) )
    {
        return;
    }
    boost::algorithm::replace_first( Desc.mAction, "#", "ui." );
    boost::algorithm::erase_all( Desc.mAction, "%" );
    if( NumActions == 2 )
    {
        Json::Value& Arg = TriggerAction[1];
        if( Arg.isInt() )
        {
            Desc.mArg = ( int32_t )Arg.asInt();
        }
        else if( Arg.isDouble() )
        {
            Desc.mArg = Arg.asDouble();
        }
        else if( Arg.isString() )
        {
            Desc.mArg = Arg.asString();
        }
    }
    target.push_back( Desc );
}

bool Button::Trigger()
{
    return ExecuteAction( AT_Trigger );
}

bool Button::ExecuteAction( ActionType type )
{
    Actions_t& acts = mActions[type];
    for( Actions_t::const_iterator i = acts.begin(), e = acts.end(); i != e; ++i )
    {
        ActionDesc const& Act = *i;
        ModelValue const& Model = RootModel::Get()[ Act.mAction ];
        switch( Model.GetType() )
        {
        case ModelValue::Mt_VoidFunction:
            RootModel::Get()[Act.mAction]();
            break;
        case ModelValue::Mt_IntFunction:
            RootModel::Get()[Act.mAction]( ( int32_t )Act.mArg );
            break;
        case ModelValue::Mt_DoubleFunction:
            RootModel::Get()[Act.mAction]( ( double )Act.mArg );
            break;
        case ModelValue::Mt_StringFunction:
            RootModel::Get()[Act.mAction]( Act.mArg.operator std::string() );
            break;
        default:
            assert( false );
            break;
        }
    }
    return !acts.empty();
}

Button::Button( int32_t Id )
    : Widget( Id )
{

}

void Button::OnMouseEnter()
{
    operator()( PT_Highlight ) = 1;
    ExecuteAction( AT_MouseEnter );
}

void Button::OnMouseLeave()
{
    operator()( PT_Highlight ) = 0;
    ExecuteAction( AT_MouseLeave );
}
