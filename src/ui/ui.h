#ifndef INCLUDED_UI_UI_H
#define INCLUDED_UI_UI_H

class Ui : public Singleton<Ui>
{
	friend class Singleton<Ui>;
	Root mEmptyRoot;
	Root* mActiveRoot;
	typedef boost::ptr_map<std::string, Root> Roots_t;
	Roots_t mRoots;
	Ui();
	ModelValue mUiModel;
	ModelValue mLoad;
	Registration mOnPressId;
	Registration mOnReleaseId;
	Registration mKeyId;
	void OnMousePressEvent(UiMousePressEvent const& Evt);
	void OnMouseReleaseEvent(UiMouseReleaseEvent const& Evt);
	void OnKeyEvent(const KeyEvent& Event);
public:
	void Load(std::string const& Name);
	Root const& GetRoot() const;
	Root& GetRoot();
};

#endif//INCLUDED_UI_UI_H
