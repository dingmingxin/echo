#pragma once
#include "Render/ShaderProgram.h"
#include "engine/core/Memory/MemAllocDef.h"
#include "GLES2ShaderProgram.h"

namespace Echo
{
	class GLES2ShaderGPUProxy;
	class GLES2ShaderProgramGPUProxy;

	class GLES2ShaderProgramTaskBase
	{
	public:
		GLES2ShaderProgramTaskBase(GLES2ShaderProgramGPUProxy* proxy);

	protected:
		GLES2ShaderProgramGPUProxy* m_proxy;
	};

	class GLES2ShaderProgramTaskDestroyProxy : public GLES2ShaderProgramTaskBase
	{
	public:
		GLES2ShaderProgramTaskDestroyProxy(GLES2ShaderProgramGPUProxy* proxy);
		void Execute();
	};

	class GLES2ShaderProgramTaskCreateProgram : public GLES2ShaderProgramTaskBase
	{
	public:
		GLES2ShaderProgramTaskCreateProgram(GLES2ShaderProgramGPUProxy* proxy);
		void Execute();
	};

	class GLES2ShaderProgramTaskLinkShaders : public GLES2ShaderProgramTaskBase
	{
	public:
		GLES2ShaderProgramTaskLinkShaders(GLES2ShaderProgramGPUProxy* proxy);
		void Execute();
	};

	class GLES2ShaderProgramTaskUniformReflection : public GLES2ShaderProgramTaskBase
	{
	public:
		GLES2ShaderProgramTaskUniformReflection(GLES2ShaderProgramGPUProxy* proxy, ShaderProgram::UniformArray* uniform_array);
		void Execute();

	private:
		ShaderProgram::UniformArray* m_uniform_array;
	};

	class GLES2ShaderProgramTaskAttribReflection : public GLES2ShaderProgramTaskBase
	{
	public:
		GLES2ShaderProgramTaskAttribReflection(GLES2ShaderProgramGPUProxy* proxy, GLES2ShaderProgram::AttribLocationArray* attrib_array);
		void Execute();

	private:
		GLES2ShaderProgram::AttribLocationArray*	m_attrib_array;
	};

	class GLES2ShaderProgramTaskBindUniforms : public GLES2ShaderProgramTaskBase
	{
		struct UniformData
		{
			ShaderParamType	type;
			int location;
			int count;
			Byte* value;
		};

	public:
		GLES2ShaderProgramTaskBindUniforms(GLES2ShaderProgramGPUProxy* proxy, const ShaderProgram::UniformArray& uniform_array);
		GLES2ShaderProgramTaskBindUniforms(GLES2ShaderProgramGPUProxy* proxy, size_t offset, size_t count);
		void Execute();

	private:
		vector<UniformData>::type m_uniform_list;

		size_t m_cache_offset;
		size_t m_cache_count;
	};

	class GLES2ShaderProgramTaskAttachShader : public GLES2ShaderProgramTaskBase
	{
	public:
		GLES2ShaderProgramTaskAttachShader(GLES2ShaderProgramGPUProxy* proxy, GLES2ShaderGPUProxy* shader_proxy);
		void Execute();

	private:
		GLES2ShaderGPUProxy* m_shader_proxy;
	};

	class GLES2ShaderProgramTaskDetachShader : public GLES2ShaderProgramTaskBase
	{
	public:
		GLES2ShaderProgramTaskDetachShader(GLES2ShaderProgramGPUProxy* proxy, GLES2ShaderGPUProxy* shader_proxy);
		void Execute();

	private:
		GLES2ShaderGPUProxy* m_shader_proxy;
	};

	class GLES2ShaderProgramTaskUseProgram : public GLES2ShaderProgramTaskBase
	{
	public:
		GLES2ShaderProgramTaskUseProgram(GLES2ShaderProgramGPUProxy* proxy);

		void Execute();
	};
}