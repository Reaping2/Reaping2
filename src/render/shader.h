#ifndef INCLUDED_RENDER_SHADER_H
#define INCLUDED_RENDER_SHADER_H

class ShaderRepo;
class ShaderManager;
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
	friend class ShaderManager;
	void Bind()const;
	void Unbind()const;
	GLuint GetProgId()const{return mProgramId;}
public:
	~Shader();
};

#endif//INCLUDED_RENDER_SHADER_H
