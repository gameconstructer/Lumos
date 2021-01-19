#include "Precompiled.h"
#include "VKDevice.h"
#include "VKVertexBuffer.h"
#include "VKRenderer.h"
#include "VKPipeline.h"

namespace Lumos
{ 
	namespace Graphics
	{
		VKVertexBuffer::VKVertexBuffer(const BufferUsage& usage)
			: VKBuffer(), m_Usage(usage), m_Size(0)
		{
		}

		VKVertexBuffer::~VKVertexBuffer()
		{
			LUMOS_PROFILE_FUNCTION();
			if (m_MappedBuffer)
			{
				VKBuffer::Flush(m_Size);
				VKBuffer::UnMap();
				m_MappedBuffer = false;
			}
		}

		void VKVertexBuffer::Resize(u32 size)
		{
			LUMOS_PROFILE_FUNCTION();
			m_Size = size;

			VKBuffer::Init(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, size, nullptr);
		}

		void VKVertexBuffer::SetData(u32 size, const void* data)
		{
			LUMOS_PROFILE_FUNCTION();
			VKBuffer::Init(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, size, data);
		}

		void VKVertexBuffer::SetDataSub(u32 size, const void* data, u32 offset)
		{
			LUMOS_PROFILE_FUNCTION();
			VKBuffer::Init(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, size, data);
		}

		void* VKVertexBuffer::GetPointerInternal()
		{
			LUMOS_PROFILE_FUNCTION();
			if (!m_MappedBuffer)
			{
				VKBuffer::Map();
				m_MappedBuffer = true;
			}
			
			return m_Mapped;
		}

		void VKVertexBuffer::ReleasePointer()
		{
			LUMOS_PROFILE_FUNCTION();
			if (m_MappedBuffer)
			{
				VKBuffer::Flush(m_Size);
				VKBuffer::UnMap();
				m_MappedBuffer = false;
			}
		}

		void VKVertexBuffer::Bind(CommandBuffer* commandBuffer, Pipeline* pipeline)
		{
			LUMOS_PROFILE_FUNCTION();
            VkDeviceSize offsets[1] = { 0 };
			if(commandBuffer)
                vkCmdBindVertexBuffers(static_cast<VKCommandBuffer*>(commandBuffer)->GetCommandBuffer(), 0, 1, &m_Buffer, offsets);
		}

		void VKVertexBuffer::Unbind()
		{
		}
        
        void VKVertexBuffer::MakeDefault()
        {
            CreateFunc = CreateFuncVulkan;
        }
        
        VertexBuffer* VKVertexBuffer::CreateFuncVulkan(const BufferUsage& usage)
        {
            return new VKVertexBuffer(usage);
        }
	}
}