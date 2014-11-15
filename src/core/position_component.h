#ifndef INCLUDED_CORE_POSITION_COMPONENT_H
#define INCLUDED_CORE_POSITION_COMPONENT_H

class PositionComponent : public Component
{
public:
	DEFINE_COMPONENT_BASE(PositionComponent)
	double const& GetX()const; //TODO: component style, its not a really good idea for models, if components are changed
	double const& GetY()const;
	double GetOrientation();
	void SetX( double X );
	void SetY( double Y );
	void SetOrientation( double Orientation );
protected:
	PositionComponent();
	friend class ComponentRepo;
private:
	double mX;
	double mY;
	double mOrientation;
};

#endif//INCLUDED_CORE_POSITION_COMPONENT_H