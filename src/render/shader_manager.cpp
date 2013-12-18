#include "i_render.h"

ShaderManager::ShaderManager()
:mActiveShader(NULL)
{
	ShaderRepo::Get();
	InitGlobalUniforms();
}

void ShaderManager::InitGlobalUniforms()
{
	mGlobalOffsets[0]=0;
	mGlobalOffsets[GlobalShaderData::WorldProjection+1]=mGlobalOffsets[GlobalShaderData::WorldProjection]+sizeof(glm::mat4);
	mGlobalOffsets[GlobalShaderData::WorldCamera+1]=mGlobalOffsets[GlobalShaderData::WorldCamera]+sizeof(glm::mat4);
	mGlobalOffsets[GlobalShaderData::UiProjection+1]=mGlobalOffsets[GlobalShaderData::UiProjection]+sizeof(glm::mat4);
	glGenBuffers(1,&mGlobalsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER,mGlobalsUBO);
	glBufferData(GL_UNIFORM_BUFFER,mGlobalOffsets[GlobalShaderData::TotalSize],NULL,GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER,0);
	glBindBufferRange(GL_UNIFORM_BUFFER,0,mGlobalsUBO,0,sizeof(glm::mat4)*2);
}

void ShaderManager::ActivateShader( std::string const& Name )
{
	if(mActiveShader!=NULL&&mActiveShaderName==Name)
		return;
	mActiveShaderName=Name;
	mActiveShader=&ShaderRepo::Get()(AutoId(Name));
	mActiveShader->Bind();
}

ShaderManager::~ShaderManager()
{
	glDeleteBuffers(1,&mGlobalsUBO);
}

GLuint ShaderManager::GetUniformLocation( std::string const& Loc )
{
	if(!mActiveShader)return -1;
	LocMap_t& LocMap=mShaderLocs[mActiveShader->GetProgId()];
	LocMap_t::const_iterator i=LocMap.find(Loc);
	if(i==LocMap.end())
	{
		GLuint LocId=glGetUniformLocation(mActiveShader->GetProgId(),Loc.c_str());
		LocMap[Loc]=LocId;
		return LocId;
	}
	return i->second;
}

template<>
void ShaderManager::UploadData( std::string const& Name,GLuint const& Data )
{
	GLuint Loc=GetUniformLocation(Name);
	if(Loc==-1)return;
	glUniform1i(Loc,Data);
}

template<>
void ShaderManager::UploadData( std::string const& Name,glm::vec2 const& Data )
{
	GLuint Loc=GetUniformLocation(Name);
	if(Loc==-1)return;
	glUniform2fv(Loc,1,glm::value_ptr(Data));
}

template<>
void ShaderManager::UploadData( std::string const& Name,glm::mat2 const& Data )
{
	GLuint Loc=GetUniformLocation(Name);
	if(Loc==-1)return;
	glUniformMatrix2fv(Loc,1,GL_FALSE,glm::value_ptr(Data));
}
