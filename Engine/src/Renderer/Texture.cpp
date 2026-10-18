#include "pch.h"
#include "Texture.h"

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "Core/Log.h"

namespace Engine {

	Texture::Texture(int32_t Width, int32_t Height, unsigned char* data, int32_t Format)
	{
		m_Width = Width;
		m_Height = Height;
		m_Format = Format;
		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		GLenum channelFormat = GL_RGB;
		if (m_Format == 3) channelFormat = GL_RGB;
		if (m_Format == 4) channelFormat = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, channelFormat, m_Width, m_Height, 0, channelFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		LOG_INFO("Texture created successfully: {}x{}, {} channels", m_Width, m_Height, m_Format);
	}

	Texture::Texture(const std::string& TexturePath)
	{
		stbi_set_flip_vertically_on_load(true);
		unsigned char* image = stbi_load(TexturePath.c_str(), &m_Width, &m_Height, &m_Format, 0);
		if (!image)
		{
			LOG_ERROR("Failed to load texture '{}': {}", TexturePath, stbi_failure_reason());
			return;
		}

		if (m_Format != 3 && m_Format != 4)
			LOG_WARN("Texture '{}' has {} channels, only RGB and RGBA are supported so it will look wrong", TexturePath, m_Format);

		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		GLenum channelFormat = GL_RGB;
		if (m_Format == 3) channelFormat = GL_RGB;
		if (m_Format == 4) channelFormat = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, channelFormat, m_Width, m_Height, 0, channelFormat, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		LOG_INFO("Texture loaded successfully: {} ({}x{}, {} channels)", TexturePath, m_Width, m_Height, m_Format);

		stbi_image_free(image);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture::~Texture()
	{
		if (m_Texture)
			glDeleteTextures(1, &m_Texture);
	}

	void Texture::Bind(uint32_t index)
	{
		if (m_Texture)
		{
			m_BindedIndex = index;
			glActiveTexture(GL_TEXTURE0 + index);
			glBindTexture(GL_TEXTURE_2D, m_Texture);
		}
	}

	void Texture::UnBind()
	{
		glActiveTexture(GL_TEXTURE0 + m_BindedIndex);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::Delete()
	{
		if (m_Texture)
			glDeleteTextures(1, &m_Texture);
	}
}