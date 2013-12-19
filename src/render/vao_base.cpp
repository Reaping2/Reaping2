#include "i_render.h"

void VaoBase::Init()
{
	glGenBuffers(1,&mDataBuffer);

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mDataBuffer);

	glGenSamplers(1,&mSampler);
	glSamplerParameteri(mSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(mSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(mSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	Unbind();
}

void VaoBase::Bind()
{
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mDataBuffer);
}

void VaoBase::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
