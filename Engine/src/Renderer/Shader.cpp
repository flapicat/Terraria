#include <pch.h>
#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Log.h"

#include <fstream>
#include <sstream>

namespace Engine {

	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
		std::string vertexSource = ReadFile(vertexPath);
		std::string fragmentSource = ReadFile(fragmentPath);
		m_RendererID = CreateProgram(vertexSource, fragmentSource);
		if (m_RendererID)
			LOG_INFO("Shader created: {} + {}", vertexPath, fragmentPath);
		else
			LOG_ERROR("Shader {} + {} could not be built, anything drawn with it will be invisible", vertexPath, fragmentPath);
	}

	Shader::~Shader() {
		if(m_RendererID)
			glDeleteProgram(m_RendererID);
	}

	void Shader::Bind() const {
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind() const {
		glUseProgram(0);
	}

	std::string Shader::ReadFile(const std::string& path) {
		std::ifstream file(path);

		if (!file.is_open()) {
			LOG_ERROR("Failed to open shader file: {}", path);
			return {};
		}

		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	uint32_t Shader::CompileShader(uint32_t type, const std::string& source) {
		uint32_t shader = glCreateShader(type);
		const char* sourceCStr = source.c_str();
		glShaderSource(shader, 1, &sourceCStr, nullptr);
		glCompileShader(shader);

		int success = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success) {
			int length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			std::string infoLog(length, '\0');
			glGetShaderInfoLog(shader, length, &length, infoLog.data());

			const char* typeName = type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT";
			LOG_ERROR("{} shader compilation failed:\n{}", typeName, infoLog);

			glDeleteShader(shader);
			return 0;
		}

		LOG_INFO("{} shader compiled successfully", type == GL_VERTEX_SHADER ? "Vertex" : "Fragment");
		return shader;
	}

	uint32_t Shader::CreateProgram(const std::string& vertexSource, const std::string& fragmentSource) {
		uint32_t vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
		uint32_t fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

		if (!vertexShader || !fragmentShader) {
			return 0;
		}

		uint32_t program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		int success = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &success);

		if (!success) {
			int length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			std::string infoLog(length, '\0');
			glGetProgramInfoLog(program, length, &length, infoLog.data());

			LOG_ERROR("Shader linking failed:\n{}", infoLog);

			glDeleteProgram(program);
			program = 0;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return program;
	}

	int Shader::GetUniformLocation(const std::string& name) const {
		int location = glGetUniformLocation(m_RendererID, name.c_str());

		if (location == -1 && m_MissingUniforms.insert(name).second)
			LOG_WARN("Uniform '{}' not found in shader {} (misspelled, or optimized out because the shader never uses it)", name, m_RendererID);

		return location;
	}

	void Shader::SetBool(const std::string& name, bool value) {
		glUniform1i(GetUniformLocation(name), value ? 1 : 0);
	}

	void Shader::SetBool2(const std::string& name, bool x, bool y) {
		glUniform2i(GetUniformLocation(name), x, y);
	}

	void Shader::SetBool3(const std::string& name, bool x, bool y, bool z) {
		glUniform3i(GetUniformLocation(name), x, y, z);
	}

	void Shader::SetBool4(const std::string& name, bool x, bool y, bool z, bool w) {
		glUniform4i(GetUniformLocation(name), x, y, z, w);
	}

	void Shader::SetInt(const std::string& name, int value) {
		glUniform1i(GetUniformLocation(name), value);
	}

	void Shader::SetInt2(const std::string& name, int x, int y) {
		glUniform2i(GetUniformLocation(name), x, y);
	}

	void Shader::SetInt3(const std::string& name, int x, int y, int z) {
		glUniform3i(GetUniformLocation(name), x, y, z);
	}

	void Shader::SetInt4(const std::string& name, int x, int y, int z, int w) {
		glUniform4i(GetUniformLocation(name), x, y, z, w);
	}

	void Shader::SetUInt(const std::string& name, uint32_t value) {
		glUniform1ui(GetUniformLocation(name), value);
	}

	void Shader::SetUInt2(const std::string& name, uint32_t x, uint32_t y) {
		glUniform2ui(GetUniformLocation(name), x, y);
	}

	void Shader::SetUInt3(const std::string& name, uint32_t x, uint32_t y, uint32_t z) {
		glUniform3ui(GetUniformLocation(name), x, y, z);
	}

	void Shader::SetUInt4(const std::string& name, uint32_t x, uint32_t y, uint32_t z, uint32_t w) {
		glUniform4ui(GetUniformLocation(name), x, y, z, w);
	}

	void Shader::SetFloat(const std::string& name, float value) {
		glUniform1f(GetUniformLocation(name), value);
	}

	void Shader::SetFloat2(const std::string& name, float x, float y) {
		glUniform2f(GetUniformLocation(name), x, y);
	}

	void Shader::SetFloat3(const std::string& name, float x, float y, float z) {
		glUniform3f(GetUniformLocation(name), x, y, z);
	}

	void Shader::SetFloat4(const std::string& name, float x, float y, float z, float w) {
		glUniform4f(GetUniformLocation(name), x, y, z, w);
	}

	void Shader::SetBool(const std::string& name, const glm::bvec2& value) {
		glUniform2i(GetUniformLocation(name), value.x, value.y);
	}

	void Shader::SetBool(const std::string& name, const glm::bvec3& value) {
		glUniform3i(GetUniformLocation(name), value.x, value.y, value.z);
	}

	void Shader::SetBool(const std::string& name, const glm::bvec4& value) {
		glUniform4i(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void Shader::SetInt(const std::string& name, const glm::ivec2& value) {
		glUniform2iv(GetUniformLocation(name), 1, glm::value_ptr(value));
	}

	void Shader::SetInt(const std::string& name, const glm::ivec3& value) {
		glUniform3iv(GetUniformLocation(name), 1, glm::value_ptr(value));
	}

	void Shader::SetInt(const std::string& name, const glm::ivec4& value) {
		glUniform4iv(GetUniformLocation(name), 1, glm::value_ptr(value));
	}

	void Shader::SetUInt(const std::string& name, const glm::uvec2& value) {
		glUniform2uiv(GetUniformLocation(name), 1, glm::value_ptr(value));
	}

	void Shader::SetUInt(const std::string& name, const glm::uvec3& value) {
		glUniform3uiv(GetUniformLocation(name), 1, glm::value_ptr(value));
	}

	void Shader::SetUInt(const std::string& name, const glm::uvec4& value) {
		glUniform4uiv(GetUniformLocation(name), 1, glm::value_ptr(value));
	}

	void Shader::SetFloat(const std::string& name, const glm::vec2& value) {
		glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(value));
	}

	void Shader::SetFloat(const std::string& name, const glm::vec3& value) {
		glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
	}

	void Shader::SetFloat(const std::string& name, const glm::vec4& value) {
		glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(value));
	}

	void Shader::SetMat3(const std::string& name, const glm::mat3& value) {
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& value) {
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::SetIntArray(const std::string& name, const int* values, uint32_t count) {
		glUniform1iv(GetUniformLocation(name), count, values);
	}

	void Shader::SetUIntArray(const std::string& name, const uint32_t* values, uint32_t count) {
		glUniform1uiv(GetUniformLocation(name), count, values);
	}

	void Shader::SetFloatArray(const std::string& name, const float* values, uint32_t count) {
		glUniform1fv(GetUniformLocation(name), count, values);
	}

}