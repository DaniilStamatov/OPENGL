#include <vector>
#include "Shader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};


class Mesh
{
private:
	unsigned int vbo, vao, ibo;
public:
	std::vector<Vertex> verticies;
	std::vector<Texture> textures;
	std::vector<unsigned int> indecies;

	Mesh(std::vector<Vertex> verticies, std::vector<Texture> textures, std::vector<unsigned int> indecies);
	void Draw(Shader& shader);

private:
	void SetupMesh();

};