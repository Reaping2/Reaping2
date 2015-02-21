#ifndef INCLUDED_UI_UI_H
#define INCLUDED_UI_UI_H
#include "ui/i_ui.h"
class Ui : public Singleton<Ui>
{
    friend class Singleton<Ui>;
    Root mEmptyRoot;
    Root* mActiveRoot;
    Widget* mLastEnteredWidget;
    typedef boost::ptr_map<std::string, Root> Roots_t;
    Roots_t mRoots;
    Ui();
    ModelValue mUiModel;
    ModelValue mLoad;
    AutoReg mOnPressId;
    AutoReg mOnReleaseId;
    AutoReg mOnMoveId;
    AutoReg mKeyId;
    void OnMousePressEvent( UiMousePressEvent const& Evt );
    void OnMouseReleaseEvent( UiMouseReleaseEvent const& Evt );
    void OnMouseMoveEvent( UiMouseMoveEvent const& Evt );
    void OnKeyEvent( const KeyEvent& Event );
public:
    void Load( std::string const& Name );
    Root const& GetRoot() const;
    Root& GetRoot();
    Root& GetRoot( std::string const& Name );
};

#endif//INCLUDED_UI_UI_H
