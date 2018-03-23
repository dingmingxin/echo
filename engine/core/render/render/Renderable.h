#pragma once

#include "RenderQueue.h"
#include <engine/core/Util/PtrMonitor.h>
#include <engine/core/Util/Array.hpp>
#include "ShaderProgram.h"
#include "Texture.h"
#include "RenderState.h"

namespace Echo
{
	static const size_t MAX_TEXTURE_SAMPLER = 8;
	static const int	SHADER_TEXTURE_SLOT0= 0;	// Ĭ��ʹ��������
	static const int	SHADER_TEXTURE_SLOT1= 1;	// Ĭ��ʹ��������
	static const int	SHADER_TEXTURE_SLOT2= 2;	// Ĭ��ʹ��������
	static const int	SHADER_TEXTURE_SLOT3= 3;	// Ĭ��ʹ��������
	static const int	SHADER_TEXTURE_SLOT4= 4;	// Ĭ��ʹ��������
	static const int	SHADER_TEXTURE_SLOT5= 5;	// Ĭ��ʹ��������
	static const int	SHADER_TEXTURE_SLOT6= 6;	// Ĭ��ʹ��������
	static const int	SHADER_TEXTURE_SLOT7= 7;	// Ĭ��ʹ��������

	/**
	 * ��Ⱦ�ӿ�
	 */
	class ECHO_EXPORT_RENDER IManualRenderable
	{
	public:
		virtual ~IManualRenderable(){}

		// ִ����Ⱦ
		virtual void render()=0;
	};

	/**
	 * ��С����Ⱦ���װ
	 */
	class ECHO_EXPORT_RENDER Renderable : public IManualRenderable
	{
		friend class RenderQueue;
		friend class Renderer;
		typedef ui32 MaterialID;
		typedef array<TextureSampler, MAX_TEXTURE_SAMPLER> MaxTextureArray;
	public:
		// ��ɫ������
		struct ShaderParam
		{
			ui32			physicsIndex;
			ShaderParamType stype;
			const void*		pData;
			ui32			ParamsLength;  // shader constance register num.
		};

	public:
		// ��ȡΨһ��ʶ��
		ui32 getIdentifier() const { return m_identifier; }

		// ��ȡ��Ⱦ��������
		RenderInput* getRenderInput() const { return m_RenderInput; }

		// ������Ⱦ��������
		inline void setRenderInput(RenderInput* input) { m_RenderInput = input; }

		// ��ʼ����
		void beginShaderParams(size_t paramNum);

		// ����������
		void endShaderParams();

		// ���ò���
		void setShaderParam(size_t physicsIndex, ShaderParamType type, const void* param, size_t num=1);
		
		// �޸Ĳ���
		void modifyShaderParam(ui32 physics, ShaderParamType type, void* param, size_t num=1);

		// �޸�Low����
		void modifyLowShaderParam(ui32 physics, ShaderParamType type, void* param, ui32 id, size_t num = 1);

		// ��������
		void setTexture( ui32 stage, Texture* texture, const SamplerState* state);

		// ��ȡ����
		Texture* getTexture(ui32 stage) const { return m_Textures[stage].m_texture; }

		// ִ����Ⱦ
		virtual void render();
		
		// ������Ⱦ����
		void setRenderQueue(RenderQueue* pRenderQueue) { m_pRenderQueue = pRenderQueue; }

		// ���õ�Ч����Ⱦ����
		void setLowRenderQueue(RenderQueue* pRenderQueue, int id = 0,int reSetPhysicsIndex = 0);

		// ��������Ⱦ����
		void setStageRenderQueue(RenderQueue* stageRenderQueue);

		void resizeLowShaderParam(ui32 size){ m_LowShaderParams.resize(size); }

		void setLowShaderParam(ui32 id, size_t physicsIndex, ShaderParamType type, const void* param, size_t num = 1);

		void setUseDefaultShaderParams(bool setting){ m_useDefaultShaderParams = setting; }

		// ��ȡ��Ⱦ����
		const RenderQueue* getRenderQueue(void){ return m_pRenderQueue; }
		const RenderQueue* getLowRenderQueue(void){ return m_pLowRenderQueue; }

		// �ύ����Ⱦ����
		void submitToRenderQueue();

		void setVisible(bool* state) { m_visible = state; }
		bool isVisible() { return *m_visible; }

		// �Ƿ����Լ�����Ⱦ״̬
		bool isHaveRenderState( ) { return m_bRenderState; }

		// ������Ⱦ״̬
		void setBlendState( BlendState* state );

		// ���ù�դ��״̬
		void setRasterizerState( RasterizerState* state );

		// �������ģ��״̬
		void setDepthStencilState(DepthStencilState* state);

		// ��ȡ��Ⱦ״̬
		BlendState*	getBlendState() { return m_pBlendState; }

		// ��ȡ��դ��״̬
		RasterizerState* getRasterizerState() { return m_pRasterizerState; }

		// ��ȡ���ģ��״̬
		DepthStencilState* getDepthStencilState() { return m_pDepthStencil; }

		// ����ӵ������Ϣ,�������
		void setOwnerDesc(const String& ownerDesc) { m_ownerDesc = ownerDesc; }

	protected:
		// ������������������
		void bindTextures();

		// �󶨲���
		void bindShaderParams();

		// ����Ⱦ״̬
		void bindRenderState();

	private:
		Renderable( RenderQueue* pRenderQueue, Material* material, int identifier);
		virtual ~Renderable();

	public:
		ui32									m_identifier;
		String									m_ownerDesc;			// ӵ������Ϣ
		RenderQueue*							m_stageRenderQueue;		// Opaque,Transparent ������Ⱦ����
		RenderQueue*							m_pRenderQueue;
		RenderQueue*							m_pLowRenderQueue;
		MaterialID								m_materialID;			// material
		RenderInput*							m_RenderInput;			// first  VB, IB, etc.
		MaxTextureArray							m_Textures;				// now only one texture sampler.
		vector<ShaderParam>::type				m_ShaderParams;			// third shader params
		vector<vector<ShaderParam>::type>::type	m_LowShaderParams;		// third shader params
		size_t									m_LowShaderID;
		size_t									m_SParamWriteIndex;
		vector<size_t>::type					m_LowSParamWriteIndex;
		bool*									m_visible;

		bool									m_bRenderState;
		BlendState*								m_pBlendState;
		RasterizerState*						m_pRasterizerState;
		DepthStencilState*						m_pDepthStencil;
		Vector3									m_worldPos;
		bool									m_useDefaultShaderParams;
	};
	typedef ui32 RenderableID;
}