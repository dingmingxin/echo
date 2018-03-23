#include <engine/core/Util/LogManager.h>
#include "Render/ShaderProgram.h"
#include "Render/Material.h"
#include "Render/Renderer.h"
#include "Render/UniformCache.h"

namespace Echo
{
	// ���캯��
	ShaderProgram::ShaderProgram( Material* material)
		: m_bLinked(false)
		, m_material( material)
	{
		for(ui32 i = 0; i < Shader::ST_SHADERCOUNT; ++i)
		{
			m_pShaders[i] = NULL;
		}
	}

	// ��������
	ShaderProgram::~ShaderProgram()
	{
		for(ui32 i = 0; i < Shader::ST_SHADERCOUNT; ++i)
		{
			EchoSafeDelete(m_pShaders[i], Shader);
		}
	}

	bool ShaderProgram::attachShader(Shader* pShader)
	{
		if(!pShader)
			return false;

		Shader::ShaderType type = pShader->getShaderType();
		if(m_pShaders[(ui32)type])
		{
			EchoLogError("The shader [%s] has been already attached.", Shader::GetShaderTypeDesc(type).c_str());
			return false;
		}

		m_pShaders[(ui32)type] = pShader;
		pShader->setShaderProgram(this);
		m_bLinked = false;

		return true;
	}

	Shader* ShaderProgram::detachShader(Shader::ShaderType type)
	{
		Shader* pShader = m_pShaders[(ui32)type];
		m_pShaders[(ui32)type] = NULL;

		m_bLinked = false;

		return pShader;
	}

	// ���ݱ�����������ֵ
	void ShaderProgram::setUniform( const char* name, const void* value, ShaderParamType uniformType, ui32 count)
	{
		setUniform(getParamPhysicsIndex(name), value, uniformType, count);
	}

	// ��ȡ����������ַ
	int ShaderProgram::getParamPhysicsIndex(const String& paramName)
	{
		for (UniformArray::iterator it = m_uniforms.begin(); it != m_uniforms.end(); it++)
		{
			if (it->second.m_name == paramName)
				return it->second.m_location;
		}

		EchoLogError("%s is not param of shader program [%s] vs-[%s] ps-[%s]", paramName.c_str(), m_material->getName().c_str(), m_pShaders[Shader::ST_VERTEXSHADER]->getName().c_str(), m_pShaders[Shader::ST_PIXELSHADER]->getName().c_str());
		EchoAssert(false);

		return 0;
	}

	// ���ݱ������ͻ�ȡ���ӦByteSize
	int ShaderProgram::getUniformByteSizeByUniformType( ShaderParamType uniformType)
	{
		switch( uniformType)
		{
		case SPT_UNKNOWN:			return 0;  
		case SPT_INT:				return 4;  
		case SPT_FLOAT:				return 4;  
		case SPT_VEC2:				return 8;  
		case SPT_VEC3:				return 12;  
		case SPT_VEC4:				return 16;  
		case SPT_MAT4:				return 64;
		case SPT_TEXTURE:			return 4;
		default:					return 0;
		}
	}

	// ���������������ñ���ֵ
	void ShaderProgram::setUniform(ui32 physicIdx, const void* value, ShaderParamType uniformType, ui32 count)
	{
		EchoAssert(value);

		UniformArray::iterator it = m_uniforms.find(physicIdx);
		if (it != m_uniforms.end())
		{
#ifdef ECHO_RENDER_THREAD
			Uniform& uniform = it->second;
			Byte* uniform_cache_address = g_uniform_cache->AppendUniform(value, uniformType, uniform.m_sizeInBytes);
			uniform.setValue(uniform_cache_address);
#else
			Uniform& uniform = it->second;
			uniform.setValue(value);
#endif
		}
		else
		{
			EchoLogError("ShaderProgram uniform [%d] not exist!", physicIdx);
		}
	}

	// �������ƻ�ȡUniform
	ShaderProgram::Uniform* ShaderProgram::getUniform(const String& name)
	{
		for (UniformArray::iterator it = m_uniforms.begin(); it != m_uniforms.end(); it++)
		{
			if (it->second.m_name == name)
				return &it->second;
		}

		return NULL;
	}

	// �������Ϸ���
	bool ShaderProgram::checkValid()
	{
		for (UniformArray::iterator it = m_uniforms.begin(); it != m_uniforms.end(); it++)
		{
			if (!it->second.m_value)
				return false;
		}

		return true;
	}

	i32 ShaderProgram::getAtrribLocation(RenderInput::VertexSemantic vertexSemantic)
	{
		return 0;
	}

}