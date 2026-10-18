#include <pch.h>
#include "IndexBuffer.h"

#include <glad/glad.h>

namespace Engine {

    IndexBuffer::IndexBuffer(const uint32_t* indices, uint32_t count)
        : m_Count(count), m_Capacity(count)
    {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t),indices, GL_STATIC_DRAW);
    }

    IndexBuffer::IndexBuffer(uint32_t capacity)
        : m_Count(0), m_Capacity(capacity)
    {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, capacity * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);
    }

    IndexBuffer::~IndexBuffer()
    {
        if(m_RendererID)
            glDeleteBuffers(1, &m_RendererID);
    }

    void IndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void IndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void IndexBuffer::SetData(const void* data, uint32_t count)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);

        // Only reallocate when the data doesn't fit, otherwise update in place
        if (count > m_Capacity)
        {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_DYNAMIC_DRAW);
            m_Capacity = count;
        }
        else
        {
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(uint32_t), data);
        }
        m_Count = count;
    }

}
