#include <pch.h>
#include "VertexBuffer.h"

#include <glad/glad.h>

namespace Engine {

    VertexBuffer::VertexBuffer(const void* data, uint32_t count)
        : m_Capacity(count)
    {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

        glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_STATIC_DRAW);
    }

    VertexBuffer::VertexBuffer(uint32_t capacity)
        : m_Capacity(capacity)
    {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

        glBufferData(GL_ARRAY_BUFFER, capacity * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()
    {
        if(m_RendererID)
            glDeleteBuffers(1, &m_RendererID);
    }

    void VertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void VertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::SetData(const void* data, uint32_t count)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

        // Only reallocate when the data doesn't fit, otherwise update in place
        if (count > m_Capacity)
        {
            glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_DYNAMIC_DRAW);
            m_Capacity = count;
            return;
        }
        glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(float), data);
    }

}
