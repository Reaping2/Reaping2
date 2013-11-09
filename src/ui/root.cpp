#include "i_ui.h"

Root::Root()
: mTexWdg(NULL)
{
	mDimensions=glm::vec4(0.,0.,100.,100.);
	mDimSet=true;
	bool TestUi=true;
	if(TestUi)
		InitTestUi();
}

void Root::InitTestUi()
{
	FixedRatioContainer* FRC=new FixedRatioContainer(1.0,HorizontalAlignment::Right,VerticalAlignment::Top);
	FRC->SetRelativeDimensions(glm::vec4(0.5,0.5,0.25,0.25));
	AddChild(FRC);
	FixedRatioContainer* FRC2=new FixedRatioContainer(1.0,HorizontalAlignment::Center,VerticalAlignment::Top);
	FRC2->SetRelativeDimensions(glm::vec4(0.35,0.8,0.3,0.15));
	AddChild(FRC2);
	mTexWdg=new TextWidget(glm::vec4(0,0,1,1));
	(*mTexWdg)(Widget::PT_Visible)=1;
	(*mTexWdg)(Widget::PT_Text)="REAPING2";
	(*mTexWdg)(Widget::PT_Enabled)=1;
	(*mTexWdg)(Widget::PT_Color)=int32_t(0x000000);
	mTimerId=TimerServer::Get().AddTimer(boost::bind(&Root::OnTimer,this),0.01);
	FRC2->AddChild(mTexWdg);
	bool TestButtons=false;
	if(!TestButtons)return;
	Widget* WDG=new Widget(glm::vec4(0.75,0,0.25,0.25));
	AddChild(WDG);
	const int I=2;
	const int J=2;
	const int K=2;
	for(int ii=0;ii<2;++ii)
	{
		Widget* Wid=ii?FRC:WDG;
		for(int i=0;i<I*I;++i)
		{
			Widget* Itr=new Widget(glm::vec4((1./I)*(i/I),(1./I)*(i%I),(1./I),(1./I)));
			Wid->AddChild(Itr);
			for(int j=0;j<J*J;++j)
			{
				Widget* It2=new Widget(glm::vec4((1./J)*(j/J),(1./J)*(j%J),(1./J),(1./J)));
				Itr->AddChild(It2);
				for(int k=0;k<K;++k)
				{
					Widget* It3=new Widget(glm::vec4(1./K*k,1./K*k,1./K,1./K));
					(*It3)(Widget::PT_Visible)=1;
					(*It3)(Widget::PT_Enabled)=1;
					It2->AddChild(It3);
				}
			}
		}
	}
}

void Root::OnTimer()
{
	int32_t Col=(*mTexWdg)(Widget::PT_Color);
	if(Col<0xff0000)
	{
		(*mTexWdg)(Widget::PT_Color)=Col+0x010000;
	}
	else
	{
		(*mTexWdg)(Widget::PT_Color)=0xff0000;
		mTimerId.Unregister();
	}
}
