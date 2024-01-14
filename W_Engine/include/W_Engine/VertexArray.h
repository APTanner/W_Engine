#pragma once

#include <memory>

#include "Buffer.h"
namespace W_Engine 
{
	class VertexArray
	{
	public:
        VertexArray();
        ~VertexArray();

        VertexArray(VertexArray&& other) noexcept :
            m_vao(other.m_vao),
            m_vertexBuffer(std::move(other.m_vertexBuffer)),
            m_elementBuffer(std::move(other.m_elementBuffer))
        {
            other.m_vao = 0;
        }

		void SetVertexBuffer(std::unique_ptr<VertexBuffer> vertexBuffer);
		void SetElementBuffer(std::unique_ptr<ElementBuffer> elementBuffer);

		void Bind() const;
		void Unbind() const;

		inline int GetIndexCount() const { return m_elementBuffer->GetIndexCount(); }

	private:
		unsigned int m_vao;
		std::unique_ptr<VertexBuffer> m_vertexBuffer;
		std::unique_ptr<ElementBuffer> m_elementBuffer;
	};
}

