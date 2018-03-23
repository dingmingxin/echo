#pragma once
#include "Render/PixelFormat.h"

namespace Echo
{
	class GLES2DepthStencilViewGPUProxy;
	
	class GLES2DepthStencilViewTaskBase
	{
	public:
		GLES2DepthStencilViewTaskBase(GLES2DepthStencilViewGPUProxy* proxy);

	protected:
		GLES2DepthStencilViewGPUProxy* m_proxy;
	};

	class GLES2DepthStencilViewTaskCreateView : public GLES2DepthStencilViewTaskBase
	{
	public:
		GLES2DepthStencilViewTaskCreateView(GLES2DepthStencilViewGPUProxy* proxy, PixelFormat pixFmt, ui32 width, ui32 height);
		void Execute();

	private:
		PixelFormat m_pixFmt;
		ui32 m_width;
		ui32 m_height;
	};

	class GLES2DepthStencilViewTaskDestroyView : public GLES2DepthStencilViewTaskBase
	{
	public:
		GLES2DepthStencilViewTaskDestroyView(GLES2DepthStencilViewGPUProxy* proxy);
		void Execute();
	};

	class GLES2DepthStencilViewTaskOnAttached : public GLES2DepthStencilViewTaskBase
	{
	public:
		GLES2DepthStencilViewTaskOnAttached(GLES2DepthStencilViewGPUProxy* proxy, PixelFormat pixFmt);
		void Execute();

	private:
		PixelFormat m_pixFmt;
	};

	class GLES2DepthStencilViewTaskOnDetached : public GLES2DepthStencilViewTaskBase
	{
	public:
		GLES2DepthStencilViewTaskOnDetached(GLES2DepthStencilViewGPUProxy* proxy);
		void Execute();
	};
}