#pragma once

namespace Engine
{
    class VertexBuffer {
    public:
        VertexBuffer(const void* data, uint32_t size);
        VertexBuffer(uint32_t capacity);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;

        void SetData(const void* data, uint32_t size);

        uint32_t GetID() const { return m_RendererID; }
    private:
        uint32_t m_RendererID = 0;
        uint32_t m_Capacity = 0;
    };

}
