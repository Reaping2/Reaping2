#ifndef INCLUDED_RENDER_SHADER_H
#define INCLUDED_RENDER_SHADER_H

class ShaderRepo;
class Shader
{
	GLuint mVertexId;
	GLuint mFragmentId;
	GLuint mProgramId;
	void Load(std::string const& Name);
	void Unload();
	friend class ShaderRepo;
	Shader(std::string const& Name);
	Shader();
	Shader(const Shader&);
public:
	~Shader();
	void Bind()const;
	void Unbind()const;
	GLuint GetProgId()const{return mProgramId;}
};

#endif//INCLUDED_RENDER_SHADER_H
