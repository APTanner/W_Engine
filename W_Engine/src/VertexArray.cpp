#include <W_Engine/VertexArray.h>

#include <memory>
#include <iostream>

#include <glad/glad.h>

namespace W_Engine
{
	VertexArray::VertexArray()
	{
		m_vao = 0;
		glGenVertexArrays(1, &m_vao);
	}
	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void VertexArray::SetVertexBuffer(std::unique_ptr<VertexBuffer> vertexBuffer)
	{
		m_vertexBuffer = std::move(vertexBuffer);

        Bind();
        m_vertexBuffer->Bind();

		auto& vertexBufferElements = m_vertexBuffer->GetLayout().GetElements();
		for (int i = 0; i < vertexBufferElements.size(); ++i)
		{
			auto& element = vertexBufferElements[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(
				i,
				GetBufferDataTypeComponentCount(element.Type),
				static_cast<GLenum>(GetBufferDataTypeGLenum(element.Type)),
				element.Normalized ? GL_TRUE : GL_FALSE,
				m_vertexBuffer->GetLayout().GetStride(),
				reinterpret_cast<const void*>(element.Offset)
            );
		}

        Unbind();
        m_vertexBuffer->Unbind();
	}

	void VertexArray::SetElementBuffer(std::unique_ptr<ElementBuffer> elementBuffer)
	{
		m_elementBuffer = std::move(elementBuffer);

        Bind();
        m_elementBuffer->Bind();

        Unbind();
        m_elementBuffer->Unbind();
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
