#include "i_ui.h"

void Ui::Load( std::string const& Name )
{
	if(mLastEnteredWidget)
		mLastEnteredWidget->OnMouseLeave();
	mLastEnteredWidget=NULL;
	Roots_t::iterator it=mRoots.find(Name);
	if(mRoots.end()!=it)
	{
		mActiveRoot=it->second;
		return;
	}
	Root* NewRoot=new Root;
	AutoFile F=Filesys::Get().Open(boost::filesystem::path("ui")/(Name+".json"));
	if(!F.get())return;
	JsonReader Reader(*F);
	if(!Reader.IsValid())return;
	NewRoot->Init(Reader.GetRoot());
	mRoots.insert(Name,std::auto_ptr<Root>(NewRoot));
	mActiveRoot=NewRoot;
}

Root const& Ui::GetRoot() const
{
	return *mActiveRoot;
}

Root& Ui::GetRoot()
{
	return *mActiveRoot;
}

Ui::Ui()
: mUiModel("ui",&RootModel::Get())
, mLoad(StringFunc(this,&Ui::Load),"load",&mUiModel)
, mActiveRoot(&mEmptyRoot)
, mLastEnteredWidget(NULL)
{
	mKeyId=EventServer<KeyEvent>::Get().Subscribe(boost::bind(&Ui::OnKeyEvent,this,_1));
	mOnPressId=EventServer<UiMousePressEvent>::Get().Subscribe(boost::bind(&Ui::OnMousePressEvent,this,_1));
	mOnReleaseId=EventServer<UiMouseReleaseEvent>::Get().Subscribe(boost::bind(&Ui::OnMouseReleaseEvent,this,_1));
	mOnMoveId=EventServer<UiMouseMoveEvent>::Get().Subscribe(boost::bind(&Ui::OnMouseMoveEvent,this,_1));
	Load("start");
}

void Ui::OnMousePressEvent( UiMousePressEvent const& Evt )
{
	Widget* Wdg=mActiveRoot->GetHit(Evt.Pos);
	if(!Wdg)return;
	Evt.SetHandled();
}

void Ui::OnMouseReleaseEvent( UiMouseReleaseEvent const& Evt )
{
	Widget* Wdg=mActiveRoot->GetHit(Evt.Pos);
	if(!Wdg)return;
	if(Wdg->Trigger())
		Evt.SetHandled();
}

void Ui::OnKeyEvent( const KeyEvent& Event )
{
	if(Event.Key==GLFW_KEY_ESCAPE&&Event.State==KeyState::Down)
	{
		// todo: a hud onload/onexit actionje pause es resumeoljon
		RootModel::Get()["scene"]["pause"]();
		Load("start");
	}
}

void Ui::OnMouseMoveEvent( UiMouseMoveEvent const& Evt )
{
	Widget* Wdg=mActiveRoot->GetHit(Evt.Pos);
	if(mLastEnteredWidget==Wdg)
		return;
	if(mLastEnteredWidget)
		mLastEnteredWidget->OnMouseLeave();
	mLastEnteredWidget=Wdg;
	if(!mLastEnteredWidget)
		return;
	mLastEnteredWidget->OnMouseEnter();
}

