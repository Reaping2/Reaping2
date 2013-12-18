#ifndef INCLUDED_RENDER_VAO_BASE_H
#define INCLUDED_RENDER_VAO_BASE_H

class VaoBase
{
	GLuint mDataBuffer;
	GLuint mVAO;
	GLuint mSampler;
public:
	void Init();
	void Bind();
	void Unbind();
};

#endif//INCLUDED_RENDER_VAO_BASE_H
