#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> verticies, std::vector<Textures> textures, std::vector<unsigned int> indecies)
{
	this->verticies = verticies;
	this->textures = textures;
	this->indecies = indecies;

	SetupMesh();
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseCount = 1;
	unsigned int specularCount = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseCount++);
		else if (name == "texture_specular")
			number = std::to_string(specularCount++);
		shader.SetUniform1i((name + number).c_str(), i);

		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indecies.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);

}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &vao);
	GlCall(glGenBuffers(1, &vbo));
	GlCall(glGenBuffers(1, &ibo));
	GlCall(glBindVertexArray(vao));

	GlCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));//now using GL_ARRAY_BUFFER will be used to  configure currently bound buffer(vbo)
	GlCall(glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(Vertex), &verticies[0], GL_STATIC_DRAW));//function copies previously defined vertex data(verticies) to the buffers memory currently bounded

	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indecies.size() * sizeof(unsigned int), &indecies[0], GL_STATIC_DRAW));
	GlCall(glEnableVertexAttribArray(0));
	GlCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));// binds to the active GL_ARRAY_BUFFER
	glEnableVertexAttribArray(1);
	GlCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));
	glEnableVertexAttribArray(2);
	GlCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));
	glBindVertexArray(0);
}
