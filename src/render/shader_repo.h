#ifndef INCLUDED_RENDER_SHADER_REPO_H
#define INCLUDED_RENDER_SHADER_REPO_H

class ShaderRepo : public Repository<Shader>, public Singleton<ShaderRepo>
{
	Shader mDefaultShader;
	friend class Singleton<ShaderRepo>;
	ShaderRepo();
	void LoadShaders();
	static std::string const mDefaultShaderName;
};

#endif//INCLUDED_RENDER_SHADER_REPO_H
