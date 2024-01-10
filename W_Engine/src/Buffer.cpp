#include <W_Engine/Buffer.h>

#include <assimp/AssertHandler.h>

#include <glad/glad.h>

namespace W_Engine
{
    struct BufferDataTypeData
    {
        int ComponentCount;
        int ElementSize;
        GLenum GLType;
    };

    static constexpr uint32_t numBufferDataTypes = 3;

    constexpr BufferDataTypeData bufferDataTypeData[numBufferDataTypes] = {
        {2, sizeof(float), GL_FLOAT}, //vector2
        {3, sizeof(float), GL_FLOAT}, //vector3
        {4, sizeof(float), GL_FLOAT}, //vector4
    };

    int GetBufferDataTypeComponentCount(BufferDataType t)
    {
        return bufferDataTypeData[(uint32_t)t].ComponentCount;
    }

    int GetBufferDataTypeElementSize(BufferDataType t)
    {
        return bufferDataTypeData[(uint32_t)t].ElementSize;
    }

    unsigned int GetBufferDataTypeGLenum(BufferDataType t)
    {
        return bufferDataTypeData[(uint32_t)t].GLType;
    }

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
