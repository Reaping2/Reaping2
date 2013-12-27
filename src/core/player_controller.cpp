#include "i_core.h"
#include "input/i_input.h"

PlayerController::PlayerController()
: mCurrentMovement(0)
, mDirty(true)
, mMouse(Mouse::Get())
, mPlayerModel("player",&RootModel::Get())
{
	Keyboard& Keys=Keyboard::Get();
	mKeyId=EventServer<KeyEvent>::Get().Subscribe(boost::bind(&PlayerController::OnKeyEvent,this,_1));
	mMouseMoveId=EventServer<WorldMouseMoveEvent>::Get().Subscribe(boost::bind(&PlayerController::OnMouseMoveEvent,this,_1));
	mMousePressId=EventServer<WorldMousePressEvent>::Get().Subscribe(boost::bind(&PlayerController::OnMousePressEvent,this,_1));
	mMouseReleaseId=EventServer<WorldMouseReleaseEvent>::Get().Subscribe(boost::bind(&PlayerController::OnMouseReleaseEvent,this,_1));
}

void PlayerController::SetActor(Actor* Obj)
{
	mPlayerModels.clear();
	Controller::SetActor(Obj);
	mPlayerModels.push_back(new ModelValue(Obj->GetHP(),"hp",&mPlayerModel));
	mPlayerModels.push_back(new ModelValue(Obj->GetX(),"x",&mPlayerModel));
	mPlayerModels.push_back(new ModelValue(Obj->GetY(),"y",&mPlayerModel));
}

void PlayerController::OnKeyEvent(const KeyEvent& Event)
{
	uint32_t OldMovement=mCurrentMovement;
	uint32_t Mod=0;
	if(Event.Key==GLFW_KEY_W)Mod=MF_Up;
	else if(Event.Key==GLFW_KEY_A)Mod=MF_Left;
	else if(Event.Key==GLFW_KEY_S)Mod=MF_Down;
	else if(Event.Key==GLFW_KEY_D)Mod=MF_Right;
	if(Event.State==KeyState::Down)
		mCurrentMovement|=Mod;
	else
		mCurrentMovement&=~Mod;
	mDirty=mDirty||mCurrentMovement!=OldMovement;
}

void PlayerController::Update( double Seconds )
{
	UpdateRotation();
	if(!mActor)return;
	if(!mDirty)return;
	mDirty=false;
	int x=((mCurrentMovement&MF_Left)?-1:0)+((mCurrentMovement&MF_Right)?1:0);
	int y=((mCurrentMovement&MF_Up)?1:0)+((mCurrentMovement&MF_Down)?-1:0);
	mActor->SetSpeed(std::max<double>(std::abs(x),std::abs(y))*.35);
	if(x==0&&y==0)
	{
		mActor->AddAction(AutoId("idle"));
		mActor->AddAction(AutoId("body_idle"));
	}
	double Heading=0;
	static const double pi=boost::math::constants::pi<double>();
	if(x==0)
		Heading=(y<0)?-pi/2:pi/2;
	else if(y==0)
		Heading=(x<0)?-pi:0;
	else if(y>0)
		Heading=(x<0)?pi*0.75:pi/4;
	else
		Heading=(x<0)?pi*1.25:pi*1.75;

	mActor->SetHeading(Heading);
	if(x==0&&y==0)
	{

	}
	else
	{
		mActor->AddAction(AutoId("move"));
		mActor->AddAction(AutoId("body_move"));
	}
}

PlayerController::~PlayerController()
{
	mPlayerModels.clear();
}

void PlayerController::OnMouseMoveEvent(const WorldMouseMoveEvent& Event)
{
	mX=Event.Pos.x;
	mY=Event.Pos.y;
	UpdateRotation();
}

void PlayerController::UpdateRotation()
{
	double Rot=atan2(mY-mActor->GetY(),mX-mActor->GetX());
	mActor->SetOrientation(Rot);
}

void PlayerController::OnMousePressEvent(const WorldMousePressEvent& Event)
{
	if (Event.Button==Mouse::Button_Left&&!mMouse.IsButtonPressed(Mouse::Button_Left))
		mActor->AddAction(AutoId("shoot"));
	else if (Event.Button==Mouse::Button_Right&&!mMouse.IsButtonPressed(Mouse::Button_Right))
		mActor->AddAction(AutoId("shoot_alt"));
	// ez itt pusztan funkcionalitas tesztelesre van, dummy implementacio
	static const double Cooldown=0.2;
	static double PrevTime=0;
	const double CurTime=glfwGetTime();
	if(CurTime-PrevTime<Cooldown) return;
	PrevTime=CurTime;
	if (Event.Button==Mouse::Button_Middle)
	{
		Creep* Obj=new Creep(rand()%2?"pok1":"pok2", Event.Pos.x,Event.Pos.y,rand()%2?mActor:(Actor*)NULL);
		Scene::Get().AddActor(Obj);
	}
}


void PlayerController::OnMouseReleaseEvent(const WorldMouseReleaseEvent& Event)
{
	if (Event.Button==Mouse::Button_Left)
	{
		mActor->DropAction(AutoId("shoot"));
		if (mMouse.IsButtonPressed(Mouse::Button_Right))
			mActor->AddAction(AutoId("shoot_alt"));
	}
	else if (Event.Button==Mouse::Button_Right)
	{
		mActor->DropAction(AutoId("shoot_alt"));
		if (mMouse.IsButtonPressed(Mouse::Button_Left))
			mActor->AddAction(AutoId("shoot"));
	}
}
