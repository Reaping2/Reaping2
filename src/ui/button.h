#ifndef INCLUDED_UI_BUTTON_H
#define INCLUDED_UI_BUTTON_H

class Button : public Widget
{
    typedef Widget BaseClass;
    struct ActionDesc
    {
        ActionDesc( Widget* w );
        std::string mAction;
        Widget::Prop mArg;
    };
    typedef std::vector<ActionDesc> Actions_t;
    enum ActionType {
        AT_Trigger,
        AT_MouseEnter,
        AT_MouseLeave,
        NumActionTypes,
    };
    typedef std::map<ActionType, Actions_t> ActionsMap_t;
    ActionsMap_t mActions;
    void AddAction( Actions_t& target, Json::Value& TriggerAction );
    void InitAction( ActionType type, Json::Value& actionDesc );
    bool ExecuteAction( ActionType type );
public:
    Button( int32_t Id );
    virtual void Init( Json::Value& Descriptor );
    virtual bool Trigger();
    virtual void OnMouseEnter();
    virtual void OnMouseLeave();
};

#endif//INCLUDED_UI_BUTTON_H
