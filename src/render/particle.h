#ifndef INCLUDED_RENDER_PARTICLE_H
#define INCLUDED_RENDER_PARTICLE_H

struct Particle
{
	glm::vec2 mQuad[4];
	//glm::vec2 mSpeed;
	int32_t mId;
	glm::vec4 mColor;
	//int32_t mMovementId;
	Particle():mId(0),mColor(1.0f){}
};

#endif//INCLUDED_RENDER_PARTICLE_H
