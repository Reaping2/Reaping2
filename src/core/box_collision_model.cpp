#include "i_core.h"

bool BoxCollisionModel::AreActorsColliding( Actor const& ObjA, Actor const& ObjB, double Dt )const
{
	// A: (0,0)
	glm::vec2 B(ObjB.GetX()-ObjA.GetX(),ObjB.GetY()-ObjA.GetY());
	// BSize: (0,0)
	glm::vec2 ASize(ObjA.GetRadius()+ObjB.GetRadius(),ObjA.GetRadius()+ObjB.GetRadius());
	// on point check
	if(std::abs(B.x)<ASize.x&&std::abs(B.y)<ASize.y)
		return true;
	// slow movement, don't sweep
	//if(ObjA.GetSpeed()<10&&ObjB.GetSpeed()<10)
	//	return false;
	// sweep
	glm::vec2 Spd(ObjB.GetSpeedX()-ObjA.GetSpeedX(),ObjB.GetSpeedY()-ObjA.GetSpeedY());
	glm::vec2 T1minusB=ASize-B;
	glm::vec2 T2minusB=-ASize-B;
	glm::vec2 MinTimes(std::numeric_limits<float>::max());
	glm::vec2 MaxTimes(std::numeric_limits<float>::min());
	static const size_t Dim=2;
	for(size_t i=0;i<Dim;++i)
	{
		if(std::abs(Spd[i])<=std::numeric_limits<float>::epsilon())
			continue;
		float d1=T1minusB[i]/Spd[i];
		float d2=T2minusB[i]/Spd[i];
		MinTimes[i]=std::min<float>(d1,d2);
		MaxTimes[i]=std::max<float>(d1,d2);
	}
	double MinTime=glm::compMax(MinTimes);
	double MaxTime=glm::compMin(MaxTimes);


	bool const SweepResult=MinTime>0&&MinTime<MaxTime&&MaxTime<Dt;
	if(SweepResult)
		L2("Sweep result %d\n",SweepResult);
	return SweepResult;
}
