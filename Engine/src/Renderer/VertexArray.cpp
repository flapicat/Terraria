#include <pch.h>
#include "VertexArray.h"

#include <glad/glad.h>

namespace Engine {

    VertexArray::VertexArray()
    {
        glCreateVertexArrays(1, &m_RendererID);
        glBindVertexArray(m_RendererID);
    }

    VertexArray::~VertexArray()
    {
        if(m_RendererID)
            glDeleteVertexArrays(1, &m_RendererID);
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_RendererID);
    }

    void VertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer, const BufferLayout& layout)
    {
        this->Bind();
        vertexBuffer->Bind();
        uint32_t index = 0;
        for (const auto& element : layout) {
            if (element.Type != ShaderDataType::Float && element.Type != ShaderDataType::Float2 &&
                element.Type != ShaderDataType::Float3 && element.Type != ShaderDataType::Float4)
                LOG_WARN("Vertex attribute '{}' is not a float type, VertexArray uploads every attribute as floats so it will read wrong data", element.Name);

            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index,
                element.GetComponentCount(),
                GL_FLOAT,
                GL_FALSE,
                layout.GetStride(),
                reinterpret_cast<const void*>(element.Offset)
            );
            index++;
        }
        m_VertexBuffer = vertexBuffer;
    }

    void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
    {
        this->Bind();

        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }

}