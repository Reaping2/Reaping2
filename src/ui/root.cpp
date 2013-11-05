#include "i_ui.h"

Root::Root()
{
	mDimensions=glm::vec4(0.,0.,100.,100.);
	mDimSet=true;
	bool TestUi=true;
	if(TestUi)
		InitTestUi();
}

void Root::InitTestUi()
{
	FixedRatioContainer* FRC=new FixedRatioContainer(1.0);
	FRC->SetRelativeDimensions(glm::vec4(0,0,0.25,0.25));
	AddChild(FRC);
	Widget* WDG=new Widget(glm::vec4(0.75,0,0.25,0.25));
	AddChild(WDG);
	const int I=5;
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
					It3->SetVisible(true);
					It2->AddChild(It3);
				}
			}
		}
	}
}
