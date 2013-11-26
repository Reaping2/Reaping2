#include "i_core.h"

bool BoxCollisionModel::AreActorsColliding( Actor const& ObjA, Actor const& ObjB, double Dt )const
{
	// A: (0,0)
	glm::vec2 B(ObjB.GetX()-ObjA.GetX(),ObjB.GetY()-ObjA.GetY());
	// BSize: (0,0)
	glm::vec2 ASize(ObjA.GetRadius()+ObjB.GetRadius(),ObjA.GetRadius()+ObjB.GetRadius());
	// on point check
	static const float Epsilon=std::numeric_limits<float>::epsilon()*100;
	if(std::abs(B.x)+Epsilon<ASize.x&&std::abs(B.y)+Epsilon<ASize.y)
		return true;
	glm::vec2 Spd(ObjB.GetSpeedX()-ObjA.GetSpeedX(),ObjB.GetSpeedY()-ObjA.GetSpeedY());
	glm::vec2 T1minusB=ASize-B;
	glm::vec2 T2minusB=-ASize-B;
	glm::vec2 MinTimes(std::numeric_limits<float>::max());
	glm::vec2 MaxTimes(-std::numeric_limits<float>::max());
	static const size_t Dim=2;
	for(size_t i=0;i<Dim;++i)
	{
		if(std::abs(Spd[i])<=Epsilon)
		{	// one dim speed is zero
			if(std::abs(B[i])>ASize[i])continue;
			MinTimes[i]=0;
			MaxTimes[i]=(float)Dt;
			continue;
		}
		float d1=T1minusB[i]/Spd[i];
		float d2=T2minusB[i]/Spd[i];
		float mi=std::min<float>(d1,d2);
		float ma=std::max<float>(d1,d2);
		if(ma<=Epsilon)
		{	// if maxtime is 0/negative, then we're already past the other object, no collision (handle edge cases)
			ma=-1;
			mi=-1;
		}
		MinTimes[i]=mi;
		MaxTimes[i]=ma;
	}
	double MinTime=glm::compMax(MinTimes);
	double MaxTime=glm::compMin(MaxTimes);

	bool const SweepResult=MinTime>=0&&MinTime<=MaxTime&&MinTime<=Dt;
	return SweepResult;
}
