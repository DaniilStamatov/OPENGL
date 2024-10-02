#pragma once
#include <string>
#include <glad/glad.h>

class Texture
{
private:
	unsigned int m_RendererID;
	unsigned char* m_localBuffer;
	int m_Width, m_Height, m_BPP;

public:
	Texture(const std::string& filepath);
	~Texture();
	void Bind(unsigned int slot) const;
	void Unbind() const;
};