#pragma once
#include <string>
#include <cstdint>
#include <unordered_set>
#include <glm/glm.hpp>

namespace Engine {

    class Shader
    {
    public:
        Shader(const std::string& vertexPath,const std::string& fragmentPath);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        uint32_t GetRendererID() const { return m_RendererID; }

        // Bool
        void SetBool(const std::string& name, bool value);
        void SetBool2(const std::string& name, bool x, bool y);
        void SetBool3(const std::string& name, bool x, bool y, bool z);
        void SetBool4(const std::string& name, bool x, bool y, bool z, bool w);

        // Int
        void SetInt(const std::string& name, int value);
        void SetInt2(const std::string& name, int x, int y);
        void SetInt3(const std::string& name, int x, int y, int z);
        void SetInt4(const std::string& name, int x, int y, int z, int w);

        // UInt
        void SetUInt(const std::string& name, uint32_t value);
        void SetUInt2(const std::string& name, uint32_t x, uint32_t y);
        void SetUInt3(const std::string& name, uint32_t x, uint32_t y, uint32_t z);
        void SetUInt4(const std::string& name,uint32_t x,uint32_t y,uint32_t z,uint32_t w);

        // Float
        void SetFloat(const std::string& name, float value);
        void SetFloat2(const std::string& name, float x, float y);
        void SetFloat3(const std::string& name, float x, float y, float z);
        void SetFloat4(const std::string& name,float x,float y,float z,float w);

        // GLM vectors
        void SetBool(const std::string& name, const glm::bvec2& value);
        void SetBool(const std::string& name, const glm::bvec3& value);
        void SetBool(const std::string& name, const glm::bvec4& value);

        void SetInt(const std::string& name, const glm::ivec2& value);
        void SetInt(const std::string& name, const glm::ivec3& value);
        void SetInt(const std::string& name, const glm::ivec4& value);

        void SetUInt(const std::string& name, const glm::uvec2& value);
        void SetUInt(const std::string& name, const glm::uvec3& value);
        void SetUInt(const std::string& name, const glm::uvec4& value);

        void SetFloat(const std::string& name, const glm::vec2& value);
        void SetFloat(const std::string& name, const glm::vec3& value);
        void SetFloat(const std::string& name, const glm::vec4& value);

        // Matrices
        void SetMat3(const std::string& name, const glm::mat3& value);
        void SetMat4(const std::string& name, const glm::mat4& value);

        // Arrays
        void SetIntArray(const std::string& name,const int* values,uint32_t count);
        void SetUIntArray(const std::string& name,const uint32_t* values,uint32_t count);
        void SetFloatArray(const std::string& name,const float* values,uint32_t count);
    private:
        std::string ReadFile(const std::string& path);
        uint32_t CompileShader(uint32_t type,const std::string& source);
        uint32_t CreateProgram(const std::string& vertexSource, const std::string& fragmentSource);
        int GetUniformLocation(const std::string& name) const;
    private:
        uint32_t m_RendererID = 0;
        mutable std::unordered_set<std::string> m_MissingUniforms;
    };

}