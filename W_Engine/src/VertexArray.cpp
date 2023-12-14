#include <W_Engine/VertexArray.h>

#include <memory>

namespace W_Engine
{
	VertexArray::VertexArray()
	{
		m_vao = 0;
		glGenVertexArrays(1, &m_vao);
	}
	VertexArray::~VertexArray()
	{
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_vao);
	}

	void VertexArray::SetVertexBuffer(std::unique_ptr<VertexBuffer> vertexBuffer)
	{
		m_vertexBuffer = std::move(vertexBuffer);
		auto& vertexBufferElements = m_vertexBuffer->GetLayout().GetElements();
		for (int i = 0; i < vertexBufferElements.size(); ++i)
		{
			auto& element = vertexBufferElements[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(
				i,
				GetBufferDataTypeComponentCount(element.Type),
				GetBufferDataTypeGLenum(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				m_vertexBuffer->GetLayout().GetStride(),
				reinterpret_cast<const void*>(element.Offset));
		}
	}

	void VertexArray::SetElementBuffer(std::unique_ptr<ElementBuffer> elementBuffer)
	{
		m_elementBuffer = std::move(elementBuffer);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_vao);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
}
