
#include "Mesh.h"
#include "vector"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	std::vector<Mesh> meshes;
	std::vector<Textures> texturesLoaded;
	Model(const std::string path)
	{
		LoadModel(path);
	}

	void Draw(Shader &shader);
private:
	std::string directory;
	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Textures> loadMaterialTextures(aiMaterial* Mat, aiTextureType type, std::string typeName);

};