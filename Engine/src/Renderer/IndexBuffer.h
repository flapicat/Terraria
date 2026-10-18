#pragma once

#include <cstdint>

namespace Engine {

    class IndexBuffer {
    public:
        IndexBuffer(const uint32_t* indices, uint32_t count);
        IndexBuffer(uint32_t capacity);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;

        void SetData(const void* data, uint32_t count);

        uint32_t GetCount() const { return m_Count; }
        uint32_t GetID() const { return m_RendererID; }
    private:
        uint32_t m_RendererID = 0;
        uint32_t m_Count = 0;
        uint32_t m_Capacity = 0;
    };

}
