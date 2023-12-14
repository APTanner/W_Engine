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

