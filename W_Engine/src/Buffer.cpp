#include <W_Engine/Buffer.h>

namespace W_Engine
{
	void BufferLayout::CalculateStrideAndElementOffsets()
	{
		int strideSoFar{};
		for (int i = 0; i < m_elements.size(); ++i)
		{
			VertexBufferElement& elem = m_elements[i];
			elem.Offset = strideSoFar;
			strideSoFar += elem.Size;
		}
		m_stride = strideSoFar;
	}

	VertexBuffer::VertexBuffer(float* verticies, size_t size, BufferLayout layout)
	{
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW);
		m_layout = layout;
	}
	VertexBuffer::~VertexBuffer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &m_vbo);
	}
	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	}
	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	ElementBuffer::ElementBuffer(uint32_t* indicies, size_t size)
	{
		m_count = size / sizeof(uint32_t);
		glGenBuffers(1, &m_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indicies, GL_STATIC_DRAW);
	}
	ElementBuffer::~ElementBuffer()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &m_ebo);
	}
	void ElementBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	}
	void ElementBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
