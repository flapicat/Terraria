#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BufferLayout.h"

#include <memory>

namespace Engine {

    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;
        void Unbind() const;

        void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer, const BufferLayout& layout);
        
        void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

        const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const
        {
            return m_IndexBuffer;
        }

    private:
        uint32_t m_RendererID = 0;

        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
    };

}
