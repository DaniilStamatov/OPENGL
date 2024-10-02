#include "Texture.h"
#include "vendor/stb_image/stb_image.h"
#include "Renderer.h"
Texture::Texture(const std::string& filepath) : m_RendererID(0), m_Height(0), m_Width(0), m_BPP(0), m_localBuffer(nullptr)
{
	m_localBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 0);
	
	GlCall(glGenTextures(1, &m_RendererID));
	GlCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLenum format;
	if (m_BPP == 1) format = 1;
	else if (m_BPP == 3)
		format = GL_RGB;
	else if (m_BPP == 4)
		format = GL_RGBA;

	// second param is mipmap level
	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_localBuffer));
	GlCall(glGenerateMipmap(GL_TEXTURE_2D));

	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GlCall(glBindTexture(GL_TEXTURE_2D, 0));


	if (m_localBuffer)
		stbi_image_free(m_localBuffer);
}

Texture::~Texture()
{
	GlCall(glDeleteTextures(1, &m_RendererID));
}
void Texture::Bind(unsigned int slot) const
{
	GlCall(glActiveTexture(GL_TEXTURE0 + slot));
	GlCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GlCall(glBindTexture(GL_TEXTURE_2D, 0));
}