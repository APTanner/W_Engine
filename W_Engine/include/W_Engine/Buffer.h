#pragma once
#include <glad/glad.h>

#include <initializer_list>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

#include <assimp/AssertHandler.h>


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

	enum class BufferDataType : uint32_t
	{
		Vector2 = 0, Vector3, Vector4,
	};

	enum class BufferDataNormalizationState
	{
		NotNormalized,
		Normalized
	};

	inline int GetBufferDataTypeComponentCount(BufferDataType t)
	{
		return bufferDataTypeData[(uint32_t)t].ComponentCount;
	}

	inline int GetBufferDataTypeElementSize(BufferDataType t)
	{
		return bufferDataTypeData[(uint32_t)t].ElementSize;
	}

	inline GLenum GetBufferDataTypeGLenum(BufferDataType t)
	{
		return bufferDataTypeData[(uint32_t)t].GLType;
	}

	inline int GetBufferDataTypeSize(BufferDataType t)
	{
		return GetBufferDataTypeComponentCount(t) * GetBufferDataTypeElementSize(t);
	}

	struct VertexBufferElement
	{
		BufferDataType Type;
		int Offset;
		int Size;
		bool Normalized;

		std::string Name;

		VertexBufferElement(
			BufferDataType type,
			const std::string& name,
			BufferDataNormalizationState normalized = BufferDataNormalizationState::NotNormalized)
			:
			Type(type),
			Name(name),
			Normalized(normalized == BufferDataNormalizationState::Normalized ? true : false),
			Size(GetBufferDataTypeSize(type)),
			Offset(0)
		{
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}
		BufferLayout(std::initializer_list<VertexBufferElement> elems)
			: m_elements(elems)
		{
			CalculateStrideAndElementOffsets();
		}

		inline const std::vector<VertexBufferElement>& GetElements() const { return m_elements; }

		inline int GetStride() const { return m_stride; }
	private:
		void CalculateStrideAndElementOffsets();
	private:
		std::vector<VertexBufferElement> m_elements;
		int m_stride = 0;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(float* verticies, size_t size, BufferLayout layout);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		inline BufferLayout& GetLayout() { return m_layout; }
	private:
		unsigned int m_vbo = 0;
		BufferLayout m_layout;
	};

	class ElementBuffer
	{
	public:
		ElementBuffer(uint32_t* indicies, size_t size);
		~ElementBuffer();

		void Bind() const;
		void Unbind() const;

		inline int GetIndexCount() const { return m_count; }
	private:
		unsigned int m_ebo = 0;
		unsigned int m_count = 0;
	};
}

