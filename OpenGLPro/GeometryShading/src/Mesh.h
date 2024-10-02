#include "Renderer.h"
#include <vector>
#include "Shader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Textures
{
	unsigned int id;
	std::string type;
	std::string path;
};


class Mesh
{
private:
	unsigned int vbo, ibo;
public:
	unsigned int vao;
	std::vector<Vertex> verticies;
	std::vector<Textures> textures;
	std::vector<unsigned int> indecies;

	Mesh(std::vector<Vertex> verticies, std::vector<Textures> textures, std::vector<unsigned int> indecies);
	void Draw(Shader& shader);

private:
	void SetupMesh();

};